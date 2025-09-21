#pragma once
#include <Arduino.h>

//Menu constants
const int MAX_ITEMS = 2;
const int MAX_ITEM_LENGTH = 20;

//Menu data
extern char menu_items[MAX_ITEMS][MAX_ITEM_LENGTH];

//Menu screen identifiers
enum MenuScreen {
    SCREEN_PROPOSE = 0,
    SCREEN_OSCILLOSCOPE = 1
};

//Menu functions
void init_menu();
int get_current_screen();
void set_current_screen(int screen);
const char* get_menu_item(int index);
const char* get_current_screen_name();