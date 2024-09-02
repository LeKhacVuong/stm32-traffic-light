//
// Created by lekhacvuong on 7/27/24.
//
#include <stdio.h>
#include "sm_mb_slave_impl.h"
#include "linux_serial.h"
#include "sm_logger.h"

#define TAG "main"
#include "sm_host.h"
#include <time.h>
#define USB_PORT "/dev/ttyUSB0"
#define SLAVE_ID 0x99
int g_fd;
sm_host_t* g_host;
elapsed_timer_t sync_timeout;

uint16_t g_input_reg[128] = {0,};

enum LIGHT{
    BED_LIGHT,
    LIVING_LIGHT,
    KITCHEN_LIGHT,
    HALLWAY_LIGHT,
    LIGHT_NUMBER
};

typedef struct devTime_t{
    uint8_t m_hour;
    uint8_t m_min;
}devTime_t;

typedef struct light_info_t{
    uint8_t m_stt;
    uint8_t m_brightness;
    uint8_t m_auto;
    devTime_t m_autoConfigStart[3];
    devTime_t m_autoConfigStop[3];
}light_info_t;

typedef struct dev_info_t{
    devTime_t m_devTime;

    uint8_t m_doorStt;
    uint8_t m_bedFan;
    uint8_t m_humidity;
    uint8_t m_temperature;
    uint8_t m_fireSensor;
    uint8_t m_smokeSensor;
    uint8_t m_fireBuzzer;
    uint8_t m_hallwayDetectHuman;

    uint8_t m_autoFan;
    devTime_t m_autoFanStart;
    devTime_t m_autoFanStop;

    light_info_t m_light[LIGHT_NUMBER];
}dev_info_t;

dev_info_t g_devInfo;

int64_t get_tick_count() {
    struct timespec ts;
    int32_t tick = 0U;
    clock_gettime(CLOCK_REALTIME, &ts);
    tick = ts.tv_nsec / 1000000;
    tick += ts.tv_sec * 1000;
    return tick;
}

bool getLightStt(uint8_t _light){
    return g_devInfo.m_light[_light].m_stt;
}

void controlLight(uint8_t _light, uint8_t _stt){
    if(getLightStt(_light) == _stt)
        return;

    LOG_INF(TAG, "Now %s light %d at %dh - %dm", _stt?"ON":"OFF", _light, g_devInfo.m_devTime.m_hour, g_devInfo.m_devTime.m_min);
    g_devInfo.m_light[_light].m_stt = _stt;
    sm_host_send_cmd(g_host, CMD_HOLDING_DEV_SEND_SYNC_DATA, (const uint8_t *) &g_devInfo, sizeof(dev_info_t));
    elapsed_timer_reset(&sync_timeout);
}

int32_t serialHostSendIfDefault(const uint8_t* _data, int32_t _len, int32_t _timeout, void* _arg){
    return serial_send_bytes(g_fd, _data, _len);
}

int32_t serialHostRecvIfDefault(uint8_t *_buf, int32_t _len, int32_t _timeout, void* _arg) {
    return serial_recv_bytes(g_fd, _buf, _len);
}

void log_put(const char* _log) {
    printf("%s\n", _log);
}

int compareTime(devTime_t _first, devTime_t _second){
    if(_first.m_hour == _second.m_hour && _first.m_min == _second.m_min)
        return 0;

    if(_first.m_hour > _second.m_hour)
        return 1;

    if((_first.m_hour == _second.m_hour) && (_first.m_hour > _second.m_min))
        return 1;

    return -1;
}

bool isInTime(devTime_t _current, devTime_t _timeStart, devTime_t _timeStop) {
    if(compareTime(_timeStart, _timeStop) == 0)
        return false;


    if (compareTime(_timeStart, _timeStop) < 0){
        if(compareTime(_current, _timeStart)>=0 &&  compareTime(_current, _timeStop)<=0){
            return true;
        }
    }
    else {
        if(compareTime(_current, _timeStop) <=0 && _current.m_hour >= 0
            || compareTime(_current, _timeStart)>=0 && _current.m_hour < 24){
            return true;
        }
    }

    return false;
}

void increaseTimeMin(devTime_t* _time){
    if(!_time)
        return;

    _time->m_min++;
    if(_time->m_min >= 60){
        _time->m_min = 0;
        _time->m_hour++;
        if(_time->m_hour >= 24)
            _time->m_hour = 0;
    }
}

int32_t host_receive_cmd_callback(int32_t _cmd, const uint8_t* _data, int32_t _len, void* _arg ) {
    switch (_cmd) {
        case CMD_HOLDING_UPDATE_TIME:{
            uint8_t hour = _data[1];
            uint8_t min  = _data[0];
//            LOG_INF(TAG, "Set local time to %2dh-%2dm", hour, min);
            g_devInfo.m_devTime.m_hour = hour;
            g_devInfo.m_devTime.m_min = min;
            break;
        }
        case CMD_HOLDING_OPEN_DOOR:
            LOG_INF(TAG, "Now %s door lock", _data[0] ? "Open" : "Lock");
            g_devInfo.m_doorStt = _data[0];
            break;
        case CMD_HOLDING_CONTROL_LIGHT_BED:
            LOG_INF(TAG, "Now %s bed room light", _data[0] ? "Open" : "Lock");
            g_devInfo.m_light[BED_LIGHT].m_stt = _data[0];
            break;
        case CMD_HOLDING_SET_BED_BRIGHTNESS:
            LOG_INF(TAG, "Now set bed light brightness to %d", _data[0]);
            g_devInfo.m_light[BED_LIGHT].m_brightness = _data[0];
            break;
        case CMD_HOLDING_CONTROL_FAN_BED:
            LOG_INF(TAG, "Now %s bed room fan", _data[0] ? "Open" : "Lock");
            g_devInfo.m_bedFan = _data[0];
            break;
        case CMD_HOLDING_CONTROL_LIGHT_LIVING:
            LOG_INF(TAG, "Now %s living room light", _data[0] ? "Open" : "Lock");
            g_devInfo.m_light[LIVING_LIGHT].m_stt = _data[0];
            break;
        case CMD_HOLDING_SET_LIVING_BRIGHTNESS:
            LOG_INF(TAG, "Now set living light brightness to %d", _data[0]);
            g_devInfo.m_light[LIVING_LIGHT].m_brightness = _data[0];
            break;
        case CMD_HOLDING_CONTROL_LIGHT_KITCHEN:
            LOG_INF(TAG, "Now %s kitchen light", _data[0] ? "Open" : "Lock");
            g_devInfo.m_light[KITCHEN_LIGHT].m_stt = _data[0];
            break;
        case CMD_HOLDING_SET_KITCHEN_BRIGHTNESS:
            LOG_INF(TAG, "Now set kitchen light brightness to %d", _data[0]);
            g_devInfo.m_light[KITCHEN_LIGHT].m_brightness = _data[0];
            break;
        case CMD_HOLDING_CONTROL_LIGHT_HALLWAY:
            LOG_INF(TAG, "Now %s hallway light", _data[0] ? "Open" : "Lock");
            g_devInfo.m_light[HALLWAY_LIGHT].m_stt = _data[0];
            break;
        case CMD_HOLDING_SET_HALLWAY_BRIGHTNESS:
            LOG_INF(TAG, "Now set hallway light brightness to %d", _data[0]);
            g_devInfo.m_light[HALLWAY_LIGHT].m_brightness = _data[0];
            break;
        case CMD_HOLDING_CONTROL_FIRE_BUZZER:
            LOG_INF(TAG, "Now %s fire buzzer", _data[0] ? "Open" : "Lock");
            g_devInfo.m_fireBuzzer = _data[0];
            break;
        case CMD_HOLDING_SET_AUTO_LIGHT:{
            LOG_INF(TAG, "Set auto light %d to %s", _data[0], _data[1] ? "ON" : "OFF");
            uint8_t light = _data[0];
            g_devInfo.m_light[light].m_auto = _data[1];
            if(!_data[1]){
                controlLight(light, false);
                for(int i = 0; i < 3; i++){
                    g_devInfo.m_light[light].m_autoConfigStart[i].m_hour = 0;
                    g_devInfo.m_light[light].m_autoConfigStart[i].m_min = 0;
                    g_devInfo.m_light[light].m_autoConfigStop[i].m_hour = 0;
                    g_devInfo.m_light[light].m_autoConfigStop[i].m_min = 0;
                }
            }
            break;
        }
        case CMD_HOLDING_SET_AUTO_TIME:{
            uint8_t light = _data[0];
            uint8_t cfId = _data[1];
            uint8_t hourStart = _data[2];
            uint8_t minStart = _data[3];
            uint8_t hourStop = _data[4];
            uint8_t minStop = _data[5];
            g_devInfo.m_light[light].m_autoConfigStart[cfId].m_hour = hourStart;
            g_devInfo.m_light[light].m_autoConfigStart[cfId].m_min = minStart;
            g_devInfo.m_light[light].m_autoConfigStop[cfId].m_hour = hourStop;
            g_devInfo.m_light[light].m_autoConfigStop[cfId].m_min = minStop;
            LOG_INF(TAG, "Set auto time for light %d, cf %d, from %dh-%dm to %dh-%dm", light,
                                                                    cfId, hourStart, minStart, hourStop, minStop);
            break;
        }
        case CMD_HOLDING_DELETE_AUTO_TIME:{
            uint8_t light = _data[0];
            uint8_t cfId = _data[1];

            if(isInTime(g_devInfo.m_devTime, g_devInfo.m_light[light].m_autoConfigStart[cfId], g_devInfo.m_light[light].m_autoConfigStop[cfId])){
                controlLight(light, false);
            }

            g_devInfo.m_light[light].m_autoConfigStart[cfId].m_hour = 0;
            g_devInfo.m_light[light].m_autoConfigStart[cfId].m_min = 0;
            g_devInfo.m_light[light].m_autoConfigStop[cfId].m_hour = 0;
            g_devInfo.m_light[light].m_autoConfigStop[cfId].m_min = 0;
            LOG_INF(TAG, "Remove auto time for light %d, cf %d", light, cfId);
            break;
        }
        case CMD_HOLDING_HALLWAY_HUMAN_DETECT:
            LOG_INF(TAG, "Now %s hallway human detect", _data[0] ? "Enable" : "Disable");
            g_devInfo.m_hallwayDetectHuman = _data[0];
            break;
        case CMD_HOLDING_SET_AUTO_FAN:
            LOG_INF(TAG, "Set auto fan to %s", _data[0]? "ON" : "OFF");
            g_devInfo.m_autoFan = _data[0];
            if(g_devInfo.m_autoFan){
                uint8_t hourStart = _data[1];
                uint8_t minStart = _data[2];
                uint8_t hourStop = _data[3];
                uint8_t minStop = _data[4];

                g_devInfo.m_autoFanStart.m_hour = hourStart;
                g_devInfo.m_autoFanStart.m_min = minStart;
                g_devInfo.m_autoFanStop.m_hour = hourStop;
                g_devInfo.m_autoFanStop.m_min = minStop;
                LOG_INF(TAG, "Set auto time for bed fan, from %dh-%dm to %dh-%dm", hourStart, minStart, hourStop, minStop);
            }else{
                if(isInTime(g_devInfo.m_devTime, g_devInfo.m_autoFanStart, g_devInfo.m_autoFanStop)){
                    if(g_devInfo.m_bedFan){
                        g_devInfo.m_bedFan = false;
                        LOG_INF(TAG, "OFF fan at %dh - %dm because off auto", g_devInfo.m_devTime.m_hour, g_devInfo.m_devTime.m_min);
                    }
                }
            }
            break;
        case CMD_HOLDING_BLINK_LED_TEST:
            LOG_INF(TAG, "Blink led test...");
            break;
        default:
            break;
    }
    sm_host_send_cmd(g_host, CMD_HOLDING_DEV_SEND_SYNC_DATA, (const uint8_t *) &g_devInfo, sizeof(dev_info_t));
    elapsed_timer_reset(&sync_timeout);
    return 0;
}



void processAutoLight(){
    for(int id = 0; id < LIGHT_NUMBER; id++){
        if(g_devInfo.m_light[id].m_auto){
            for(int i = 0; i < 3; i++){
                if(isInTime(g_devInfo.m_devTime, g_devInfo.m_light[id].m_autoConfigStart[i], g_devInfo.m_light[id].m_autoConfigStop[i])){
                    controlLight(id, true);
                    return;
                }
            }
            controlLight(id, false);
        }
    }
}

void processAutoFan(){
    if(g_devInfo.m_autoFan){
        if(isInTime(g_devInfo.m_devTime, g_devInfo.m_autoFanStart, g_devInfo.m_autoFanStop)){
            if(!g_devInfo.m_bedFan){
                ///open fan
                g_devInfo.m_bedFan = true;
                LOG_INF(TAG, "ON fan at %dh - %dm", g_devInfo.m_devTime.m_hour, g_devInfo.m_devTime.m_min);
                sm_host_send_cmd(g_host, CMD_HOLDING_DEV_SEND_SYNC_DATA, (const uint8_t *) &g_devInfo, sizeof(dev_info_t));
                elapsed_timer_reset(&sync_timeout);
            }
            return;
        }
        if(g_devInfo.m_bedFan){
            ///Off fan
            g_devInfo.m_bedFan = false;
            LOG_INF(TAG, "OFF fan at %dh - %dm", g_devInfo.m_devTime.m_hour, g_devInfo.m_devTime.m_min);
            sm_host_send_cmd(g_host, CMD_HOLDING_DEV_SEND_SYNC_DATA, (const uint8_t *) &g_devInfo, sizeof(dev_info_t));
            elapsed_timer_reset(&sync_timeout);
        }
    }

}

int main(){

    sm_logger_init(log_put, LOG_LEVEL_DEBUG);

    g_fd = serial_init(USB_PORT, 9600, SERIAL_FLAG_BLOCKING);
    if (g_fd < 0) {
        printf("cannot open serial port\n");
        return -1;
    }

    time_t now;
    struct tm *tm_info;
    time(&now);
    tm_info = localtime(&now);
    g_devInfo.m_devTime.m_hour = tm_info->tm_hour;
    g_devInfo.m_devTime.m_min = tm_info->tm_min;

    elapsed_timer_resetz(&sync_timeout, 500);

    g_host = sm_host_create(SM_HOST_ASYNC_MODE, 0x99, serialHostSendIfDefault, serialHostRecvIfDefault, NULL);
    sm_host_reg_handle(g_host, host_receive_cmd_callback, NULL);

    while(true){
        uint8_t buf;
        int len = serial_recv_bytes(g_fd, &buf, 1);
        if(len > 0){
//            LOG_INF(TAG, "feed 1 bytes");
            sm_host_asyn_feed(&buf, 1, g_host);
        }
        sm_host_process(g_host);

        processAutoLight();
        processAutoFan();
    }
}
