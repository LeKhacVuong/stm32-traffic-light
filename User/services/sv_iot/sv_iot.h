/*
 * sv_iot.h
 *
 *  Created on: Oct 14, 2024
 *      Author: Ng Thuy Quynh
 */

#ifndef SERVICES_SV_IOT_SV_IOT_H_
#define SERVICES_SV_IOT_SV_IOT_H_

/// EVENT
typedef enum {
	EVENT_CONNECTED,
	EVENT_DISCONNECTED,
	EVENT_COMMAND,
}SV_IOT_EVENT;

typedef enum {
	IOT_CMD_A = 0,
	IOT_CMD_B,
	IOT_CMD_NUM,
}SV_IOT_CMD;

/// EVENT CALLBACK
typedef struct {
	void (*on_conn)(void *arg);
	void (*on_disc)(void *arg);
	void (*on_cmd)(void *argv, void *arg);
}sv_iot_callback_t;

/// STATE
typedef enum {
	STATE_IDLE,
	STATE_CONNECTING,
	STATE_CONNECTED,
	STATE_DISCONNECTED,
}SV_IOT_STATE;



#endif /* SERVICES_SV_IOT_SV_IOT_H_ */
