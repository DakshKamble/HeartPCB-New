#pragma once
#include <Arduino.h>
#include "config.h"
#include "state_manager.h"

// Button handler - processes physical button and generates events
class ButtonHandler {
public:
    void init();
    ButtonEvent update();
    
private:
    
    bool button_was_pressed = false;
    unsigned long button_press_start = 0;
    bool long_press_triggered = false;
};

// Global button handler instance
extern ButtonHandler g_button;