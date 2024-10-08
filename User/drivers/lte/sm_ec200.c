#include "sm_ec200.h"
#include <sm_logger.h>
#include "sm_hal_delay.h"
#include <string.h>
#include <stdio.h>

static const char* TAG = "SM_EC200";
#define RESET_MODEM_TIME    15*1000
void sm_ec200_init(sm_ec200_t *modem, sm_hal_io_t *reset_pin, sm_hal_uart_t *driver) {
    sm_modem_init((sm_modem_t*) modem, reset_pin, driver);
    EC200_INIT: sm_modem_hw_reset((sm_modem_t*) modem, 400, 0);
	LOG_DBG(TAG, "Reset modem");
    sm_hal_delay_ms(RESET_MODEM_TIME);

    uint32_t retain = 0;

    if (sm_ec200_sync(modem) != 0) {
    	LOG_DBG(TAG, "sync fail");
        retain++;
        if (retain > 3)
            goto EC200_INIT;
    }
    if (sm_ec200_get_pin(modem) != 0) {
    	LOG_DBG(TAG, "get pin fail");
        retain++;
        if (retain > 3)
            goto EC200_INIT;
    }
    if (sm_ec200_get_function(modem) != 0) {
    	LOG_DBG(TAG, "get function fail");
        retain++;
        if (retain > 3)
            goto EC200_INIT;
    }
    if (sm_ec200_get_signal(modem) != 0) {
    	LOG_DBG(TAG, "get signal fail");
        retain++;
        if (retain > 3)
            goto EC200_INIT;
    }
    if (sm_ec200_get_simNb(modem) != 0) {
    	LOG_DBG(TAG, "get simNb fail");
        retain++;
        if (retain > 3)
            goto EC200_INIT;
    }
    if (sm_ec200_get_network_reg(modem) != 0) {
           LOG_DBG(TAG, "get Network fail");
           retain++;
           if (retain > 3)
               goto EC200_INIT;
       }
	if (retain) goto EC200_INIT;
    sm_ec200_echo_mode(modem, 0);
	LOG_DBG(TAG, "echo mode off");
}

void sm_ec200_deinit(sm_ec200_t *modem) {
	sm_modem_deinit((sm_modem_t*) modem);
}

int32_t sm_ec200_sync(sm_ec200_t *modem) {
	return sm_modem_send_cmd((sm_modem_t*) modem,
                             "AT\r\n",
                             "OK\r\n",
                             "ERROR\r\n", 1000);
}

int32_t sm_ec200_echo_mode(sm_ec200_t *modem, uint8_t mode) {
	if (mode == 0)
		return sm_modem_send_cmd((sm_modem_t*) modem,
                                 "ATE0\r\n",
                                 "OK\r\n",
                                 "ERROR\r\n", 1000);
	return sm_modem_send_cmd((sm_modem_t*) modem,
                             "ATE1\r\n",
                             "OK\r\n",
                             "ERROR\r\n", 1000);
} // mode = 0 : disable echo mode, mode = 1 : enable echo mode

int32_t sm_ec200_set_pin(sm_ec200_t *modem, char *pin) {
	sm_modem_t *p_modem = (sm_modem_t*) modem;
	char buff[128];
	memset(buff, 0, p_modem->buff_length);

	sprintf(buff, "AT+CPIN=%s\r\n", pin);

	return sm_modem_send_cmd((sm_modem_t*) modem,
                             buff,
                             "OK\r\n",
                             "ERROR\r\n",
                             1000);
}

int32_t sm_ec200_get_pin(sm_ec200_t *modem) {
	return sm_modem_send_cmd((sm_modem_t*) modem,
                             "AT+CPIN?\r\n",
                             "OK\r\n",
                             "ERROR\r\n",
                             1000);
}

int32_t sm_ec200_get_signal(sm_ec200_t *modem) {
	int32_t rc = sm_modem_send_cmd((sm_modem_t*) modem,
                                   "AT+CSQ\r\n",
                                   "OK\r\n",
                                   "ERROR\r\n", 1000);

	if (rc == 0) {
		sscanf(modem->base.buff,
               "AT+CSQ\r\r\n+CSQ: %d,%d\r\n\r\nOK\r\n",
               &modem->rssi,
               &modem->ber);
	}
	return rc;
}

int32_t sm_ec200_get_name(sm_ec200_t *modem, char *name) {
	int32_t rc = sm_modem_send_cmd((sm_modem_t*) modem, "AT+GMM\r\n", "OK\r\n",
			"ERROR\r\n", 1000);
	if (rc == 0) {
		memcpy(modem->base.buff + 2, name, 4);
	}
	return rc;
}

int32_t sm_ec200_get_function(sm_ec200_t *modem) {
	return sm_modem_send_cmd((sm_modem_t*) modem,
                             "AT+CFUN?\r\n",
                             "OK\r\n",
                             "ERROR\r\n", 1000);
}

int32_t sm_ec200_set_apn(sm_ec200_t *modem, char *apn, char *user, char *pass) {
	char buff[256] = { 0 };
	sprintf(buff, "AT+QICSGP=1,1,\"%s\",\"%s\",\"%s\",1\r\n", apn, user, pass);
	return sm_modem_send_cmd((sm_modem_t*) modem,
                             buff, "OK\r\n",
                             "ERROR\r\n",
                             1000);
}

int32_t sm_ec200_get_network_reg(sm_ec200_t *modem) {
	return sm_modem_send_cmd((sm_modem_t*) modem,
                             "AT+CREG?\r\n",
                             "OK\r\n",
                             "ERROR\r\n", 1000);
}

int32_t sm_ec200_get_simNb(sm_ec200_t *modem) {
	int32_t rc = sm_modem_send_cmd((sm_modem_t*) modem,
                                   "AT+CIMI\r\n",
                                   "OK\r\n",
                                   "ERROR\r\n", 1000);
	if (rc == 0) {
		sscanf(modem->base.buff, "AT+CIMI\r\r\n%s\r\n\r\nOK\r\n",
				(char*) modem->IMSI);
		strncpy(modem->simNb, modem->IMSI + 7, 8);
	}
	return rc;
}

int32_t sm_ec200_get_ip(sm_ec200_t *modem) {
	if (sm_modem_send_cmd(&modem->base, "AT+QIACT=1\r\n", "OK\r\n", "ERROR\r\n", 10000) == 0) {
		if (sm_modem_send_cmd(&modem->base,
                              "AT+QIACT?\r\n",
                              "OK\r\n",
                              "ERROR\r\n",
                              10000) == 0) {
			char *str = strstr(modem->base.buff, "+QIACT");
			sscanf(str,
                   "+QIACT: 1,1,1,\"%d.%d.%d.%d\"",
                   &modem->ip[0],
					&modem->ip[1],
                    &modem->ip[2],
                    &modem->ip[3]);
			sm_modem_send_cmd(&modem->base,
                              "AT+QIDEACT=1\r\n",
                              "OK\r\n",
                              "ERROR\r\n", 10000);
			return 0;
		}
	}
	return -1;
}

int32_t sm_ec200_get_time_network(sm_ec200_t* modem, struct tm *p_time) {
	char buff[128];
	memset (buff, 0, 128);
	sprintf (buff, "AT+QLTS=2\r\n");

	if (sm_modem_send_cmd ((sm_modem_t*) modem, buff, "OK", "ERROR", 1000) != 0){
		return -1;
	}
	char *str_time = strstr (modem->base.buff, "\"");
	if (str_time == NULL){
		return -1;
	}
	int year = 0;
	uint8_t num_scan = 0 ;
	num_scan = sscanf (str_time, "\"%d/%d/%d,%d:%d:%d+", &year, &p_time->tm_mon, &p_time->tm_mday, &p_time->tm_hour,
			&p_time->tm_min, &p_time->tm_sec);
	p_time->tm_year = year - 2000;
	if (num_scan < 6) {
		return -1;
	}
	return 0;
}

int32_t sm_ec200_hard_reset(sm_ec200_t *modem) {
	sm_modem_hw_reset((sm_modem_t*) modem, 200, 1);
	return 0;
}
