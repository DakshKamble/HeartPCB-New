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

//Screen state identifiers
enum ScreenState {
    STATE_SPLASH = 0,    // Showing splash screen
    STATE_MENU = 1,      // Browsing menu items
    STATE_SELECTED = 2   // Inside a selected menu item
};

//Propose mode sub-states
enum ProposeState {
    PROPOSE_WAITING = 0,    // Showing "Press to Accept"
    PROPOSE_DISPLAYING = 1  // Showing "I Love You" with flashing heart
};

//Menu functions
void init_menu();
int get_current_screen();
void set_current_screen(int screen);
const char* get_menu_item(int index);
const char* get_current_screen_name();

//Screen state functions
int get_screen_state();
void set_screen_state(int state);
void enter_selected_item();
void exit_to_menu();
bool is_in_menu();
bool is_item_selected();
bool is_showing_splash();
void exit_splash_to_menu();
bool should_exit_splash();

//Propose mode functions
int get_propose_state();
void set_propose_state(int state);
void handle_propose_button_press();
bool should_return_to_menu();
void reset_propose_timer();

//Splash screen functions
const char* get_splash_text();
int get_splash_text_x();
int get_splash_text_y();