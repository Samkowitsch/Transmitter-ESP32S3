//
// Created by Sam Weffling on 01.02.26.
//

#ifndef ESP_C3_RECEIVER_ESPNOW_HELPER_H
#define ESP_C3_RECEIVER_ESPNOW_HELPER_H

#include <stdint.h>

bool is_valid_mac(const uint8_t mac[6]);
bool is_broadcast_mac(const uint8_t mac[6]);
#endif //ESP_C3_RECEIVER_ESPNOW_HELPER_H