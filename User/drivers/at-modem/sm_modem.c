/*
 * sm_modem.c
 *
 *  Created on: Jul 5, 2023
 *      Author: Admin
 */
#include <string.h>
#include <assert.h>

#include "sm_modem.h"
#include "sm_hal_delay.h"
#include "sm_logger.h"


static const char* TAG = "MODEM";

char buff_modem[MODEM_BUFF_SIZE] = {0};

void sm_modem_init(sm_modem_t *modem,sm_hal_io_t *io, sm_hal_uart_t *driver){
    modem->reset_pin = io;
    modem->driver = driver;
    if(modem->buff_length == 0 && modem->buff == NULL){
        modem->buff_length = MODEM_BUFF_SIZE;
//        modem->buff = calloc(modem->buff_length,sizeof(char));
        modem->buff = buff_modem;
    }
    else if(modem->buff_length != 0 && modem->buff == NULL){
//        modem->buff = calloc(modem->buff_length,sizeof(char));
    	modem->buff = buff_modem;
    }
    modem->lock = 0;
}

void sm_modem_deinit(sm_modem_t *modem){
    modem->reset_pin = NULL;
    modem->driver = NULL;
}

int32_t sm_modem_send_cmd(sm_modem_t *modem,char* cmd,char* res_ok, char* res_fail,uint32_t timeout){
	while (modem->lock) {
		sm_hal_delay_ms(1);
	}
	modem->lock = 1;
//	LOG_DBG(TAG, "cmd: %s ", cmd);
    sm_hal_uart_write(modem->driver,(uint8_t*)cmd,strlen(cmd));
    memset(modem->buff, 0, modem->buff_length);
    uint32_t time = 0;  
    uint32_t index_buff = 0;
    int32_t res = 0;
    while(1){
		size_t len = sm_hal_uart_read(modem->driver,
				(uint8_t*) modem->buff + index_buff,256);
        if(len > 0){
			index_buff += (uint32_t) len;
			if (strstr(modem->buff, res_ok) != NULL) {
				res = 0;
				break;
			}
			if (strstr(modem->buff, res_fail) != NULL) {
				res = -2;
				break;
			}
        }
        time++;
        if(time > timeout) {
        	LOG_ERR(TAG, "timeout");
			res = -1;
            break;
        }
        sm_hal_delay_ms(1);
    }
//	LOG_DBG(TAG, "res: %s len: %d time: %d",modem->buff, strlen(modem->buff), time);
	modem->lock = 0;
    return res;
}

int32_t sm_modem_send_data(sm_modem_t *modem, char* data, int32_t len){
	while (modem->lock) {
		sm_hal_delay_ms(1);
	}
	modem->lock = 1;

	int32_t rc = sm_hal_uart_write(modem->driver, (uint8_t*) data,
			(uint32_t) len);
	modem->lock = 0;
	return rc;
}

void sm_modem_hw_reset(sm_modem_t *modem, uint32_t duration_ms, int logic) {
	if (logic) {
        sm_hal_io_set_value(modem->reset_pin, 0);
		sm_hal_delay_ms(duration_ms);
        sm_hal_io_set_value(modem->reset_pin, 1);
	}
	else {
        sm_hal_io_set_value(modem->reset_pin, 1);
		sm_hal_delay_ms(duration_ms);
        sm_hal_io_set_value(modem->reset_pin, 0);
	}
}

int32_t sm_modem_read(sm_modem_t *modem,char* data,uint32_t len){
	while (modem->lock) {
		sm_hal_delay_ms(1);
	}
	modem->lock = 1;
	size_t length = sm_hal_uart_read(modem->driver, (uint8_t*) data, len);
	modem->lock = 0;
	return length;
}

int32_t sm_modem_read_until_char(sm_modem_t *modem,char* _buff, char _ch, uint32_t timeout) {
	while (modem->lock) {
		sm_hal_delay_ms(1);
	}
	modem->lock = 1;
	uint32_t err = 0;
	uint32_t index_buff = 0;
	int32_t ret = 0;
	uint32_t time = 0;
	memset(modem->buff, 0, MODEM_BUFF_SIZE);
	while (1) {
		ret = sm_hal_uart_read(modem->driver, (uint8_t*) modem->buff + index_buff, 1);
		if (ret == 1) {
			if (*(modem->buff + index_buff) == _ch) {
				memcpy(_buff, modem->buff, index_buff+1);
				modem->lock = 0;
				return index_buff+1;
			}
	        index_buff++;
		}
		time++;
		if(time > timeout) {
			LOG_ERR(TAG, "modem read until char timeout");
			err = -1;
			break;
		}
		sm_hal_delay_ms(1);
	}
	modem->lock = 0;
	return err;
}

int32_t sm_modem_read_until_str(sm_modem_t *modem,char* _buff, char* _str, uint32_t timeout) {
    while (modem->lock) {
        sm_hal_delay_ms(1);
    }
    modem->lock = 1;
    uint32_t err = 0;
    uint32_t index_buff = 0;
    int32_t ret = 0;
    uint32_t time = 0;
    memset(modem->buff, 0, MODEM_BUFF_SIZE);
    while (1) {
        ret = sm_hal_uart_read(modem->driver, (uint8_t*) modem->buff + index_buff, 1);
        if (ret == 1) {
            if (strstr(modem->buff, _str)) {
                memcpy(_buff, modem->buff, index_buff+1);
                modem->lock = 0;
                return index_buff+1;
            }
            index_buff++;
        }
        time++;
        if(time > timeout) {
            LOG_ERR(TAG, "modem read until string timeout");
            err = -1;
            break;
        }
        sm_hal_delay_ms(1);
    }
    modem->lock = 0;
    return err;
}

int32_t sm_modem_peek(sm_modem_t *modem,char* data, uint32_t len){
	while (modem->lock) {
		sm_hal_delay_ms(1);
	}
	modem->lock = 1;
	// size_t length = sm_hal_uart_peek(modem->driver, (uint8_t*) data, len);
	modem->lock = 0;
	return len;
}
