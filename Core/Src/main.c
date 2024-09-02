#include "board.h"
#include "sm_logger.h"
#include "sm_host.h"
#include "stm32l1xx_hal_flash_ex.h"
#include "string.h"
#include "rc522.h"

#define TAG "main"

void log_put(const char* _log) {
    printf("%s\n", _log);
}

int64_t get_tick_count(){
	return HAL_GetTick();
}


typedef struct app_t{
	uint8_t m_rxBuffer;
}app_t;
app_t app;


int main(void)
{
	board_init();
	sm_logger_init(log_put, LOG_LEVEL_DEBUG);
	LOG_INF(TAG, "Start slave light app");

	HAL_UART_Receive_IT(&huart2, &app.m_rxBuffer, 1);

  while (1)
  {

  }
}

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart2);
	HAL_UART_Receive_IT(&huart2, &app.m_rxBuffer, 1);
}

void SysTick_Handler(void)
{
  HAL_IncTick();
}



