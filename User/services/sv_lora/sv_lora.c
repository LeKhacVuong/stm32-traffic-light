/*
 * sv_lora.c
 *
 *  Created on: Oct 8, 2024
 *      Author: lekhacvuong
 */

#include "sv_lora.h"
#include "LoRa.h"
#include "main.h"
#include "sm_elapsed_timer.h"

#define BUFF_SIZE 512

LoRa g_lora;
uint8_t g_queue_buff[BUFF_SIZE];
uint32_t g_head = 0, g_tail = 0;

static int sv_lora_get_bytes_availabe(){
	return (g_head >= g_tail) ? g_head - g_tail : BUFF_SIZE - g_tail + g_head;
}

int sv_lora_send_bytes(uint8_t* _data, uint16_t _len, int32_t _timeout){
	if(LoRa_transmit(&g_lora, _data, _len, _timeout) > 0){
		return -1;
	}
	return _len;
}

int sv_lora_get_bytes(uint8_t* _buff, uint8_t _len, uint32_t _timeout){

	elapsed_timer_t timeout;
	elapsed_timer_resetz(&timeout, _timeout);

	do{
		if(sv_lora_get_bytes_availabe() >= _len){
			for(int i = 0; i < _len; i++){
				_buff[i] = g_queue_buff[g_tail];
				g_tail++;
				if(g_tail >= BUFF_SIZE){
					g_tail = 0;
				}
			}
			return _len;
		}
		sv_lora_polling_bytes();
	}
	while(elapsed_timer_get_remain(&timeout));
	return 0;
}

int sv_lora_init(){
	g_lora = newLoRa();

	g_lora.hSPIx                 = &hspi1;
	g_lora.CS_port               = SPI1_SS_GPIO_Port;
	g_lora.CS_pin                = SPI1_SS_Pin;
	g_lora.reset_port            = LORA_RST_GPIO_Port;
	g_lora.reset_pin             = LORA_RST_Pin;
	g_lora.frequency             = 433;						// default = 433 MHz
	g_lora.spredingFactor        = SF_7;					// default = SF_7
	g_lora.bandWidth			 = BW_125KHz;				// default = BW_125KHz
	g_lora.crcRate				 = CR_4_5;					// default = CR_4_5
	g_lora.power				 = POWER_20db;				// default = 20db
	g_lora.overCurrentProtection = 120; 					// default = 100 mA
	g_lora.preamble				 = 10;		  				// default = 8;

	LoRa_reset(&g_lora);
	LoRa_init(&g_lora);

	LoRa_startReceiving(&g_lora);

	return 0;
}

void sv_lora_polling_bytes(){
	uint8_t read_buff[128];

	int byte = LoRa_receive(&g_lora, read_buff, 128);
	for(int i = 0; i < byte; i++){
		g_queue_buff[g_head] = read_buff[i];
		g_head++;
		if(g_head >= BUFF_SIZE){
			g_tail = 0;
		}
	}
}




