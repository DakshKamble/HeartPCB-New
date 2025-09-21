#include "button_handler.h"
#include "menu.h"

//Button handler variables (private to this module)
static int item_selected = 0;
static unsigned long button_press_start = 0;
static bool button_was_pressed = false;
static bool long_press_triggered = false;
static bool long_press_active = false;

static const unsigned long DEBOUNCE_DELAY = 50; // 50ms debounce
static const unsigned long LONG_PRESS_TIME = 500; // 3 seconds for long press

//Initialize button handler
void init_button_handler() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

//Handle button input with new press/release logic
void handle_button_input() {
    bool button_currently_pressed = (digitalRead(BUTTON_PIN) == LOW);
    unsigned long current_time = millis();
    
    // Button just pressed (falling edge)
    if (button_currently_pressed && !button_was_pressed) {
        if (current_time - button_press_start > DEBOUNCE_DELAY) {
            button_press_start = current_time;
            button_was_pressed = true;
            long_press_triggered = false;
            long_press_active = false;
        }
    }
    
    // Button currently held down
    if (button_currently_pressed && button_was_pressed) {
        unsigned long press_duration = current_time - button_press_start;
        
        // Check for long press (3 seconds)
        if (press_duration >= LONG_PRESS_TIME && !long_press_triggered) {
            long_press_triggered = true;
            long_press_active = true;
        }
    }
    
    // Button just released (rising edge)
    if (!button_currently_pressed && button_was_pressed) {
        unsigned long press_duration = current_time - button_press_start;
        button_was_pressed = false;
        long_press_active = false;
        
        // Handle button release based on press duration and current state
        if (press_duration < LONG_PRESS_TIME) {
            // Short press
            if (is_in_menu()) {
                // In menu: change menu item
                item_selected = (item_selected + 1) % 2; // Cycle between 0 and 1
            } else if (is_item_selected()) {
                // In selection mode
                if (get_current_screen() == SCREEN_PROPOSE) {
                    // Handle propose mode button press
                    handle_propose_button_press();
                } else {
                    // Other modes: exit back to menu
                    exit_to_menu();
                }
            }
        } else {
            // Long press release
            if (is_in_menu()) {
                // In menu: enter selected item
                set_current_screen(item_selected);
                enter_selected_item();
                // Reset propose state when entering propose mode
                if (item_selected == SCREEN_PROPOSE) {
                    set_propose_state(PROPOSE_WAITING);
                }
            }
            // If already in selection, long press does nothing on release
        }
        
        long_press_triggered = false;
    }
}

//Check if button is currently pressed
bool is_button_pressed() {
    return digitalRead(BUTTON_PIN) == LOW;
}

//Check if long press is currently active (for display inversion)
bool is_long_press_active() {
    return long_press_active;
}

//Get currently selected menu item
int get_selected_item() {
    return item_selected;
}

//Reset button state
void reset_button_state() {
    item_selected = 0;
    button_press_start = 0;
    button_was_pressed = false;
    long_press_triggered = false;
    long_press_active = false;
}