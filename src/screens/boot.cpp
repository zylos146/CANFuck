#include "screen/boot.hpp"

// TODO - Move this to an extern defined somewhere global
BootScreen::BootScreen() {
  ui_root = ui_BOOT = lv_obj_create(NULL);
  lv_obj_clear_flag(ui_BOOT, LV_OBJ_FLAG_SCROLLABLE);

  ui_BOOT_Panel1 = lv_obj_create(ui_BOOT);
  lv_obj_set_width(ui_BOOT_Panel1, 240);
  lv_obj_set_height(ui_BOOT_Panel1, 280);
  lv_obj_set_align(ui_BOOT_Panel1, LV_ALIGN_CENTER);
  lv_obj_clear_flag(ui_BOOT_Panel1, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_radius(ui_BOOT_Panel1, 40, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_BOOT_Image5 = lv_img_create(ui_BOOT_Panel1);
  lv_img_set_src(ui_BOOT_Image5, &canfuck_small);
  lv_obj_set_width(ui_BOOT_Image5, LV_SIZE_CONTENT);   /// 800
  lv_obj_set_height(ui_BOOT_Image5, LV_SIZE_CONTENT);    /// 190
  lv_obj_set_align(ui_BOOT_Image5, LV_ALIGN_CENTER);
  lv_obj_add_flag(ui_BOOT_Image5, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
  lv_obj_clear_flag(ui_BOOT_Image5, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
  
  ui_BOOT_Name = lv_textarea_create(ui_BOOT_Panel1);
  lv_obj_set_width(ui_BOOT_Name, 240);
  lv_obj_set_height(ui_BOOT_Name, 32);
  lv_obj_set_x(ui_BOOT_Name, 0);
  lv_obj_set_y(ui_BOOT_Name, 64);
  lv_obj_set_align(ui_BOOT_Name, LV_ALIGN_CENTER);
  lv_obj_set_style_text_align(ui_BOOT_Name, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_BOOT_Name, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_BOOT_Name, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_textarea_set_text(ui_BOOT_Name, "Loading...");

  ui_BOOT_NameTitle = lv_textarea_create(ui_BOOT_Panel1);
  lv_obj_set_width(ui_BOOT_NameTitle, 240);
  lv_obj_set_height(ui_BOOT_NameTitle, 38);
  lv_obj_set_x(ui_BOOT_NameTitle, 0);
  lv_obj_set_y(ui_BOOT_NameTitle, 40);
  lv_obj_set_align(ui_BOOT_NameTitle, LV_ALIGN_CENTER);
  lv_obj_set_style_text_align(ui_BOOT_NameTitle, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_BOOT_NameTitle, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_BOOT_NameTitle, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(ui_BOOT_NameTitle, &montserrat_16_bold, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_textarea_set_text(ui_BOOT_NameTitle, "IP Address");
}

uint8_t counter = 0;
char test[20];
void BootScreen::tick() {
  counter = (counter + 1) % 200;

  if (WiFi.status() == WL_CONNECTED) {
    lv_textarea_set_text(ui_BOOT_Name, WiFi.localIP().toString().c_str());
    lv_refr_now(NULL);
  } else {
    sprintf(test, "%02X", counter);
    strcat(test, " Unconnected");
    lv_textarea_set_text(ui_BOOT_Name, test);
    lv_refr_now(NULL);
  }
}