#include <string.h>
#include "sm_ec200_mqtt.h"

#include <sm_logger.h>

#include "sm_string_util.h"
/*
 * MQTT Related URCs
 * +QMTSTAT: <client_idx>,<err_code>
 *	Ex:  +QMTSTAT: 1,1 : Connection is closed or reset by a peer end.
 *	=> Execute AT+QMTOPEN command and reopen MQTT connection.
 * +QMTRECV: <client_idx>,<recv_id>
 *	Reported when the message received from MQTT server has been stored in buffer.
 * +QMTRECV: <client_idx>,<msgid>,<topic>[,<payload_len>],<payload>
 *
 * +QMTPING: <client_idx>,<result>
 *	It is reported when server receives no message from the client within 1.5 times the keep-alive time period
 *	and it will disconnect the client as if the client has sent a DISCONNECT message.
*/
#define SECOND	1000
#define MQTT_OPEN_TIMEOUT_MS				20*SECOND
#define MQTT_CLOSE_TIMEOUT_MS				5*SECOND
#define MQTT_CONNECTED_TIMEOUT_MS			5*SECOND
#define MQTT_DISCONNECTED_TIMEOUT_MS		5*SECOND
#define MQTT_SUBCRIBE_TIMEOUT_MS			5*SECOND
#define MQTT_UNSUBCRIBE_TIMEOUT_MS			5*SECOND
#define MQTT_PUBLISH_TIMEOUT_MS				5*SECOND

int32_t sm_ec200_config_mqtt(sm_ec200_t *modem){
	sm_modem_t *p_modem = (sm_modem_t*) modem;
	char buff[128] = { 0 };
	memset(buff, 0, 128);
	// sprintf(buff, "AT+QMTCFG=\"version\",1,3\r\n");
	// if (sm_modem_send_cmd(p_modem, buff, "OK\r\n","ERROR\r\n",1000) != 0) return -1;
	memset(buff, 0, 128);
	sprintf(buff, "AT+QMTCFG=\"keepalive\",1,0\r\n");
	if (sm_modem_send_cmd(p_modem, buff, "OK\r\n","ERROR\r\n",1000) != 0) {
        return -1;
    }
	// memset(buff, 0, 128);
	// sprintf(buff, "AT+QMTCFG=\"qmtping\",1,60\r\n");
	// if (sm_modem_send_cmd(p_modem, buff, "OK\r\n","ERROR\r\n",1000) != 0) return -1;
	memset(buff, 0, 128);
	sprintf(buff, "AT+QMTCFG=\"recv/mode\",1,1,0\r\n");
	if (sm_modem_send_cmd(p_modem, buff, "OK\r\n","ERROR\r\n",1000) != 0) {
        return -1;
    }

	memset(buff, 0, 128);
	sprintf(buff, "AT+QMTCFG=\"dataformat\",1,0,0\r\n");
	if (sm_modem_send_cmd(p_modem, buff, "OK\r\n","ERROR\r\n",1000) != 0) {
        return -1;
    }
	return 0;
}

int32_t sm_ec200_open_mqtt(sm_ec200_t *modem, const char *host, uint32_t port) {
	sm_modem_t *p_modem = (sm_modem_t*) modem;
	char buff[128] = { 0 };
	memset(buff, 0, 128);
	sprintf(buff, "AT+QMTOPEN=1,\"%s\",%d\r\n", host, (unsigned int) port);
	return sm_modem_send_cmd(p_modem,
                             buff,
                             "OK\r\n\r\n+QMTOPEN: 1,0\r\n",
                             "ERROR\r\n", MQTT_OPEN_TIMEOUT_MS);
}

int32_t sm_ec200_connect_mqtt(sm_ec200_t *modem, const char *client_id, const char *user, const char *pass) {
	sm_modem_t *p_modem = (sm_modem_t*) modem;

	char buff[256] = { 0 };
	memset(buff, 0, 256);

	if (user != NULL && pass != NULL) {
		sprintf(buff, "AT+QMTCONN=1,\"%s\",%s,%s\r\n", client_id, user, pass);
	} else {
		sprintf(buff, "AT+QMTCONN=1,\"%s\"\r\n", client_id);
	}
	return sm_modem_send_cmd(p_modem,
                             buff,
                             "OK\r\n\r\n+QMTCONN: 1,0,0\r\n",
                             "ERROR\r\n", MQTT_CONNECTED_TIMEOUT_MS);
}

int32_t sm_ec200_disconnect_mqtt(sm_ec200_t *modem) {
	sm_modem_t *p_modem = (sm_modem_t*) modem;
	return sm_modem_send_cmd(p_modem,
                             "AT+QMTDISC=1\r\n",
                             "OK\r\n\r\n+QMTDISC: 1,0\r\n",
                             "ERROR\r\n", MQTT_DISCONNECTED_TIMEOUT_MS);
}

int32_t sm_ec200_close_mqtt(sm_ec200_t *modem) {
	sm_modem_t *p_modem = (sm_modem_t*) modem;
	return sm_modem_send_cmd(p_modem,
                             "AT+QMTCLOSE=1\r\n",
                             "OK\r\n\r\n+QMTCLOSE: 1,0\r\n",
                             "ERROR\r\n", MQTT_CLOSE_TIMEOUT_MS);
}

int32_t sm_ec200_publish_mqtt(sm_ec200_t *modem,
                              const char *topic,
                              const char *data,
                              uint32_t len,
                              int qos,
                              int retain) {
	sm_modem_t *p_modem = (sm_modem_t*) modem;

	char buff[256] = { 0 };
	memset(buff, 0, 256);
	sprintf(buff, "AT+QMTPUBEX=1,1,%d,%d,\"%s\",%u\r\n", qos, retain, topic, len);

	if (sm_modem_send_cmd(p_modem, buff, ">", "ERROR\r\n", MQTT_PUBLISH_TIMEOUT_MS) != 0) {
		return -1;
	}
	return sm_modem_send_cmd(p_modem, data, "OK\r\n\r\n+QMTPUBEX: 1,1,0", "ERROR\r\n", MQTT_PUBLISH_TIMEOUT_MS);
}

int32_t sm_ec200_subscribes_mqtt(sm_ec200_t *modem, const char *topic, int qos) {
	sm_modem_t *p_modem = (sm_modem_t*) modem;
	char buff[512] = { 0 };
	memset(buff, 0, 512);
	sprintf(buff, "AT+QMTSUB=1,1,\"%s\",%d\r\n", topic, qos);
	return sm_modem_send_cmd(p_modem, buff, "OK\r\n\r\n+QMTSUB: 1,1,0", "ERROR\r\n", MQTT_SUBCRIBE_TIMEOUT_MS);
}

int32_t sm_ec200_unsubscribes_mqtt(sm_ec200_t *modem, const char *topic) {
	sm_modem_t *p_modem = (sm_modem_t*) modem;
	char buff[512] = { 0 };
	memset(buff, 0, 512);
	sprintf(buff, "AT+QMTUNS=1,1,\"%s\"\r\n", topic);
	return sm_modem_send_cmd(p_modem,
                             buff,
                             "OK\r\n\r\n+QMTUNS: 1,1,0",
                             "ERROR\r\n", MQTT_UNSUBCRIBE_TIMEOUT_MS);
}

int32_t sm_ec200_receive_mqtt_data(sm_ec200_t* modem){
	sm_modem_t* p_modem = (sm_modem_t*) modem;
	char buff[2048] = {0};
	memset(buff, 0, 2048);
	sprintf(buff, "AT+QMTRECV?\r\n");
	if (sm_modem_send_cmd(p_modem, buff, "OK\r\n","OK\r\n", 2000)) {
		// LOG_DBG("EC200_MQTT", "+QMTRECV? no response");
		return -1;
	}

    memset(buff, 0, 1024);

	if (shortest_substring(p_modem->buff, "+QMTRECV:","\r\n", buff, 2048) <= 0) {
		return -1;
	}else {
		int buff_index[5] = {0,0,0,0,0};
		int numscan = sscanf(buff,
                             "+QMTRECV: 1,%d,%d,%d,%d,%d\r\n",
                             &buff_index[0],
                             &buff_index[1],
                             &buff_index[2],
                             &buff_index[3],
                             &buff_index[4]);
		if (numscan < 5) {
			return -1;
		}
		// LOG_DBG("EC200_MQTT", "ec200 buffer: %d %d %d %d %d", buff_index[0], buff_index[1],buff_index[2],buff_index[3],buff_index[4]);
		for (int i = 0; i < 5; i++) {
			if (buff_index[i] == 1)
				return (int32_t)i;
		}
	}

	return -1;
}

static void parse_mqtt(char * source, char *topic, char *payload){
	char *p_topic,*p_payload;
	p_topic = strstr(source,",\"");
	p_topic+= 2;
	char* p_topic_end = strstr(p_topic,"\",\"");
	*p_topic_end = 0;
	memcpy(topic,p_topic,p_topic_end-p_topic+1);

	p_payload = p_topic_end+3;
	char *p_payload_end = strstr(p_payload,"\"\r\n\r\nOK\r\n");
	*p_payload_end = 0;
	memcpy(payload,p_payload,p_payload_end-p_payload+1);
}

int32_t sm_ec200_data_incoming_mqtt(sm_ec200_t *modem, char *topic, char *data) {
	int buff_index = sm_ec200_receive_mqtt_data(modem);
	// LOG_DBG("EC200_MQTT", "buff_index: %d", buff_index);
	if(buff_index == -1){
		return - 1;
	}
	sm_modem_t* p_modem = (sm_modem_t*) modem;
	char buff[128];
	memset(buff, '\0', 128);
	sprintf(buff, "AT+QMTRECV=1,%d\r\n",buff_index);
	// LOG_DBG("EC200_MQTT", "CMD: %s", buff);
	if(sm_modem_send_cmd(p_modem, buff, "OK\r\n","ERROR\r\n", 5000)) {
		// LOG_DBG("EC200_MQTT", "Cant read mqtt data ");
		return -1;
	}
	parse_mqtt(p_modem->buff, topic, data);
	// LOG_DBG("EC200_MQTT", "topic: %s\npayload: %s", topic, data);
	return 0;
}

URC_MSG sm_ec200_read_urc_mqtt(sm_ec200_t *modem) {
	sm_modem_t* p_modem = (sm_modem_t*) modem;
	char buff[512], urc[16];
	memset(buff, '\0', 512);
	memset(urc, '\0', 16);
	int32_t len = -1;
	len = sm_modem_read(p_modem, buff, 512);
	if (len > 0) {
		LOG_DBG("EC200_MQTT", "Modem read : %s - len: %d",buff, len);
		if (strstr(buff, "+QMTSTAT: 1,1") != NULL) {
			LOG_DBG("EC200_MQTT", "URC mqtt disconnected from broker!");
			return URC_QMTSTAT;
		}
	}
    return URC_NULL;
}

