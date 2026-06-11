#pragma once

#include "../lvgl.h"
#include <stdint.h>
#include <stdio.h>

/**
 * @brief Initialize the drawing canvas
 * @param parent Parent object where the canvas will be created
 * @param x X position of the canvas
 * @param y Y position of the canvas
 * @param width Width of the canvas
 * @param height Height of the canvas
 * @return Pointer to the created canvas object
 */
lv_obj_t * canvas_init(lv_obj_t * parent, int32_t x, int32_t y, int32_t width, int32_t height);

/**
 * @brief Clear the canvas (make it completely black)
 * @param canvas Pointer to the canvas object
 */
void canvas_clear(lv_obj_t * canvas);

/**
 * @brief Draw a line on the canvas
 * @param canvas Pointer to the canvas object
 * @param x1 Start X coordinate
 * @param y1 Start Y coordinate
 * @param x2 End X coordinate
 * @param y2 End Y coordinate
 * @param color Color of the line
 * @param width Width of the line
 */
void canvas_draw_line(lv_obj_t * canvas, int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t color, int32_t width);
