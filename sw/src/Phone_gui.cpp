#include "Phone_gui.h"
//#include "../assets/Weather_app_background.h"

#define HOME_BACKGROUND_COLOR 0x0F40


void lv_phone_gui_widgets(void) {
    // Sets the background color of the screen
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(HOME_BACKGROUND_COLOR), LV_PART_MAIN);

    //make_weather_button();
    make_Notepad_button();
    make_Texting_button();
    make_Drawing_button();
}
