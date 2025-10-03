#include "menu.h"

//Menu variables (private to this module)
static int current_screen = 0;
static int screen_state = STATE_SPLASH; // Start with splash screen
static int propose_state = PROPOSE_WAITING;
static unsigned long propose_display_start = 0;
static unsigned long splash_start_time = 0;
static const unsigned long PROPOSE_DISPLAY_TIME = 10000; // 10 seconds
static const unsigned long SPLASH_DISPLAY_TIME = 2000;   // 2 seconds

//Splash screen configuration (easily changeable)
static const char* SPLASH_TEXT = "Hi Daksh";
static const int SPLASH_TEXT_X = 2;
static const int SPLASH_TEXT_Y = 25;

//Menu items
char menu_items[MAX_ITEMS][MAX_ITEM_LENGTH] = {
    {"Propose"},
    {"Oscilloscope"}
};

//Initialize menu system
void init_menu() {
    current_screen = 0;
    screen_state = STATE_SPLASH;
    propose_state = PROPOSE_WAITING;
    propose_display_start = 0;
    splash_start_time = millis();
}

//Get current screen
int get_current_screen() {
    return current_screen;
}

//Set current screen
void set_current_screen(int screen) {
    if (screen >= 0 && screen < MAX_ITEMS) {
        current_screen = screen;
    }
}

//Get menu item by index
const char* get_menu_item(int index) {
    if (index >= 0 && index < MAX_ITEMS) {
        return menu_items[index];
    }
    return "Invalid";
}

//Get current screen name
const char* get_current_screen_name() {
    return get_menu_item(current_screen);
}

//Screen state functions
int get_screen_state() {
    return screen_state;
}

void set_screen_state(int state) {
    if (state == STATE_MENU || state == STATE_SELECTED) {
        screen_state = state;
    }
}

void enter_selected_item() {
    screen_state = STATE_SELECTED;
}

void exit_to_menu() {
    screen_state = STATE_MENU;
}

bool is_in_menu() {
    return screen_state == STATE_MENU;
}

bool is_item_selected() {
    return screen_state == STATE_SELECTED;
}

bool is_showing_splash() {
    return screen_state == STATE_SPLASH;
}

void exit_splash_to_menu() {
    screen_state = STATE_MENU;
}

bool should_exit_splash() {
    return (millis() - splash_start_time) >= SPLASH_DISPLAY_TIME;
}

//Get splash screen configuration
const char* get_splash_text() {
    return SPLASH_TEXT;
}

int get_splash_text_x() {
    return SPLASH_TEXT_X;
}

int get_splash_text_y() {
    return SPLASH_TEXT_Y;
}

//Propose mode functions
int get_propose_state() {
    return propose_state;
}

void set_propose_state(int state) {
    if (state == PROPOSE_WAITING || state == PROPOSE_DISPLAYING) {
        propose_state = state;
        if (state == PROPOSE_DISPLAYING) {
            propose_display_start = millis();
        }
    }
}

void handle_propose_button_press() {
    if (propose_state == PROPOSE_WAITING) {
        // Move to displaying "I Love You"
        set_propose_state(PROPOSE_DISPLAYING);
    }
    // If already displaying, button press does nothing
}

bool should_return_to_menu() {
    if (propose_state == PROPOSE_DISPLAYING) {
        return (millis() - propose_display_start) >= PROPOSE_DISPLAY_TIME;
    }
    return false;
}

void reset_propose_timer() {
    propose_display_start = millis();
}