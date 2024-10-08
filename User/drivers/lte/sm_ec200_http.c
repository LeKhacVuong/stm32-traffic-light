//
// Created by admin on 9/16/2024.
//
#include <stdio.h>
#include "sm_ec200_http.h"
#include "sm_hal.h"
#include <string.h>
#define SEC(x)	                        (uint32_t)(1000*x)

#define HTTP_CONFIG_TIMEOUT_MS          SEC(5)
#define HTTP_OPEN_TIMEOUT_MS            SEC(5)
#define HTTP_CLOSE_TIMEOUT_MS           SEC(5)
#define HTTP_READ_TIMEOUT_MS            SEC(60)
#define HTTP_WRITE_TIMEOUT_MS           SEC(5)
#define HTTP_COMMON_TIMEOUT_MS          SEC(5)

int32_t sm_ec200_http_config(sm_ec200_t* modem, const char* cfg, int val) {
    if (!modem) return -1;
    sm_modem_t* p_modem = (sm_modem_t*) modem;
    char cmd[128];
    sprintf(cmd, "AT+QHTTPCFG=\"%s\",%d\r\n",cfg, val);
    return sm_modem_send_cmd(p_modem , cmd,
                        "OK\r\n",
                        "ERROR",
                                HTTP_CONFIG_TIMEOUT_MS);
}

int32_t sm_ec200_http_add_header(sm_ec200_t* modem, const char* header) {
    if (!modem) return -1;
    sm_modem_t* p_modem = (sm_modem_t*) modem;
    char cmd[128];
    sprintf(cmd, "AT+QHTTPCFG=\"header\",%s\r\n",header);
    return sm_modem_send_cmd(p_modem , cmd,
                        "OK\r\n",
                        "ERROR",
                                HTTP_COMMON_TIMEOUT_MS);
}

int32_t sm_ec200_http_open(sm_ec200_t *modem, char *host, int port) {
    if (!modem) return -1;
    sm_modem_t* p_modem = (sm_modem_t*) modem;
    char cmd[256];
    sprintf(cmd, "AT+QIOPEN=1,1,\"TCP\",\"%s\",%d,0,0\r\n", host, port);
    return sm_modem_send_cmd(p_modem, cmd,
                        "OK\r\n\r\n+QIOPEN: 1,0",
                        "ERROR",
                                HTTP_OPEN_TIMEOUT_MS);
}

int32_t sm_ec200_http_close(sm_ec200_t *modem) {
    if (!modem) return -1;
    sm_modem_t* p_modem = (sm_modem_t*) modem;
    return sm_modem_send_cmd(p_modem, "AT+QICLOSE=1\r\n",
                        "OK\r\n",
                        "ERROR\r\n",
                                HTTP_CLOSE_TIMEOUT_MS);
}

int32_t sm_ec200_http_write(sm_ec200_t *modem, char *data, uint32_t len, int32_t* status_code) {
    if (!modem) return -1;
    sm_modem_t* p_modem = (sm_modem_t*) modem;
    char cmd[128];
    sprintf(cmd, "AT+QHTTPPOST=%lu,80,80\r\n", len);
    int32_t err = sm_modem_send_cmd(p_modem, cmd,
                                "CONNECT",
                                "ERROR",
                                HTTP_COMMON_TIMEOUT_MS);
    if (err) return err;

    err = sm_modem_send_cmd(p_modem, data,
                        "OK\r\n\r\n+QHTTPPOST",
                        "ERROR",
                            HTTP_WRITE_TIMEOUT_MS);
    char* p_status = strstr(p_modem->buff, "+QHTTPPOST: 0");
    int32_t status = -1;
    if (p_status) {
        sscanf(p_status, "+QHTTPPOST: 0,%d,", &status);
    }
    *status_code = status;
    return (err != 0) ? -1 : 0;
}

int32_t sm_ec200_http_read(sm_ec200_t *modem, char *data) {
    if (!modem) return -1;
    sm_modem_t* p_modem = (sm_modem_t*) modem;
    char cmd[128];
    sprintf(cmd, "AT+QHTTPREAD=80\r\n");
    int32_t err = sm_modem_send_cmd (p_modem, cmd,
                    "+QHTTPREAD: 0\r\n",
                    "+CME ERROR",
                            HTTP_READ_TIMEOUT_MS);

    if (!err){
        strcpy(data, modem->base.buff);
    }
    return err;
}

int32_t sm_ec200_http_get_range(sm_ec200_t* modem, uint32_t start, uint32_t range, int32_t* status_code) {
    if (!modem) return -1;
    sm_modem_t* p_modem = (sm_modem_t*) modem;
    char buff[64] = {0};
    memset(buff, 0, 64);
    sprintf(buff, "AT+QHTTPGETEX=80,%d,%d\r\n", start, range);
    int32_t err =  sm_modem_send_cmd(p_modem, buff,
                                "+QHTTPGET: 0,20",
                                "+CME ERROR\r\n",
                                HTTP_COMMON_TIMEOUT_MS);
    char* p_status = strstr(p_modem->buff, "+QHTTPGET: 0");
    int32_t status = -1;
    if (p_status) {
        sscanf(p_status, "+QHTTPGET: 0,%d,", &status);
    }
    *status_code = status;
    return (err != 0) ? -1 : 0;
}

int32_t sm_ec200_set_http_url(sm_ec200_t *modem, char *url) {
    if (!modem) return -1;
    sm_modem_t* p_modem = (sm_modem_t*) modem;
    char buff[64] = { 0 };
    memset(buff, 0, 64);
    sprintf(buff, "AT+QHTTPURL=%d,%d\r\n", (int)strlen(url), 60);
    int32_t err = sm_modem_send_cmd (p_modem, buff,
                                    "CONNECT",
                                    "+CME ERROR\r\n",
                                    HTTP_COMMON_TIMEOUT_MS);
    if (err){
        return err;
    }

    return sm_modem_send_cmd(p_modem, url,
                        "OK\r\n",
                        "+CME ERROR\r\n",
                        HTTP_COMMON_TIMEOUT_MS);
}
