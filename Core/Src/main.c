#include "main.h"
#include "stdio.h"
#include "board.h"
#include "sm_logger.h"
#include "sm_mb_slave_impl.h"
#include "ligh_data.h"
#include "string.h"


#define TAG "main"

#define MB_SLAVE_ID 1
sm_mb_slave_t* g_mb_slave;


uint8_t rx_buff;
data_queue_t mb_queue;

void log_put(const char* _log) {
    printf("%s\n", _log);
}

int32_t mb_slv_send_if(uint8_t _addr, const uint8_t* _data, uint16_t _len, int32_t _timeout, void* _arg){
    return 	HAL_UART_Transmit(&huart2, _data, _len, _timeout);
}
int32_t mb_slv_rcv_if(uint8_t _addr, uint8_t *_buf, uint16_t _max_len, int32_t _timeout, void *_arg) {
    return queue_get_bytes(&mb_queue, _buf, _max_len, _timeout);
}

light_slv_info_t g_slave_info[3];

MODBUS_ERROR_CODE modbus_cb_write_single_register_cb(uint16_t _address,
                                                     uint16_t _value,
                                                     uint8_t _unit_id,
                                                     void* _arg){
//    LOG_DBG(TAG, "Master write single reg %d, value 0x%x", _address, _value);
    if(_address == MODBUS_HOLDING_REG_CONTROL_LIGHT_INDEX){
        uint16_t id = _value>>8;
        uint16_t color = _value&0xFF;
        LOG_INF(TAG, "Slave %d change light color to %s", id, getLightColorString(color));
        g_slave_info[id].m_current_light = color;
    }

    return MODBUS_ERROR_NONE;
}
MODBUS_ERROR_CODE modbus_cb_write_multiple_registers_cb(uint16_t _address,
                                                        uint16_t _quantity,
                                                        const uint16_t* _registers,
                                                        uint8_t _unit_id,
                                                        void* _arg){
    LOG_DBG(TAG, "Master write multi reg %d quantity %d", _address, _quantity);

    return MODBUS_ERROR_NONE;
}

MODBUS_ERROR_CODE modbus_cb_read_input_registers(uint16_t _address,
                                                                uint16_t _quantity,
                                                                uint16_t* _registers_out,
                                                                uint8_t _unit_id,
                                                                void* _arg){
    LOG_DBG(TAG, "Master read input reg %d quantity %d", _address, _quantity);
    if(_quantity != MODBUS_INPUT_REG_SYNC_DATA_REG_NUM)
        return MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE;
//
    uint16_t testID = _address; //scrip

//
//    if(testID >= 3)
//        return MODBUS_ERROR_INVALID_UNIT_ID;
//
    LOG_INF(TAG, "Sync slave %d", testID);
//    if(testID == 2){
//        count = !count;
//        if(count){
//            memcpy((uint8_t*)_registers_out, &g_slave_info[testID], sizeof(light_slv_info_t));
//        }
//    }else{
//        memcpy((uint8_t*)_registers_out, &g_slave_info[testID], sizeof(light_slv_info_t));
//    }

    memcpy((uint8_t*)_registers_out, &g_slave_info[testID], sizeof(light_slv_info_t));


    return MODBUS_ERROR_NONE;
}

MODBUS_ERROR_CODE modbus_cb_read_holding_registers(uint16_t _address,
                                                   uint16_t _quantity,
                                                   uint16_t *_registers_out,
                                                   uint8_t _unit_id,
                                                   void *_arg) {
    LOG_DBG(TAG, "Master read holding reg %d quantity %d", _address, _quantity);

    return MODBUS_ERROR_NONE;
}

static const sm_mb_slave_cb_t g_mb_slave_callback = {
        .read_input_registers = modbus_cb_read_input_registers,
        .read_holding_registers = modbus_cb_read_holding_registers,
        .write_single_register = modbus_cb_write_single_register_cb,
        .write_multiple_registers = modbus_cb_write_multiple_registers_cb,
};


int main(void){
    sm_logger_init(log_put, LOG_LEVEL_DEBUG);
	board_init();
	HAL_UART_Receive_IT(&huart2, &rx_buff, 1);

    g_mb_slave = sm_mb_slave_create_default(MB_SLAVE_ID, mb_slv_send_if, mb_slv_rcv_if, NULL);
    g_mb_slave = sm_mb_slave_create_default(MB_SLAVE_ID, mb_slv_send_if, mb_slv_rcv_if, NULL);
    if(!g_mb_slave){
        LOG_ERR(TAG, "Create modbus server FAILED");
        return -1;
    }

    g_mb_slave->m_proc->init(g_mb_slave);
    g_mb_slave->m_proc->reg_function_cb(g_mb_slave, &g_mb_slave_callback);


	while (1)
	{
		g_mb_slave->m_proc->polling(g_mb_slave);
	}
}


void SysTick_Handler(void)
{
	if(uwTick % 500 == 0){
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	}

	HAL_IncTick();

}


void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart2);
	queue_push(&mb_queue, rx_buff);
	HAL_UART_Receive_IT(&huart2, &rx_buff, 1);
}

int64_t get_tick_count(){
	return HAL_GetTick();
}

