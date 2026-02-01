#include "home_tab.h"

#include "esp_log.h"
#include "esp_system.h"


void reboot_btn_cb(lv_event_t *event){


    lv_event_code_t code = lv_event_get_code(event);
    if(code == LV_EVENT_CLICKED){
        ESP_LOGI("REBT" , "reboot esp");
        esp_restart();
    }

}



void draw_home_tab(lv_obj_t* parent){

    lv_obj_t* reboot_btn = lv_button_create(parent);

    lv_obj_t* label = lv_label_create(reboot_btn);
    lv_obj_set_align(label , LV_ALIGN_CENTER);
    lv_label_set_text(label , "REBOOT");
    lv_obj_set_align(reboot_btn , LV_ALIGN_CENTER);

    lv_obj_add_event_cb(reboot_btn , reboot_btn_cb , LV_EVENT_CLICKED , NULL);
}