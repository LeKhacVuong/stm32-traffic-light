#ifndef SM_EC200N_H
#define SM_EC200N_H

#include "time.h"
#include "modules/at-modem/sm_modem.h"
#include <stdbool.h>

typedef struct sm_ec200 sm_ec200_t;

struct sm_ec200{
    sm_modem_t base;
    int rssi;   // 0 - 31
    int ber;    // 99: invalid,
    int ip[4];
    char IMSI[20];
    char simNb[8];
    char name[20];
};

/**
 * @brief
 * @param modem
 * @param reset_pin
 * @param driver
 */
void sm_ec200_init(sm_ec200_t *modem, sm_hal_io_t *reset_pin, sm_hal_uart_t *driver);
/**
 * @brief
 * @param modem
 */
void sm_ec200_deinit(sm_ec200_t *modem);
/**
 * @brief
 * @param modem
 * @return
 */
int32_t sm_ec200_sync(sm_ec200_t *modem);
/**
 * @brief
 * @param modem
 * @param mode
 * @return
 */
int32_t sm_ec200_echo_mode(sm_ec200_t *modem, uint8_t mode); // mode = 0 : disable echo mode, mode = 1 : enable echo mode
/**
 * @brief
 * @param modem
 * @param pin
 * @return
 */
int32_t sm_ec200_set_pin(sm_ec200_t *modem, char *pin);
/**
 * @brief
 * @param modem
 * @return
 */
int32_t sm_ec200_get_pin(sm_ec200_t *modem);
/**
 * @brief
 * @param modem
 * @return
 */
int32_t sm_ec200_get_signal(sm_ec200_t *modem);
/**
 * @brief
 * @param modem
 * @param name
 * @return
 */
int32_t sm_ec200_get_name(sm_ec200_t *modem, char *name);
/**
 * @brief
 * @param modem
 * @return
 */
int32_t sm_ec200_get_function(sm_ec200_t *modem);
/**
 * @brief
 * @param modem
 * @param apn
 * @param user
 * @param pass
 * @return
 */
int32_t sm_ec200_set_apn(sm_ec200_t *modem, char *apn, char *user, char *pass);
/**
 * @brief
 * @param modem
 * @return
 */
int32_t sm_ec200_get_network_reg(sm_ec200_t *modem);
/**
 * @brief
 * @param modem
 * @return
 */
int32_t sm_ec200_get_simNb(sm_ec200_t *modem);
/**
 * @brief
 * @param modem
 * @return
 */
int32_t sm_ec200_get_ip(sm_ec200_t *modem);

/**
 * @brief
 * @param modem
 * @param p_time
 * @return
 */
int32_t sm_ec200_get_time_network(sm_ec200_t* modem, struct tm *p_time);

/**
 * @brief
 * @param modem
 * @return
 */
int32_t sm_ec200_hard_reset(sm_ec200_t *modem) ;
#endif //SM_EC200N_H
