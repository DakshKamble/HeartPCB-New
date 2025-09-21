#pragma once
#include <Arduino.h>

//Button handler constants
#define BUTTON_PIN D5

//Button handler functions
void init_button_handler();
void handle_button_input();
bool is_button_pressed();

//Button state getters
int get_selected_item();
void reset_button_state();