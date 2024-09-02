#include "sm_logger.h"
#include "linux_serial.h"
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include "sm_mb_slave_impl.h"
#include "ligh_data.h"
#include "sm_elapsed_timer.h"
#include "sm_time_utils.h"

#define TAG "main"

#define MB_SLAVE_ID 1

sm_mb_slave_t* g_mb_slave;

#define USB_PORT "/dev/ttyUSB1"
int g_fd;

int64_t get_tick_count() {
    struct timespec ts;
    int32_t tick = 0U;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    tick = ts.tv_nsec / 1000000;
    tick += ts.tv_sec * 1000;
    return tick;
}

void getTimeString(char *buffer, size_t bufferSize) {
    struct timeval tv;
    struct tm *timeinfo;
    char timeStr[9];
    gettimeofday(&tv, NULL);
    timeinfo = localtime(&tv.tv_sec);
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", timeinfo);
    snprintf(buffer, bufferSize, "%s:%03ld", timeStr, tv.tv_usec / 1000);
}


void log_put(const char* _log) {
    char timeString[13];
    getTimeString(timeString, sizeof(timeString));

    printf("%s: %s\n",timeString, _log);
}

int32_t mb_slv_send_if(uint8_t _addr, const uint8_t* _data, uint16_t _len, int32_t _timeout, void* _arg){
    return 	serial_send_bytes(g_fd, _data, _len);
}

int32_t mb_slv_rcv_if(uint8_t _addr, uint8_t *_buf, uint16_t _max_len, int32_t _timeout, void *_arg) {
    elapsed_timer_t timeout;
    elapsed_timer_resetz(&timeout, _timeout);

    do {
        if(serial_data_available(g_fd) >= _max_len){
            return serial_recv_bytes(g_fd, _buf, _max_len);
        }
    }
    while(elapsed_timer_get_remain(&timeout));

    return 0;
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
    static int count = 0;
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


void at_command_process(){
    if(serial_data_available(g_fd) > 0){
        delayMs(5);
        uint16_t available_len = serial_data_available(g_fd);
        if(available_len >= 64){
            serial_flush(g_fd);
            return;
        }

        uint8_t rx_data[64] =  {0,};
        uint16_t data_len = serial_recv_bytes(g_fd, rx_data, available_len);
        LOG_INF(TAG, "Recv %d / %d byte data is %s", data_len, available_len, rx_data);

        for(int i = 0; i < data_len; i++){
            LOG_INF(TAG, "Byte %d is %d", i, rx_data[i]);
        }

        if(strcmp((char*)rx_data, "AT\n") == 0){
            LOG_INF(TAG, "AT command check");
            serial_send_bytes(g_fd, "OK", sizeof("OK"));
        }
        else if(strcmp((char*)rx_data, "AT+ROLE\n") == 0){
            LOG_INF(TAG, "AT command check");
            serial_send_bytes(g_fd, "ROLE:1", sizeof("ROLE:1"));
        }
        else{
            LOG_INF(TAG, "Invalid AT command");
            serial_send_bytes(g_fd, "ERR", sizeof("ERR"));
        }
        serial_flush(g_fd);
    }
}

int main() {
    sm_logger_init(log_put, LOG_LEVEL_DEBUG);

    g_fd = serial_init(USB_PORT, 115200, SERIAL_FLAG_BLOCKING);
    if (g_fd < 0) {
        LOG_INF(TAG, "cannot open serial port\n");
        return -1;
    }


    g_mb_slave = sm_mb_slave_create_default(MB_SLAVE_ID, mb_slv_send_if, mb_slv_rcv_if, NULL);
    if(!g_mb_slave){
        LOG_ERR(TAG, "Create modbus server FAILED");
        return -1;
    }

    g_mb_slave->m_proc->init(g_mb_slave);
    g_mb_slave->m_proc->reg_function_cb(g_mb_slave, &g_mb_slave_callback);


    while(1){
//        g_mb_slave->m_proc->polling(g_mb_slave);
        at_command_process();
    }
}
