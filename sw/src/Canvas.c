#include "../include/Canvas.h"

// Buffer for the canvas image. Allocated to match the actual canvas size.
static uint8_t * canvas_buf = NULL;
static size_t canvas_buf_size = 0;
static lv_obj_t * g_canvas = NULL;
static int32_t last_x = 0;
static int32_t last_y = 0;



// Callback for touch/pointer events on the canvas
static void canvas_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * canvas = lv_event_get_target(e);

    lv_point_t pos;
    lv_indev_get_point(lv_indev_get_act(), &pos);

    int32_t canvas_x = lv_obj_get_x(canvas);
    int32_t canvas_y = lv_obj_get_y(canvas);

    int32_t current_x = pos.x - canvas_x;
    int32_t current_y = pos.y - canvas_y;

    if(code == LV_EVENT_PRESSED) {

        last_x = current_x;
        last_y = current_y;
    }
    else if(code == LV_EVENT_PRESSING) {
        //printf("Last X = %d\n", last_x);
        //printf("Last Y = %d\n\n", last_y);
        //printf("Current X = %d\n", current_x);
        //printf("Last X = %d\n", last_x);

        canvas_draw_line(canvas, last_x, last_y, current_x, current_y, lv_color_black(), 2);

        last_x = current_x;
        last_y = current_y;


    }
}

lv_obj_t * canvas_init(lv_obj_t * parent, int32_t x, int32_t y, int32_t width, int32_t height)
{
    // Create canvas object
    lv_obj_t * canvas = lv_canvas_create(parent);

    // Allocate or resize the canvas buffer for the requested width/height
    size_t needed_size = (size_t)width * (size_t)height * sizeof(lv_color_t);
    if(canvas_buf_size < needed_size) {
        lv_free(canvas_buf);
        canvas_buf = lv_malloc(needed_size);
        if(canvas_buf == NULL) {
            return NULL;
        }
        canvas_buf_size = needed_size;
    }

    // Set the canvas buffer using native color format for lv_color_t
    lv_canvas_set_buffer(canvas, canvas_buf, width, height, LV_COLOR_FORMAT_NATIVE);

    // Set position and size
    lv_obj_set_pos(canvas, x, y);
    lv_obj_set_size(canvas, width, height);

    // Ensure the canvas can receive pointer events
    lv_obj_add_flag(canvas, LV_OBJ_FLAG_CLICKABLE);

    // Make background black
    canvas_clear(canvas);

    // Disable scrolling and set other properties
    lv_obj_clear_flag(canvas, LV_OBJ_FLAG_SCROLLABLE);

    // Remove border/styling
    lv_obj_set_style_border_width(canvas, 0, 0);

    // Add event callback for touch/mouse input
    lv_obj_add_event_cb(canvas, canvas_event_cb, LV_EVENT_ALL, NULL);

    g_canvas = canvas;
    return canvas;
}

void canvas_clear(lv_obj_t * canvas)
{
    if(!canvas) return;

    // Get canvas buffer
    lv_canvas_fill_bg(canvas, lv_color_white(), LV_OPA_COVER);
}

void canvas_draw_line(lv_obj_t * canvas, int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t color, int32_t width)
{
    if(!canvas) return;

    // Initialize layer for drawing
    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    // Setup line drawing descriptor
    lv_draw_line_dsc_t line_dsc;
    lv_draw_line_dsc_init(&line_dsc);
    line_dsc.color = color;
    line_dsc.width = width;
    line_dsc.round_end = 1;  // Round line ends
    line_dsc.round_start = 1; // Round line start

    // Draw the line
    line_dsc.p1.x = x1;
    line_dsc.p1.y = y1;
    line_dsc.p2.x = x2;
    line_dsc.p2.y = y2;
    lv_draw_line(&layer, &line_dsc);

    // Finish drawing and flush to canvas
    lv_canvas_finish_layer(canvas, &layer);
}
