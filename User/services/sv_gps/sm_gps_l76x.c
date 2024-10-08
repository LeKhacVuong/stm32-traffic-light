//
// Created by admin on 9/23/2024.
//
#include <time.h>
#include "sm_gps_l76x.h"
#include "sm_datetime.h"
#include "sm_l76x_gps.h"
#include "sm_logger.h"

typedef struct {
    sm_gps_t m_base;
    sm_l76x_t* g_gps_l76x;
}sm_gps_l76x_t;

static int32_t sm_gps_module_init(sm_gps_t* _this);
static int32_t sm_gps_module_free(sm_gps_t* _this);
static int32_t sm_gps_module_reboot(sm_gps_t* _this);
static int32_t sm_gps_module_pw_reset(sm_gps_t* _this);
static float sm_gps_module_get_lat(sm_gps_t*  _this);
static float sm_gps_module_get_lon(sm_gps_t* _this);
static int32_t sm_gps_module_get_time(sm_gps_t* _this, sm_gps_time_t* _time);
static int32_t sm_gps_module_get_coordinate(sm_gps_t* _this, sm_gps_coordinate_t* _coordinate);
static int32_t sm_gps_module_process(sm_gps_t* _this);

sm_gps_module_proc_t g_gps_module_proc = {
    .init = sm_gps_module_init,
    .free = sm_gps_module_free,
    .reboot = sm_gps_module_reboot,
    .power_reset = sm_gps_module_pw_reset,
    .get_lat = sm_gps_module_get_lat,
    .get_lon = sm_gps_module_get_lon,
    .get_time = sm_gps_module_get_time,
    .get_coordinate = sm_gps_module_get_coordinate,
    .process = sm_gps_module_process
};

static sm_gps_l76x_t g_gps_l76x = {
        .m_base.proc = &g_gps_module_proc,
        .g_gps_l76x = NULL
};

sys_datetime_t *utc_offset(sys_datetime_t* _time, int8_t _offset) {
    struct tm tm_time = {0};
    tm_time.tm_year = _time->year + (START_YEAR - 1900);
    tm_time.tm_mon = _time->mon - 1;
    tm_time.tm_mday = _time->day;
    tm_time.tm_hour = _time->hour;
    tm_time.tm_min = _time->min;
    tm_time.tm_sec = _time->sec;

    time_t raw_time;
    raw_time = mktime(&tm_time);
    raw_time += _offset * 3600;

    struct tm* ret_time = gmtime(&raw_time);

    static sys_datetime_t ret_sys_time;

    ret_sys_time.year = ret_time->tm_year - (START_YEAR - 1900);
    ret_sys_time.mon = ret_time->tm_mon + 1;
    ret_sys_time.day = ret_time->tm_mday;
    ret_sys_time.hour = ret_time->tm_hour;
    ret_sys_time.min = ret_time->tm_min;
    ret_sys_time.sec = ret_time->tm_sec;

    return &ret_sys_time;
}

static int32_t sm_gps_module_init(sm_gps_t* _this) {
    if (!_this) {
        return -1;
    }
//    g_gps_l76x.g_gps_l76x = sm_l76x_init(g_gps_uart, g_gps_rst);
    return 0;
}
static int32_t sm_gps_module_free(sm_gps_t* _this){
    if (!_this) {
        return -1;
    }
    return 0;
}
static int32_t sm_gps_module_reboot(sm_gps_t* _this){
    if (!_this) {
        return -1;
    }
    sm_l76x_reboot(g_gps_l76x.g_gps_l76x);
    return 0;
}
static int32_t sm_gps_module_pw_reset(sm_gps_t* _this){
    if (!_this) {
        return -1;
    }
    sm_l76x_reboot(g_gps_l76x.g_gps_l76x);
    return 0;
}
static float sm_gps_module_get_lat(sm_gps_t*  _this){
    if (!_this) {
        return -1;
    }
    sm_l76x_get_lat(g_gps_l76x.g_gps_l76x);
    return 0;
}
static float sm_gps_module_get_lon(sm_gps_t* _this){
    if (!_this) {
        return -1;
    }
    sm_l76x_get_lon(g_gps_l76x.g_gps_l76x);
    return 0;
}
static int32_t sm_gps_module_get_time(sm_gps_t* _this, sm_gps_time_t* _time){
    if (!_this) {
        return -1;
    }
    sys_datetime_t* tmp;
    sm_l76x_get_time(g_gps_l76x.g_gps_l76x, tmp);
    sys_datetime_t* ret_time;
    ret_time = utc_offset(tmp, GMT_OFFSET);
    _time->hour = ret_time->hour;
    _time->min = ret_time->min;
    _time->sec = ret_time->sec;
    return 0;
}
static int32_t sm_gps_module_get_coordinate(sm_gps_t* _this, sm_gps_coordinate_t* _coordinate){
    if (!_this) {
        return -1;
    }
    _coordinate->lat = sm_l76x_get_lat(g_gps_l76x.g_gps_l76x);
    _coordinate->lon = sm_l76x_get_lon(g_gps_l76x.g_gps_l76x);
    return 0;
}
static int32_t sm_gps_module_process(sm_gps_t* _this){
    if (!_this) {
        return -1;
    }
    return  sm_l76x_process(g_gps_l76x.g_gps_l76x);
}

sm_gps_t *sm_gps_create(void* _driver){
    if(!_driver){
        return NULL;
    }
    g_gps_l76x.g_gps_l76x = _driver;

    return &g_gps_l76x.m_base;
}

uint8_t sm_gps_is_valid(sm_gps_t* _this){
    if(!_this){
        return 0;
    }
    return g_gps_l76x.g_gps_l76x->data_is_valid;
}
