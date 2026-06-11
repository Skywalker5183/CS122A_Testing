#pragma once

#include "../lvgl.h"
#include "../../assets/Back_button_logo.h"
#include "Phone_gui.h"

static const int Back_button_width = 30;
static const int Back_button_height = 30;

typedef void (*back_callback_t)(void);

void make_Back_button(int w, int h, back_callback_t cb);
