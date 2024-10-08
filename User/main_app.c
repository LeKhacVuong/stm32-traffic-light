/*
 * main_app.c
 *
 *  Created on: Oct 3, 2024
 *      Author: lekhacvuong
 */

#include "main_app.h"
#include "string.h"
#include "sm_elapsed_timer.h"
#include "sv_lora.h"
#include "sm_logger.h"
#include "light_data.h"
#include "sm_mb_slave_impl.h"

#define TAG "main"

sm_mb_slave_t* g_mb_slave;

int64_t get_tick_count(){
	return HAL_GetTick();
}

void log_put(const char* _log){
	HAL_UART_Transmit(&huart1, (uint8_t*)_log, strlen(_log), 100);
}

int32_t mb_slv_send_if(uint8_t _addr, const uint8_t* _data, uint16_t _len, int32_t _timeout, void* _arg){
    return 	sv_lora_send_bytes((uint8_t*)_data, _len, _timeout);
}

int32_t mb_slv_rcv_if(uint8_t _addr, uint8_t *_buf, uint16_t _max_len, int32_t _timeout, void *_arg) {
    return sv_lora_get_bytes(_buf, _max_len, _timeout);
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

    uint16_t testID = _address; //scrip
    LOG_INF(TAG, "Sync slave %d", testID);

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

void main_app(){

	sv_lora_init();

    g_mb_slave = sm_mb_slave_create_default(1, mb_slv_send_if, mb_slv_rcv_if, NULL);
    if(!g_mb_slave){
        LOG_ERR(TAG, "Create modbus server FAILED");
        return;
    }

    g_mb_slave->m_proc->init(g_mb_slave);
    g_mb_slave->m_proc->reg_function_cb(g_mb_slave, &g_mb_slave_callback);

	while(1){
        g_mb_slave->m_proc->polling(g_mb_slave);

		sv_lora_polling_bytes();

	}
}
