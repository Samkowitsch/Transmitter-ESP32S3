//
// Created by Sam Weffling on 31/12/2025.
//

#include "settings_tab.h"

#include "gui/components/window_component.h"
#include "gui/screens/1_main/monitor/channel_monitor.h"
#include "gui/screens/1_main/settings/model_setup.h"
#include "gui/screens/1_main/settings/servo_travel.h"
#include "gui/screens/1_main/settings/servo_trim.h"

static const char* settings_map[] = {
    "Model" , "Channel" , "Servo" , "\n" ,
    "Trim" , "Exp." , "Monitor" , NULL
};






static void event_handler(lv_event_t* event) {
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t* target = lv_event_get_target(event);

    if (code == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t* parent = lv_event_get_user_data(event);
        uint32_t id = lv_buttonmatrix_get_selected_button(target);
        LV_LOG_USER("Button pressed : %s , id %ld" , lv_buttonmatrix_get_button_text(target , id) , id );

        switch (id) {
            case 0:
                // Open Channel window
                draw_model_setup_window(parent);
                break;
            case 1:

                break;
            case 2: // Servo Settings
                draw_servo_travel_window(parent);
                break;
            case 3: // Trim
                draw_servo_trim_window(parent);
                break;
            case 4:
                break;
            case 5: // Monitor
                draw_channel_monitor(parent);
                break;
            default:
                break;
        }
    }


}



void draw_settings_tab(lv_obj_t* parent) {

    lv_obj_set_style_margin_all(parent , 0 , LV_PART_MAIN);
    lv_obj_set_style_pad_all(parent , 0 , LV_PART_MAIN);

    lv_obj_t* settings_matrix = lv_buttonmatrix_create(parent);
    lv_buttonmatrix_set_map(settings_matrix , settings_map);
    lv_obj_set_align(settings_matrix , LV_ALIGN_CENTER);
    lv_obj_set_size(settings_matrix , lv_pct(100) , lv_pct(100));
    lv_obj_add_style(settings_matrix , &main_tab_content_style , LV_PART_MAIN);
    lv_obj_add_event_cb(settings_matrix , event_handler , LV_EVENT_VALUE_CHANGED, parent);
}