/*
 * sv_lora.h
 *
 *  Created on: Oct 8, 2024
 *      Author: lekhacvuong
 */

#ifndef SERVICES_SV_LORA_SV_LORA_H_
#define SERVICES_SV_LORA_SV_LORA_H_

#include "stdint.h"

int sv_lora_init();

int sv_lora_send_bytes(uint8_t* _data, uint16_t _len, int32_t _timeout);

int sv_lora_get_bytes(uint8_t* _buff, uint8_t _len, uint32_t _timeout);

void sv_lora_polling_bytes();

#endif /* SERVICES_SV_LORA_SV_LORA_H_ */
