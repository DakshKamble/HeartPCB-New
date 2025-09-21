#include "button_handler.h"

//Button handler variables (private to this module)
static int button_clicked = 0;
static int item_selected = 0;
static unsigned long last_button_time = 0;
static const unsigned long DEBOUNCE_DELAY = 50; // 50ms debounce

//Initialize button handler
void init_button_handler() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

//Handle button input with debouncing
void handle_button_input() {
    if (digitalRead(BUTTON_PIN) == LOW && button_clicked == 0) {
        unsigned long current_time = millis();
        if (current_time - last_button_time > DEBOUNCE_DELAY) {
            item_selected = (item_selected + 1) % 2; // Cycle between 0 and 1
            button_clicked = 1;
            last_button_time = current_time;
        }
    }
    
    if (digitalRead(BUTTON_PIN) == HIGH && button_clicked == 1) {
        button_clicked = 0;
    }
}

//Check if button is currently pressed
bool is_button_pressed() {
    return digitalRead(BUTTON_PIN) == LOW;
}

//Get currently selected menu item
int get_selected_item() {
    return item_selected;
}

//Reset button state
void reset_button_state() {
    button_clicked = 0;
    item_selected = 0;
}