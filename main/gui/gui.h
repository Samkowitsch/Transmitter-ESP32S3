//
// Created by Sam Weffling on 30/12/2025.
//

#ifndef LVGL_GUI_H
#define LVGL_GUI_H
#include "lvgl.h"
#include "gui/styles/styles.h"
#include "data/setup_data.h"
#include <stdint.h>

#define ENABLE_TEST_INPUT 0


typedef void (*close_window_cb_t)(void);


#define SCREEN_WIDTH  (800)
#define  SCREEN_HEIGHT  (480)


#define  TOOLBAR_HEIGHT  (50)
#define MAIN_TAB_WIDTH (75)

#define  MAIN_WINDOW_HEIGHT (SCREEN_HEIGHT - TOOLBAR_HEIGHT)
#define  MAIN_WINDOW_WIDTH  (SCREEN_WIDTH - MAIN_TAB_WIDTH)

void init_gui();

#endif //LVGL_GUI_H

