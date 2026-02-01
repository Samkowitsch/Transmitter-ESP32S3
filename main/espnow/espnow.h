#ifndef ESPNOW_H
#define ESPNOW_H

#include "esp_now.h"

#define CONFIG_ESPNOW_WIFI_MODE_STATION 1
#define CONFIG_ESPNOW_CHANNEL 1
#define CONFIG_ESPNOW_ENABLE_LONG_RANGE 0

/* ESPNOW can work in both station and softap mode. It is configured in menuconfig. */
#if CONFIG_ESPNOW_WIFI_MODE_STATION
#define ESPNOW_WIFI_MODE WIFI_MODE_STA
#define ESPNOW_WIFI_IF   WIFI_IF_STA
#else
#define ESPNOW_WIFI_MODE WIFI_MODE_AP
#define ESPNOW_WIFI_IF   WIFI_IF_AP
#endif


typedef enum
{
    ESPNOW_SEND_NOTIFY = 0,
    ESPNOW_RECEIVE_NOTIFY = 1
}e_espnow_task_notify;

typedef enum {
    ESPNOW_TRANSMIT,
    ESPNOW_RECEIVE
}e_espnow_action;

typedef struct
{
    e_espnow_action action;
    uint8_t mac_addr[ESP_NOW_ETH_ALEN];
    int32_t rssi;
    uint8_t *data;
    int data_len;
}t_espnow_data;



extern const uint8_t broadcast_mac[ESP_NOW_ETH_ALEN];


esp_err_t espnow_send(const uint8_t *mac_address ,const uint8_t* message , size_t len);
void init_espnow_task(void);

#endif
