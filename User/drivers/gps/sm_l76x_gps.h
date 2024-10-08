//
// Created by vnbk on 09/06/2023.
//

#ifndef SM_L76X_GPS_H
#define SM_L76X_GPS_H

#include "sm_datetime.h"
#include "sm_hal.h"
#include "stdint.h"
#include "stdio.h"
#include "sm_modem.h"
#include "sm_elapsed_timer.h"

#define GPS_BUFFER_LENGTH 512
typedef struct {
    sm_modem_t      m_base;
    float           m_lat;
    float           m_lon;
    sys_datetime_t  m_datetime;
    char            *buff;
    uint8_t         data_is_valid;
    elapsed_timer_t m_timeout;
}sm_l76x_t;

sm_l76x_t* sm_l76x_init(sm_hal_uart_t *_if, sm_hal_io_t *_rst);

static inline int32_t sm_l76x_deinit(sm_l76x_t* _this) {
    _this = NULL;
    return 0;
}

int32_t sm_l76x_reboot(sm_l76x_t* _this);

static inline float sm_l76x_get_lat(sm_l76x_t* _this) {
    if (!_this) {
        return -1;
    } else {
        return _this->m_lat;
    }
}

static inline float sm_l76x_get_lon(sm_l76x_t* _this) {
    if (!_this) {
        return -1;
    } else {
        return _this->m_lon;
    }
}

static inline int32_t sm_l76x_get_time(sm_l76x_t* _this, sys_datetime_t* _datetime) {
    if (!_this) {
        return -1;
    } else {
        _datetime->year = _this->m_datetime.year;
        _datetime->mon = _this->m_datetime.mon;
        _datetime->day = _this->m_datetime.day;
        _datetime->hour = _this->m_datetime.hour;
        _datetime->min = _this->m_datetime.min;
        _datetime->sec = _this->m_datetime.sec;
        return 0;
    }
}

int32_t sm_l76x_process(sm_l76x_t*);

#endif
