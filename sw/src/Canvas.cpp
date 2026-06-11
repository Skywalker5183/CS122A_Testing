#include "Canvas.h"
#include <stdio.h>
#include <stdlib.h>
#include <cmath> // For std::abs


// Kept for system compatibility layers
static uint8_t * canvas_buf = nullptr;
static size_t canvas_buf_size = 0;
static lv_obj_t * g_canvas = nullptr;


// Track the last position to calculate distance thresholds
static int32_t last_local_x = -1;
static int32_t last_local_y = -1;


static void canvas_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * drawing_area = static_cast<lv_obj_t *>(lv_event_get_target(e));


    // Handle both initial touch AND active dragging
    if(code == LV_EVENT_PRESSED || code == LV_EVENT_PRESSING) { 
        // Stop event bubbling to protect against duplication/mirroring rules
        lv_event_stop_bubbling(e);


        lv_point_t pos;
        lv_indev_get_point(lv_indev_get_act(), &pos);


        // Sanity safety check to ignore raw driver boundary anomalies
        if (pos.x <= 0 || pos.y <= 0) return;


        // Convert absolute touchscreen space directly to parent-local coordinates
        int32_t local_x = pos.x - lv_obj_get_x(drawing_area);
        int32_t local_y = pos.y - lv_obj_get_y(drawing_area);


        // If your finger has barely moved, skip processing to save your CPU thread performance
        if (code == LV_EVENT_PRESSING && last_local_x != -1) {
            int32_t diff_x = std::abs(local_x - last_local_x);
            int32_t diff_y = std::abs(local_y - last_local_y);
            
            // If the finger moved less than 2 pixels, skip creating a duplicate dot
            if (diff_x < 2 && diff_y < 2) {
                return;
            }
        }


        // Cache the new coordinates as the historic trail point
        last_local_x = local_x;
        last_local_y = local_y;


        printf("DRAWING ACTIVE -> Local Coordinate: %d, %d\n", local_x, local_y);


        // Spawn a native widget segment directly at the touch coordinates
        lv_obj_t * dot = lv_obj_create(drawing_area);
        
        // Dot thickness (8x8 pixels works beautifully to bridge gaps into a solid line)
        lv_obj_set_size(dot, 8, 8); 
        lv_obj_set_pos(dot, local_x - 4, local_y - 4); // Centers the dot perfectly under your finger


        // Style formatting: Pure solid black block
        lv_obj_set_style_bg_color(dot, lv_color_black(), 0);
        lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);
        lv_obj_set_style_border_width(dot, 0, 0); 


        // Block scrolling handlers so the layout grid stays completely static
        lv_obj_clear_flag(dot, LV_OBJ_FLAG_SCROLLABLE);
    }
    
    // When the user lifts their finger, reset history tracking
    else if(code == LV_EVENT_RELEASED) {
        last_local_x = -1;
        last_local_y = -1;
    }
}


lv_obj_t * canvas_init(lv_obj_t * parent, int32_t x, int32_t y, int32_t width, int32_t height)
{
    // Initialize a stable widget layout panel container instead of a memory array
    lv_obj_t * drawing_area = lv_obj_create(parent);
    lv_obj_set_pos(drawing_area, x, y);
    lv_obj_set_size(drawing_area, width, height);


    // Make the background canvas area bright white
    lv_obj_set_style_bg_color(drawing_area, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(drawing_area, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(drawing_area, 0, 0);
    lv_obj_clear_flag(drawing_area, LV_OBJ_FLAG_SCROLLABLE);


    // Turn on explicit touch click/drag properties
    lv_obj_add_flag(drawing_area, LV_OBJ_FLAG_CLICKABLE);


    // Reset tracking coordinates upon initial setup sequence
    last_local_x = -1;
    last_local_y = -1;


    // Bind event callback hooks
    lv_obj_add_event_cb(drawing_area, canvas_event_cb, LV_EVENT_ALL, nullptr);


    g_canvas = drawing_area;
    return drawing_area;
}


void canvas_clear(lv_obj_t * canvas)
{
    // Cleans out all spawned trail widget dots instantly when exiting back to the home menu layout
    if(canvas) {
        lv_obj_clean(canvas);
    }
    last_local_x = -1;
    last_local_y = -1;
}


// Kept here to satisfy compilation headers without breaking external linker scripts
void canvas_draw_line(lv_obj_t * canvas, int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t color, int32_t width)
{
    (void)canvas; (void)x1; (void)y1; (void)x2; (void)y2; (void)color; (void)width;
}



