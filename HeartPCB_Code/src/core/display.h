#pragma once
#include <u8g2lib.h>
#include "bitmaps.h"

//Display functions
void init_display();
void clear_display();
void update_display();
void draw_main_screen(int selected_item);
void draw_propose_screen();
void draw_oscilloscope_screen();
void draw_battery_icon();
void draw_status_icons();
void set_display_invert(bool inverted);
