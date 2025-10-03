#include "button_handler.h"

// Global instance
ButtonHandler g_button;

void ButtonHandler::init() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

ButtonEvent ButtonHandler::update() {
    bool button_currently_pressed = (digitalRead(BUTTON_PIN) == LOW);
    unsigned long current_time = millis();
    ButtonEvent event = BTN_NONE;
    
    // Button just pressed (falling edge)
    if (button_currently_pressed && !button_was_pressed) {
        if (current_time - button_press_start > BUTTON_DEBOUNCE_MS) {
            button_press_start = current_time;
            button_was_pressed = true;
            long_press_triggered = false;
        }
    }
    
    // Button currently held down
    if (button_currently_pressed && button_was_pressed) {
        unsigned long press_duration = current_time - button_press_start;
        
        // Check for long press - just set flag and show visual feedback
        if (press_duration >= LONG_PRESS_THRESHOLD_MS) {
            long_press_triggered = true;
            event = BTN_LONG_PRESS_ACTIVE;
        }
    }
    
    // Button just released (rising edge)
    if (!button_currently_pressed && button_was_pressed) {
        unsigned long press_duration = current_time - button_press_start;
        button_was_pressed = false;
        
        // Determine event based on press duration
        if (long_press_triggered) {
            event = BTN_LONG_PRESS_RELEASE;  // Long press released
        } else {
            event = BTN_SHORT_PRESS;  // Short press
        }
        
        long_press_triggered = false;
    }
    
    return event;
}