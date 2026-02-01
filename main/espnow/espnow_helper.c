//
// Created by Sam Weffling on 01.02.26.
//

#include "espnow_helper.h"
#include "espnow.h"


bool is_valid_mac(const uint8_t mac[6])
{
    bool valid_mac = false;
    for (int i = 0; i < 6; i++)
    {
        if (mac[i] != 0)
        {
            valid_mac = true;
        }
    }
    return valid_mac;
}

bool is_broadcast_mac(const uint8_t mac[6])
{
    for (int i = 0; i < 6; i++)
    {
        if (mac[i] != broadcast_mac[i])
        {
            return false;
        }
    }
    return true;
}