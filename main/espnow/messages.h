#ifndef ESPNOW_MESSAGES_H
#define ESPNOW_MESSAGES_H

#include <stdint.h>
#include "esp_now.h"

typedef enum
{
    MSG_BROADCAST,
    MSG_BINDING,
    MSG_ACK,
    MSG_ERROR,
    MSG_CHANNELS,
}e_message_type;


typedef struct
{
    e_message_type type;
    uint32_t timestamp;
}t_message_header;

typedef struct
{
    t_message_header header;
    char name[32];
}t_message_broadcast;

typedef struct
{
    t_message_header header;
    uint8_t sender_mac[6];
}t_message_binding;

typedef struct
{
    t_message_header header;
    int16_t servo[16];
    uint32_t lights;
}t_message_data;



#endif 