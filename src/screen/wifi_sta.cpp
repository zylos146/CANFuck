#include "screen/wifi_sta.hpp"
#include "wifi.hpp"
#include <ESPConnect.h>

WifiStaScreen::WifiStaScreen() {
  loopCount = 0;

  ui_root = ui_WIFI = lv_obj_create(NULL);
  lv_obj_clear_flag(ui_WIFI, LV_OBJ_FLAG_SCROLLABLE); /// Flags

  ui_WIFI_Panel1 = lv_obj_create(ui_WIFI);
  lv_obj_set_width(ui_WIFI_Panel1, 240);
  lv_obj_set_height(ui_WIFI_Panel1, 280);
  lv_obj_set_x(ui_WIFI_Panel1, 0);
  lv_obj_set_y(ui_WIFI_Panel1, 0);
  lv_obj_set_align(ui_WIFI_Panel1, LV_ALIGN_CENTER);
  lv_obj_clear_flag(ui_WIFI_Panel1, LV_OBJ_FLAG_SCROLLABLE); /// Flags

  ui_WIFI_Arc1 = lv_arc_create(ui_WIFI_Panel1);
  lv_obj_set_width(ui_WIFI_Arc1, 240);
  lv_obj_set_height(ui_WIFI_Arc1, 240);
  lv_obj_set_align(ui_WIFI_Arc1, LV_ALIGN_CENTER);
  lv_arc_set_bg_angles(ui_WIFI_Arc1, 225, -60);
  lv_obj_set_style_arc_color(ui_WIFI_Arc1, lv_color_hex(0x2095BD), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_opa(ui_WIFI_Arc1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_width(ui_WIFI_Arc1, 18, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_arc_width(ui_WIFI_Arc1, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

  lv_obj_set_style_bg_color(ui_WIFI_Arc1, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_WIFI_Arc1, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

  ui_WIFI_Arc2 = lv_arc_create(ui_WIFI_Panel1);
  lv_obj_set_width(ui_WIFI_Arc2, 160);
  lv_obj_set_height(ui_WIFI_Arc2, 160);
  lv_obj_set_align(ui_WIFI_Arc2, LV_ALIGN_CENTER);
  lv_arc_set_bg_angles(ui_WIFI_Arc2, 225, -60);
  lv_obj_set_style_arc_color(ui_WIFI_Arc2, lv_color_hex(0x2095BD), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_opa(ui_WIFI_Arc2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_width(ui_WIFI_Arc2, 18, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_arc_width(ui_WIFI_Arc2, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

  lv_obj_set_style_bg_color(ui_WIFI_Arc2, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_WIFI_Arc2, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

  ui_WIFI_Arc3 = lv_arc_create(ui_WIFI_Panel1);
  lv_obj_set_width(ui_WIFI_Arc3, 80);
  lv_obj_set_height(ui_WIFI_Arc3, 80);
  lv_obj_set_align(ui_WIFI_Arc3, LV_ALIGN_CENTER);
  lv_arc_set_bg_angles(ui_WIFI_Arc3, 225, -60);
  lv_obj_set_style_arc_color(ui_WIFI_Arc3, lv_color_hex(0x2095BD), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_opa(ui_WIFI_Arc3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_width(ui_WIFI_Arc3, 18, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_arc_width(ui_WIFI_Arc3, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

  lv_obj_set_style_bg_color(ui_WIFI_Arc3, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_WIFI_Arc3, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

  ui_WIFI_Arc4 = lv_arc_create(ui_WIFI_Panel1);
  lv_obj_set_width(ui_WIFI_Arc4, 50);
  lv_obj_set_height(ui_WIFI_Arc4, 50);
  lv_obj_set_x(ui_WIFI_Arc4, 0);
  lv_obj_set_y(ui_WIFI_Arc4, 20);
  lv_obj_set_align(ui_WIFI_Arc4, LV_ALIGN_CENTER);
  lv_arc_set_bg_angles(ui_WIFI_Arc4, 270, 271);
  lv_obj_set_style_arc_color(ui_WIFI_Arc4, lv_color_hex(0x2095BD), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_opa(ui_WIFI_Arc4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_width(ui_WIFI_Arc4, 18, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_arc_width(ui_WIFI_Arc4, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

  lv_obj_set_style_bg_color(ui_WIFI_Arc4, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_WIFI_Arc4, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

  ui_WIFI_Name = lv_textarea_create(ui_WIFI_Panel1);
  lv_obj_set_width(ui_WIFI_Name, 240);
  lv_obj_set_height(ui_WIFI_Name, 32);
  lv_obj_set_x(ui_WIFI_Name, 0);
  lv_obj_set_y(ui_WIFI_Name, 64);
  lv_obj_set_align(ui_WIFI_Name, LV_ALIGN_CENTER);
  lv_obj_set_style_text_align(ui_WIFI_Name, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_WIFI_Name, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_WIFI_Name, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_textarea_set_text(ui_WIFI_Name, "####");

  ui_WIFI_NameTitle = lv_textarea_create(ui_WIFI_Panel1);
  lv_obj_set_width(ui_WIFI_NameTitle, 240);
  lv_obj_set_height(ui_WIFI_NameTitle, 38);
  lv_obj_set_x(ui_WIFI_NameTitle, 0);
  lv_obj_set_y(ui_WIFI_NameTitle, 40);
  lv_obj_set_align(ui_WIFI_NameTitle, LV_ALIGN_CENTER);
  lv_obj_set_style_text_align(ui_WIFI_NameTitle, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_WIFI_NameTitle, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_WIFI_NameTitle, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(ui_WIFI_NameTitle, &montserrat_16_bold, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_textarea_set_text(ui_WIFI_NameTitle, "Name");

  ui_WIFI_PasswordTitle = lv_textarea_create(ui_WIFI);
  lv_obj_set_width(ui_WIFI_PasswordTitle, 240);
  lv_obj_set_height(ui_WIFI_PasswordTitle, 38);
  lv_obj_set_x(ui_WIFI_PasswordTitle, 0);
  lv_obj_set_y(ui_WIFI_PasswordTitle, 90);
  lv_obj_set_align(ui_WIFI_PasswordTitle, LV_ALIGN_CENTER);
  lv_obj_set_style_text_align(ui_WIFI_PasswordTitle, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_WIFI_PasswordTitle, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_WIFI_PasswordTitle, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(ui_WIFI_PasswordTitle, &montserrat_16_bold, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_textarea_set_text(ui_WIFI_PasswordTitle, "Connecting");
}

void WifiStaScreen::tick() {
  strcpy(connectString, "Connecting");
  for (uint8_t i = 0; i < loopCount; i++) {
    if (i % 3 == 0) {
      strcat(connectString, ".");
    }
  }

  lv_textarea_set_text(ui_WIFI_PasswordTitle, connectString);
  lv_textarea_set_text(ui_WIFI_Name, ESPConnect.getSSID().c_str());
  
  loopCount = (loopCount + 1) % 9;
  lv_refr_now(NULL);
}