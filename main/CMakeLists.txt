


idf_component_register(
    SRCS 
    "main.c"
    "espnow/espnow.c"
    "espnow/messages.c"
    "espnow/espnow_helper.c"
    "data/input_data.c"
    "data/setup_data.c"
    "gui/components/centered_label.c"
    "gui/components/channel_selector.c"
    "gui/components/flex_components.c"
    "gui/components/label_switch.c"
    "gui/components/simple_button.c"
    "gui/components/window_component.c"
    "gui/screens/1_main/monitor/channel_monitor.c"
    "gui/screens/1_main/settings/model_setup.c"
    "gui/screens/1_main/settings/servo_travel.c"
    "gui/screens/1_main/settings/servo_trim.c"
    "gui/screens/1_main/tabs/home_tab.c"
    "gui/screens/1_main/tabs/connection_tab.c"
    "gui/screens/1_main/tabs/settings_tab.c"
    "gui/screens/1_main/main_screen.c"
    "gui/styles/styles.c"
    "gui/gui.c"
    "gui/driver/esp32/waveshare_bsp.c"
    "gui/driver/esp32/rgb_interface.c"
    "gui/driver/esp32/lvgl_port.c"
    "sdcard/sdcard_interface.c"
    
    REQUIRES 
        esp_timer
        esp_wifi
        nvs_flash
        esp_driver_sdmmc
        fatfs

    INCLUDE_DIRS 
    "."
    )
