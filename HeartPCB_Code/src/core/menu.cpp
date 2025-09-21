#include "menu.h"

//Menu variables (private to this module)
static int current_screen = 0;

//Menu items
char menu_items[MAX_ITEMS][MAX_ITEM_LENGTH] = {
    {"Propose"},
    {"Oscilloscope"}
};

//Initialize menu system
void init_menu() {
    current_screen = 0;
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