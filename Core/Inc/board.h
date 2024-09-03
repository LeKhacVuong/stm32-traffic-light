/*
 * board.h
 *
 *  Created on: Aug 31, 2024
 *      Author: lekhacvuong
 */

#ifndef INC_BOARD_H_
#define INC_BOARD_H_

#include "stm32l1xx_hal.h"

void Error_Handler(void);
#define LED_Pin GPIO_PIN_7
#define LED_GPIO_Port GPIOB

extern UART_HandleTypeDef huart2;

#define QUEUE_SIZE 128

typedef struct{
	uint8_t m_queue[QUEUE_SIZE];
	uint16_t m_head;
	uint16_t m_tail;
}data_queue_t;

int queue_push(data_queue_t* _queue, uint8_t _data);

int queue_get_byte_available(data_queue_t* _queue);

int queue_get_bytes(data_queue_t* _queue, uint8_t* _buff, uint8_t _len, uint32_t _timeout);

int queue_reset(data_queue_t* _queue);

void board_init();

#endif /* INC_BOARD_H_ */
