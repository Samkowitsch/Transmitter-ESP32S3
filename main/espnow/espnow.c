//
// Created by Sam Weffling on 28.01.26.
//

#include "espnow.h"

#include "nvs_flash.h"
#include "esp_random.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_now.h"
#include "esp_crc.h"

#include "messages.h"
#include "espnow_helper.h"

const uint8_t broadcast_mac[ESP_NOW_ETH_ALEN] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
static uint8_t esp_mac[ESP_NOW_ETH_ALEN] = { 0 };
#define IS_BROADCAST_ADDR(addr) (memcmp(addr, broadcast_mac, ESP_NOW_ETH_ALEN) == 0)


static TaskHandle_t espnow_task_handle = NULL;
static TaskHandle_t espnow_transmit_task_handle = NULL;
static QueueHandle_t espnow_action_queue = NULL;


const char* TAG = "ESPNOW";


static void init_wifi(void)
{

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    const wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(ESPNOW_WIFI_MODE) );
    ESP_ERROR_CHECK( esp_wifi_start());
    ESP_ERROR_CHECK( esp_wifi_set_channel(CONFIG_ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE));

#if CONFIG_ESPNOW_ENABLE_LONG_RANGE
    ESP_ERROR_CHECK( esp_wifi_set_protocol(ESPNOW_WIFI_IF, WIFI_PROTOCOL_11B|WIFI_PROTOCOL_11G|WIFI_PROTOCOL_11N|WIFI_PROTOCOL_LR) );
#endif
}



/* ESPNOW sending or receiving callback function is called in WiFi task.
 * Users should not do lengthy operations from this task. Instead, post
 * necessary data to a queue and handle it from a lower priority task. */
static void espnow_send_cb(const esp_now_send_info_t *tx_info, esp_now_send_status_t status)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (tx_info == NULL) {
        ESP_LOGE(TAG, "Send cb arg error");
        return;
    }

    if(espnow_task_handle){
        vTaskNotifyGiveIndexedFromISR(espnow_task_handle , ESPNOW_SEND_NOTIFY , &xHigherPriorityTaskWoken);
    }
}

static void espnow_receive_cb(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len)
{
    uint8_t * mac_addr = recv_info->src_addr;
    uint8_t * des_addr = recv_info->des_addr;
    int8_t rssi = recv_info->rx_ctrl->rssi;

    //ESP_LOGI(TAG, "Received ESPNOW data, len: %d RSSI %d", len , rssi);
    //ESP_LOGI(TAG , "\t 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x " , mac_addr[0] , mac_addr[1] , mac_addr[2] , mac_addr[3] , mac_addr[4] , mac_addr[5]);

    if (mac_addr == NULL || data == NULL || len <= sizeof(t_message_header)) {
        ESP_LOGE(TAG, "Receive cb arg error");
        return;
    }

    const t_message_header *header = (t_message_header*)data;


    t_espnow_data receive_data;
    receive_data.action = ESPNOW_RECEIVE;
    receive_data.rssi = recv_info->rx_ctrl->rssi;
    memcpy(&receive_data.mac_addr, recv_info->src_addr , sizeof(receive_data.mac_addr));
    receive_data.data_len = len;
    receive_data.data = malloc(len);
    memcpy(receive_data.data, data, len);

    if (xQueueSend(espnow_action_queue , &receive_data , 0) != pdTRUE)
    {
        free(receive_data.data);
    }
    
}

esp_err_t espnow_add_peer(uint8_t mac_address[6])
{

    if (!is_valid_mac(mac_address))
    {
        ESP_LOGE(TAG, "Not a valid MAC address");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG , " add peer ");

    /* Add broadcast peer information to peer list. */
    esp_now_peer_info_t *peer = malloc(sizeof(esp_now_peer_info_t));
    if (peer == NULL) {
        ESP_LOGE(TAG, "Malloc peer information fail");
        return ESP_FAIL;
    }
    memset(peer, 0, sizeof(esp_now_peer_info_t));
    peer->channel = CONFIG_ESPNOW_CHANNEL;
    peer->ifidx = ESPNOW_WIFI_IF;
    peer->encrypt = false;
    memcpy(peer->peer_addr, mac_address, ESP_NOW_ETH_ALEN);
    const esp_err_t ret = esp_now_add_peer(peer);
    free(peer);
    return ret;
}


esp_err_t espnow_remove_peers()
{
    esp_now_peer_num_t peer_num = {0};
    esp_now_get_peer_num(&peer_num);
    ESP_LOGI(TAG , "Peers in list %d" , peer_num.total_num);

    for (int i = 0 ; i < peer_num.total_num ; i++)
    {
        esp_now_peer_info_t peer_info = {0};
        esp_now_fetch_peer(true , &peer_info);
        if (is_valid_mac(peer_info.peer_addr))
        {
            ESP_LOGI(TAG , "In list MAC : 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x" ,
                peer_info.peer_addr[0],
                peer_info.peer_addr[1],
                peer_info.peer_addr[2],
                peer_info.peer_addr[3],
                peer_info.peer_addr[4],
                peer_info.peer_addr[5]
            );
        }
    }

    return ESP_OK;
}




static esp_err_t init_espnow(void)
{
    /* Initialize ESPNOW and register sending and receiving callback function. */
    ESP_ERROR_CHECK( esp_now_init() );
    ESP_ERROR_CHECK( esp_now_register_send_cb(espnow_send_cb) );
    ESP_ERROR_CHECK( esp_now_register_recv_cb(espnow_receive_cb) );
    /* Set primary master key. */
    //ESP_ERROR_CHECK( esp_now_set_pmk((uint8_t *)CONFIG_ESPNOW_PMK) );

    ESP_ERROR_CHECK(espnow_add_peer((uint8_t*)broadcast_mac));

    return ESP_OK;
}

void espnow_task(void *parameter)
{
    t_espnow_data espnow_data;

    init_wifi();
    init_espnow();



    ESP_ERROR_CHECK(esp_read_mac(esp_mac , ESP_MAC_WIFI_STA));
    ESP_LOGI(TAG , "MAC : 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x" , esp_mac[0] , esp_mac[1], esp_mac[2], esp_mac[3], esp_mac[4], esp_mac[5]);


    while (1)
    {

        if (xQueueReceive(espnow_action_queue , &espnow_data, (100 / portTICK_PERIOD_MS)) == pdTRUE)
        {

            if(espnow_data.action == ESPNOW_RECEIVE){
                // Handle received data from espnow
                const t_message_header *header = (t_message_header *)espnow_data.data;
                switch (header->type)
                {
                    case MSG_BROADCAST: {
                            ESP_LOGI(TAG , "Received message type broadcast");
                            const t_message_broadcast *broadcast = (t_message_broadcast*)espnow_data.data;
                            ESP_LOGI(TAG , "\t Sender : %s Mac : 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x" , broadcast->name , espnow_data.mac_addr[0] , espnow_data.mac_addr[1] , espnow_data.mac_addr[2] , espnow_data.mac_addr[3] ,espnow_data.mac_addr[4] , espnow_data.mac_addr[5]); 
                            
                            esp_now_peer_info_t peer_info;
                            if(esp_now_get_peer(espnow_data.mac_addr , &peer_info) != ESP_OK){
                                espnow_add_peer(espnow_data.mac_addr);
                            }
             
                            t_espnow_data transmit_data;
                            memcpy(transmit_data.mac_addr , espnow_data.mac_addr , 6);
                            t_message_binding *binding = malloc(sizeof(t_message_binding));
                            binding->header.type = MSG_BINDING;
                            binding->header.timestamp = xTaskGetTickCount();
                            memcpy(binding->sender_mac , esp_mac , 6);

                            transmit_data.data = (uint8_t*)binding;
                            transmit_data.data_len = sizeof(t_message_binding);
                            transmit_data.action = ESPNOW_TRANSMIT;
                            if(xQueueSend(espnow_action_queue , &transmit_data , 0) != pdTRUE){
                                free(binding);
                            }
                        
                        }
                        break;
                    default:
                        break;

                }

                free(espnow_data.data);

            }else if(espnow_data.action == ESPNOW_TRANSMIT){

                ESP_LOGI(TAG , "Transmit data ");
                esp_err_t ret = esp_now_send(espnow_data.mac_addr , espnow_data.data , espnow_data.data_len);
                if(ret == ESP_OK){
                    ulTaskNotifyTakeIndexed(ESPNOW_SEND_NOTIFY , pdTRUE , portMAX_DELAY);
                }else{
                    ESP_LOGE(TAG ," send error : %s " , esp_err_to_name(ret));
                }
                


                free(espnow_data.data);
            }


        }else{
            //ESP_LOGI(TAG , "Timeout");
        }
    }
}


void init_espnow_task(void){

    const uint32_t stack = 1024 * 5;
    espnow_action_queue = xQueueCreate(2 , sizeof(t_espnow_data));
    xTaskCreate(espnow_task , "ESP_NOW Task" , stack , NULL, 2 , &espnow_task_handle);
    //xTaskCreate(espnow_transmit_task , "Transmit Task" , 1024, NULL , 2 , &espnow_transmit_task_handle);
}