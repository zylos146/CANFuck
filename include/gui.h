#ifndef GUI_H
#define GUI_H

#define LV_CONF_INCLUDE_SIMPLE 1
#include <TFT_eSPI.h>
#include <lvgl.h>

extern char wifiName[32];
extern char wifiPassword[10];
void gui_main_task(void *pvParameter);

#endif