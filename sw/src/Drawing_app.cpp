#include "../include/Drawing_app.h"
#include "../include/Canvas.h"

static void make_Drawing_app();
static void ta_event_cb(lv_event_t * e);
static void back_pressed(void);

static lv_style_t style_btn;
static lv_style_t style_btn_pressed;
static lv_color_filter_dsc_t color_filter;
static lv_obj_t * label;
static lv_obj_t * text_area;


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
    // Returns the object that triggered the event which is the button in this case
    lv_obj_t * btn = (lv_obj_t *) lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);    // Gets the event

    // Apply darkening
    if(code == LV_EVENT_PRESSED) {
        lv_obj_clean(lv_scr_act());

        lv_obj_set_style_bg_color(lv_scr_act(), lv_color_white(), LV_PART_MAIN);

        make_Drawing_app();
        make_Back_button(Back_button_width, Back_button_height, back_pressed);
    }

}

void make_Drawing_button(void)
{
    make_btn_styles();

    lv_obj_t* btn = lv_btn_create(lv_scr_act()); // Makes button
    lv_obj_remove_style_all(btn);                // Removes all styles cause were making our own

    lv_obj_t* img = lv_img_create(btn);          // Parent is the button
    lv_img_set_src(img, &Drawing_logo);          // Image appears on top of button

    lv_obj_center(img);                          // Centers image

    lv_obj_set_pos(btn, 10, 90);
    lv_obj_set_size(btn, button_width, button_height);

    lv_obj_add_style(btn, &style_btn, 0);        // Apply's style to buttons default state(0)

    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL); // Button callback for events

    label = lv_label_create(btn);     // Make label for button
    lv_label_set_text(label, "");
    lv_obj_center(label);             // Centers object inside parent object(btn)
}

static void make_Drawing_app()
{
    lv_obj_t * parent = lv_screen_active();

    // -------------------- Title Bar ----------------------- //
    lv_obj_t * title_bar = lv_obj_create(parent); // Creates a new LVGL object inside parent
    lv_obj_set_size(title_bar, LV_PCT(100), 30);
    lv_obj_align(title_bar, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_clear_flag(title_bar, LV_OBJ_FLAG_SCROLLABLE);

    //Now add text:
    lv_obj_t * title_label = lv_label_create(title_bar);
    lv_label_set_text(title_label, "Drawing");
    lv_obj_center(title_label);

    // Make Bar have no rounded corners
    lv_obj_set_style_radius(title_bar, 0, 0);
    lv_obj_set_style_bg_color(title_bar, lv_color_white(), 0);
    lv_obj_set_style_text_color(title_bar, lv_color_black(), 0);

    // Remove border
    lv_obj_set_style_border_width(title_bar, 0, LV_PART_MAIN);

    // -------------------- Canvas for Drawing ----------------------- //
    // Get screen dimensions to size canvas appropriately
    int32_t screen_width = lv_disp_get_hor_res(NULL);
    int32_t screen_height = lv_disp_get_ver_res(NULL);

    // Canvas starts below title bar (30px) and takes up remaining space
    int32_t canvas_y = 30;
    int32_t canvas_height = screen_height - canvas_y;

    // Initialize the canvas with touchscreen drawing capability
    lv_obj_t * canvas = canvas_init(parent, 0, canvas_y, screen_width, canvas_height);
}

static void back_pressed(void)
{
    lv_obj_clean(lv_scr_act());

    lv_phone_gui_widgets();
}

