#include "sv_iot.h"

#define IOT_IDLE_TIMEOUT	15000
#define IOT_OPEN_TIMEOUT	5000
#define IOT_SUB_TIMEOUT		5000
#define IOT_SYNC_TIMEOUT	5000

//#define IOT_
#define JSON_OBJ_MAX		32
#define IOT_RETRY_CONNECT_MAX	5
static void sv_iot_on_conn(int32_t err, void* arg);
static void sv_iot_on_sub(int32_t err, void* arg);
static void sv_iot_on_pub(int32_t err, void* arg);
static void sv_iot_on_disc(int32_t err, void* arg);
static void sv_iot_on_msg(sm_mqtt_client_t* mqtt_client,sm_mqtt_msg_t* msg, void* arg);

static void sv_iot_on_config_msg(sm_mqtt_msg_t* msg, void* arg);
static void sv_iot_on_command_msg(sm_mqtt_msg_t* msg, void* arg);

sv_iot_t g_sv_iot;

sv_iot_t* sv_iot_create(sm_mqtt_client_t* client,
						sv_iot_callback_t 	*event_cb,
						void				*event_arg){
	if (!client || !event_cb || !event_arg) return NULL;

	g_sv_iot.mqtt_client = client;
	g_sv_iot.event_cb = NULL;
	g_sv_iot.event_arg = NULL;
	g_sv_iot.sub_topic_num = 0;
	for (int i=0; i< IOT_SUB_TOPIC_NUMBER; i++){
		g_sv_iot.sub_topics->topic = NULL;
		g_sv_iot.sub_topics->topic_handle = NULL;
	}
	for (int i=0; i< IOT_PUB_QUEUE_SIZE; i++){
		g_sv_iot.pub_topics->topic = NULL;
		g_sv_iot.pub_topics->payload = NULL;
	}
	g_sv_iot.pub_head = 0;
	g_sv_iot.pub_tail = 0;
	g_sv_iot.state = IOT_STATE_IDLE;

	g_sv_iot.event_cb = event_cb;
	g_sv_iot.event_arg = event_arg;

	g_sv_iot.retry = 0;

	g_sv_iot.sub_topics[0].topic = sv_iot_get_config_topic();
	g_sv_iot.sub_topics[0].topic_handle = sv_iot_on_config_msg;

	g_sv_iot.sub_topics[1].topic = sv_iot_get_command_topic();
	g_sv_iot.sub_topics[1].topic_handle = sv_iot_on_command_msg;

	g_sv_iot.sub_topic_num = 0;

	sm_mqtt_set_incoming_data_cb(g_sv_iot.mqtt_client, sv_iot_on_msg, &g_sv_iot);
	elapsed_timer_resetz(&g_sv_iot.timeout, IOT_IDLE_TIMEOUT);
	return &g_sv_iot;
}

int32_t sv_iot_get_state(sv_iot_t* this){
	return (int32_t) this->state;
}

int32_t sv_iot_push_data_to_cloud(sv_iot_t* this, char* topic, char* payload){
	if (!this || !topic || !payload) return -1;

	this->pub_topics[this->pub_head].topic = topic;
	memcpy(this->pub_topics[this->pub_head].payload, payload, strlen(payload));

	if (++this->pub_head == IOT_PUB_QUEUE_SIZE){
		this->pub_head = 0;
	}
	return 0;
}

int32_t sv_iot_process(sv_iot_t* this){
	if (!this) return -1;
	int32_t err = 0;
	switch (this->state){
	case IOT_STATE_IDLE:
		if (!elapsed_timer_get_remain(&this->timeout)){
			this->state = IOT_STATE_OPEN;
		}
		break;
	case IOT_STATE_OPEN:
		if (!elapsed_timer_get_remain(&this->timeout)){
			err = sm_mqtt_connect(this->mqtt_client,
									IOT_HOST, IOT_PORT,
									this->device_name,
									IOT_USER, IOT_PASS,
									sv_iot_on_conn,
									this);
			if (err){
				if (++this->retry > IOT_RETRY_CONNECT_MAX){
					this->state = IOT_STATE_DISCONNECT;
				}else{
					elapsed_timer_resetz(&this->timeout, IOT_OPEN_TIMEOUT);
				}
			}else{
				this->state = IOT_STATE_CONNECT;
			}
		}
		break;
	case IOT_STATE_CONNECT:
		this->state = IOT_STATE_SUBSCRIBE;
		this->retry = 0;
		elapsed_timer_resetz(&this->timeout, IOT_SUB_TIMEOUT);
		break;
	case IOT_STATE_SUBSCRIBE:
		if (!elapsed_timer_get_remain(&this->timeout)){
			err = 0;
			if (this->sub_topics[this->sub_topic_num].topic != NULL){
				err = sm_mqtt_subscribe(this->mqtt_client,
										this->sub_topics[this->sub_topic_num].topic,
										0,
										sv_iot_on_sub,
										this);
			}
			if (err) {
				if (++this->retry > IOT_RETRY_CONNECT_MAX){
					this->state = IOT_STATE_DISCONNECT;
				}
			}else{
				if (++this->sub_topic_num == IOT_SUB_TOPIC_NUMBER){
					this->state = IOT_STATE_RUNNING;

				}
			}
		}
		break;
	case IOT_STATE_RUNNING:
		err = sm_mqtt_client_process(this->mqtt_client);

		if (err){
			sv_iot_on_disc(err, this);
		}else{
			if (this->event_cb->on_sync && !elapsed_timer_get_remain(&this->timeout)){
				char sync_buff[IOT_PAYLOAD_SIZE];
				memset(sync_buff, 0, IOT_PAYLOAD_SIZE);

				this->event_cb->on_sync(sync_buff, this->event_arg);
				sv_iot_push_data_to_cloud(this, sv_iot_get_sync_topic(this->device_name), sync_buff);
			}

		}

		if (this->pub_tail != this->pub_head){
			sm_mqtt_publish(this->mqtt_client,
							this->pub_topics[this->pub_tail].topic,
							this->pub_topics[this->pub_tail].payload,
							0, 0,
							sv_iot_on_pub,
							this);

			this->pub_topics[this->pub_tail].topic = NULL;
			this->pub_topics[this->pub_tail].payload = NULL;
			if (++this->pub_tail == IOT_PUB_QUEUE_SIZE){
				this->pub_tail = 0;
			}
		}
		break;
	case IOT_STATE_DISCONNECT:
		elapsed_timer_resetz(&this->timeout, IOT_IDLE_TIMEOUT);
		sm_mqtt_disconnect(this->mqtt_client, NULL, this);
		this->state = IOT_STATE_IDLE;
		this->sub_topic_num = 0;
		if (this->event_cb->on_disc){
			this->event_cb->on_disc(this->event_arg);
		}
		break;
	}
}

static void sv_iot_on_conn(int32_t err, void* arg){
	if (err){
		sv_iot_t* this = (sv_iot_t*)arg;
		if (this->event_cb->on_conn){
			this->event_cb->on_conn(this->event_arg);
		}
	}
}

static void sv_iot_on_sub(int32_t err, void* arg){
	if (err){
		sv_iot_t* this = (sv_iot_t*)arg;
		///TODO:
	}
}

static void sv_iot_on_pub(int32_t err, void* arg){
	if (err){
		sv_iot_t* this = (sv_iot_t*)arg;
		/// TODO:
	}
}

static void sv_iot_on_disc(int32_t err, void* arg){
	if (err){
		sv_iot_t* this = (sv_iot_t*)arg;
		/// TODO:
		this->state = IOT_STATE_DISCONNECT;
	}
}

static void sv_iot_on_msg(sm_mqtt_client_t* mqtt_client,sm_mqtt_msg_t* msg, void* arg){
	sv_iot_t* this = (sv_iot_t*)arg;
	for (int i = 0; i < this->sub_topic_num; i++){
		if (this->sub_topics[i].topic != NULL &&
			! strncmp(this->sub_topics[i].topic, msg->m_topic, msg->m_topic_length)){
			this->sub_topics[i].topic_handle(msg, arg);
		}
	}
}

static void sv_iot_on_config_msg(sm_mqtt_msg_t* msg, void* arg){
	  json_t mem[JSON_OBJ_MAX];
	  const json_t* json = json_create((char*)msg->m_payload, mem, ARRAY_SIZE(mem));

	  if (!json) return;


}

static void sv_iot_on_command_msg(sm_mqtt_msg_t* msg, void* arg){

}
