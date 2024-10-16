/*
 * sm_datetime.h
 *
 *  Created on: Oct 14, 2024
 *      Author: Ng Thuy Quynh
 */

#ifndef COMMON_DATA_SM_DATETIME_H_
#define COMMON_DATA_SM_DATETIME_H_

#include <stdint.h>

typedef union datetime{
    uint8_t arr_datetime[6];
    struct {
        uint8_t year;
        uint8_t mon;
        uint8_t day;
        uint8_t hour;
        uint8_t min;
        uint8_t sec;
    };
}sys_datetime_t;

#endif /* COMMON_DATA_SM_DATETIME_H_ */
