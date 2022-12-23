#include "gui.h"
#include <WiFi.h>
#include <ESPConnect.h>

char wifiName[32];
char wifiPassword[10];

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite img = TFT_eSprite(&tft);

/*A static or global variable to store the buffers*/
static lv_disp_draw_buf_t disp_buf;

static lv_color_t buf_1[240 * 10];
static lv_color_t buf_2[240 * 10];

lv_disp_drv_t disp_drv;
lv_indev_drv_t indev_drv;

lv_obj_t *btn1;
lv_obj_t *btn2;
lv_obj_t *screenMain;
lv_obj_t *label;

LV_IMG_DECLARE(canfuck_small)

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  uint32_t wh = w * h;

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors(&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

// TODO - Refactor this logging code to be shared
/*
char* log_history[24][34];
short log_count = 0;
short log_tail = 0;
void gui_log(WebLogLevel level, char* format, ...) {
  static char loc_buf[64];
  char * temp = loc_buf;
  int len;
  va_list arg;
  va_list copy;
  va_start(arg, format);
  va_copy(copy, arg);
  len = vsnprintf(NULL, 0, format, copy);
  va_end(copy);
  if(len >= sizeof(loc_buf)){
      temp = (char*)malloc(len+1);
      if(temp == NULL) {
          va_end(arg);
          return;
      }
  }

  vsnprintf(temp, 32, format, arg);


  va_end(arg);
  if(len >= sizeof(loc_buf)){
      free(temp);
  }
}


void gui_log_task(void *pvParameter) {
  while (true) {
    char* log_target = (char*)malloc(24 * 34)

    lv_textarea_set_text(ui_Screen1_TextArea1, "Logging Line\nLogging Line\nLogging Line");

    free(log_target);

    vTaskDelay(16 / portTICK_PERIOD_MS);
  }
}
*/

lv_obj_t *ui_BOOT;
lv_obj_t *ui_BOOT_Panel1;
lv_obj_t *ui_BOOT_TextArea1;
lv_obj_t *ui_BOOT_Image5;
lv_obj_t *ui_BOOT_Name;
lv_obj_t *ui_BOOT_NameTitle;
lv_obj_t *ui_BOOT_Status;
lv_obj_t *ui_BOOT_StatusTitle;
void gui_TEST_screen_init(void)
{
  ui_BOOT = lv_obj_create(NULL);
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

lv_obj_t *ui_WIFI;
lv_obj_t *ui_WIFI_Panel1;
lv_obj_t *ui_WIFI_Arc1;
lv_obj_t *ui_WIFI_Arc2;
lv_obj_t *ui_WIFI_Arc3;
lv_obj_t *ui_WIFI_Arc4;
lv_obj_t *ui_WIFI_Name;
lv_obj_t *ui_WIFI_NameTitle;
lv_obj_t *ui_WIFI_Password;
lv_obj_t *ui_WIFI_PasswordTitle;
void gui_WIFI_screen_init(void)
{
  ui_WIFI = lv_obj_create(NULL);
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
  lv_textarea_set_text(ui_WIFI_Name, wifiName);

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

  ui_WIFI_Password = lv_textarea_create(ui_WIFI_Panel1);
  lv_obj_set_width(ui_WIFI_Password, 240);
  lv_obj_set_height(ui_WIFI_Password, 32);
  lv_obj_set_x(ui_WIFI_Password, 0);
  lv_obj_set_y(ui_WIFI_Password, 114);
  lv_obj_set_align(ui_WIFI_Password, LV_ALIGN_CENTER);
  lv_obj_set_style_text_align(ui_WIFI_Password, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_WIFI_Password, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_WIFI_Password, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_textarea_set_text(ui_WIFI_Password, wifiPassword);

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
  lv_textarea_set_text(ui_WIFI_PasswordTitle, "Password");
}

lv_obj_t *ui_STATUS;
lv_obj_t *ui_STATUS_Background;
lv_obj_t *ui_STATUS_TopBar;
lv_obj_t *ui_STATUS_TopBarTitle;
void gui_STATUS_screen_init(void) {
  ui_STATUS = lv_obj_create(NULL);
  lv_obj_clear_flag(ui_STATUS, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

  ui_STATUS_Background = lv_obj_create(ui_STATUS);
  lv_obj_set_width(ui_STATUS_Background, 240);
  lv_obj_set_height(ui_STATUS_Background, 280);
  lv_obj_set_align(ui_STATUS_Background, LV_ALIGN_CENTER);
  lv_obj_clear_flag(ui_STATUS_Background, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
  lv_obj_set_style_radius(ui_STATUS_Background, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(ui_STATUS_Background, lv_color_hex(0x23253C), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_STATUS_Background, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_STATUS_Background, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_STATUS_TopBar = lv_obj_create(ui_STATUS);
  lv_obj_set_width(ui_STATUS_TopBar, 240);
  lv_obj_set_height(ui_STATUS_TopBar, 40);
  lv_obj_set_align(ui_STATUS_TopBar, LV_ALIGN_TOP_MID);
  lv_obj_clear_flag(ui_STATUS_TopBar, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
  lv_obj_set_style_radius(ui_STATUS_TopBar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(ui_STATUS_TopBar, lv_color_hex(0xD3D3D3), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_STATUS_TopBar, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_STATUS_TopBarTitle = lv_textarea_create(ui_STATUS);
  lv_obj_set_width(ui_STATUS_TopBarTitle, 240);
  lv_obj_set_height(ui_STATUS_TopBarTitle, 40);
  lv_obj_set_align(ui_STATUS_TopBarTitle, LV_ALIGN_TOP_MID);
  lv_textarea_set_text(ui_STATUS_TopBarTitle, LV_SYMBOL_WIFI "Central Nexus");
  lv_obj_set_style_text_color(ui_STATUS_TopBarTitle, lv_color_hex(0x202439), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_opa(ui_STATUS_TopBarTitle, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_align(ui_STATUS_TopBarTitle, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(ui_STATUS_TopBarTitle, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(ui_STATUS_TopBarTitle, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_STATUS_TopBarTitle, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(ui_STATUS_TopBarTitle, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_opa(ui_STATUS_TopBarTitle, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
}

enum class GuiScreen
{
  BOOT,
  WIFI,
  STATUS,

  NUM_OF_ITEMS
};

GuiScreen currentScreen = GuiScreen::STATUS;
short loopCount;
bool showWifi = false;
char topBarTitle[64];
void gui_change_task(void *pvParameter) {
  while (true) {
    char test[32];
    sprintf(test, "Loop %02X", loopCount);

    if (currentScreen == GuiScreen::BOOT && WiFi.status() == WL_CONNECTED) {
      lv_textarea_set_text(ui_BOOT_Name, WiFi.localIP().toString().c_str());
      lv_refr_now(NULL);
    }

    if (currentScreen == GuiScreen::STATUS && loopCount % 60 == 0) {
      if (showWifi) {
        strcpy(topBarTitle, "");
        strcat(topBarTitle, LV_SYMBOL_WIFI " ");
        strcat(topBarTitle, WiFi.localIP().toString().c_str());
        lv_textarea_set_text(ui_STATUS_TopBarTitle, topBarTitle);
        showWifi = false;
      } else {
        strcpy(topBarTitle, "");
        strcat(topBarTitle, LV_SYMBOL_WIFI " ");
        strcat(topBarTitle, ESPConnect.getSSID().c_str());
        lv_textarea_set_text(ui_STATUS_TopBarTitle, topBarTitle);
        showWifi = true;
      }
      lv_refr_now(NULL);
    }

    loopCount = (loopCount + 1) % 1000;
    vTaskDelay(33 / portTICK_PERIOD_MS);
  }
}

void gui_main_task(void *pvParameter)
{
  tft.init();
  tft.setRotation(0);

  tft.fillScreen(TFT_RED);

  lv_init();
  lv_disp_draw_buf_init(&disp_buf, buf_1, buf_2, 240 * 10);

  /*Initialize the display*/

  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 240;
  disp_drv.ver_res = 280;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &disp_buf;
  lv_disp_drv_register(&disp_drv);

  lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x003a57), LV_PART_MAIN);

  lv_disp_t *dispp = lv_disp_get_default();
  lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                            true, LV_FONT_DEFAULT);
  lv_disp_set_theme(dispp, theme);

  gui_TEST_screen_init();
  gui_WIFI_screen_init();
  gui_STATUS_screen_init();
  lv_disp_load_scr(ui_STATUS);

  xTaskCreatePinnedToCore(&gui_change_task, "gui_change", 4096*2, NULL, 0, NULL, 1);

  /*Create a white label, set its text and align it to the center*/
  /*
  lv_obj_t * label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "Hello world - 1234 - Hello world");
  lv_obj_set_style_text_color(lv_scr_act(), lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 20);
  */

  // xTaskCreatePinnedToCore(&gui_log_task, "gui_log", 4096*2, NULL, 0, NULL, 1);

  bool setLabel = false;
  while (true) {
    vTaskDelay(5 / portTICK_PERIOD_MS);
    lv_task_handler();
  }
}