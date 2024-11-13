#ifndef SERVICES_SV_IOT_SV_IOT_MQTT_TOPIC_H_
#define SERVICES_SV_IOT_SV_IOT_MQTT_TOPIC_H_

#define SV_IOT_TOPIC_LENGHT_MAX			(64)

#define SV_IOT_SYNC_TOPIC_BASE			"petals/%s/sync"
#define SV_IOT_INFO_TOPIC_BASE			"petals/%s/info"
#define SV_IOT_CONFIG_TOPIC_BASE		"petals/%s/config"
#define SV_IOT_COMMAND_TOPIC_BASE		"petals/%s/command"

char* g_sync_topic = NULL;
char* g_info_topic = NULL;
char* g_cfg_topic = NULL;
char* g_cmd_topic = NULL;

void sv_iot_create_topic(char* device_sn){
	char buffer[SV_IOT_TOPIC_LENGHT_MAX];

	memset(buffer, 0 ,SV_IOT_TOPIC_LENGHT_MAX);
	sprintf(buffer, SV_IOT_SYNC_TOPIC_BASE, device_sn);
	g_sync_topic = malloc(strlen(buffer));
	memcpy(g_sync_topic, buffer, strlen(buffer));

	memset(buffer, 0 ,SV_IOT_TOPIC_LENGHT_MAX);
	sprintf(buffer, SV_IOT_INFO_TOPIC_BASE, device_sn);
	g_info_topic = malloc(strlen(buffer));
	memcpy(g_info_topic, buffer, strlen(buffer));

	memset(buffer, 0 ,SV_IOT_TOPIC_LENGHT_MAX);
	sprintf(buffer, SV_IOT_CONFIG_TOPIC_BASE, device_sn);
	g_cfg_topic = malloc(strlen(buffer));
	memcpy(g_cfg_topic, buffer, strlen(buffer));

	memset(buffer, 0 ,SV_IOT_TOPIC_LENGHT_MAX);
	sprintf(buffer, SV_IOT_COMMAND_TOPIC_BASE, device_sn);
	g_cmd_topic = malloc(strlen(buffer));
	memcpy(g_cmd_topic, buffer, strlen(buffer));
}


char* sv_iot_get_sync_topic(){
	return g_sync_topic;
}
char* sv_iot_get_info_topic(){
	return g_info_topic;
}
char* sv_iot_get_config_topic(){
	return g_cfg_topic;
}
char* sv_iot_get_command_topic(){
	return g_cmd_topic;
}

#endif /* SERVICES_SV_IOT_SV_IOT_MQTT_TOPIC_H_ */
