#include "../include/Back_button.h"

static lv_style_t style_btn;
static lv_style_t style_btn_pressed;
static lv_color_filter_dsc_t color_filter;
static lv_obj_t* label;
static back_callback_t back_cb = NULL; // back_cb is a function pointer that stores the address of a function

static lv_color_t darken_cb(const lv_color_filter_dsc_t * dsc, lv_color_t color, lv_opa_t opa)
{
    LV_UNUSED(dsc); // Macro that simply tells the compiler: “I know this parameter exists, but I’m not using it.”
    return lv_color_darken(color, opa); // Take the original color and darken it by the opacity amount
}

static void make_btn_styles(void)
{
    lv_style_init(&style_btn); // Creates an empty sytle object

    lv_style_set_radius(&style_btn, 10); // Rounded corners

    // opacity (opa) is a value from 0 to 255:
    // 0 means fully transparent (invisible)
    // 255 means fully opaque (solid)

    lv_style_set_bg_opa(&style_btn, LV_OPA_COVER); // Background is fully opague / solid

    lv_style_set_bg_color(&style_btn, lv_palette_lighten(LV_PALETTE_GREY, 3)); // Sets a light gray base color

    // Adds a vertical gradient from light grey to darker grey.
    lv_style_set_bg_grad_color(&style_btn, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_bg_grad_dir(&style_btn, LV_GRAD_DIR_VER);

    // Border
    lv_style_set_border_color(&style_btn, lv_color_black());
    lv_style_set_border_opa(&style_btn, LV_OPA_20);
    lv_style_set_border_width(&style_btn, 2);

    lv_style_set_text_color(&style_btn, lv_color_black());

    // Pressed style (darken filter)
    // When this style is active, run darken_cb() on every pixel
    lv_color_filter_dsc_init(&color_filter, darken_cb);

    // Empty style object for pressed state
    lv_style_init(&style_btn_pressed);

    // Apply the darken filter with 20% opacity
    // To all colors in the button (background, border, text, images)
    lv_style_set_color_filter_dsc(&style_btn_pressed, &color_filter);
    lv_style_set_color_filter_opa(&style_btn_pressed, LV_OPA_20);
}

static void btn_event_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_PRESSED) {
        if(back_cb != NULL) { // If callback exists, call it
            back_cb();
        }

    }
}

void make_Back_button(const int width, const int height, back_callback_t cb)
{
    back_cb = cb;

    make_btn_styles();

    lv_obj_t* btn = lv_btn_create(lv_scr_act()); // Makes button
    lv_obj_remove_style_all(btn);                // Removes all styles cause were making our own

    lv_obj_t* img = lv_img_create(btn);          // Parent is the button
    lv_img_set_src(img, &Back_button_logo);          // Image appears on top of button

    lv_obj_center(img);                          // Centers image

    lv_obj_set_pos(btn, 0, 0);
    lv_obj_set_size(btn, width, height);

    lv_obj_add_style(btn, &style_btn, 0);        // Apply's style to buttons default state(0)

    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL); // Button callback for events

    label = lv_label_create(btn);     // Make label for button
    lv_label_set_text(label, "");
    lv_obj_center(label);             // Centers object inside parent object(btn)
}
