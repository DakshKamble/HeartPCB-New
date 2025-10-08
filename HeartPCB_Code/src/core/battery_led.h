#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"

// Battery LED controller for built-in RGB LED
class BatteryLED {
public:
    void init();
    void update_battery_color(int battery_percentage);
    void set_charging_animation(int battery_percentage);
    void turn_off();
    
private:
    Adafruit_NeoPixel builtin_led;
    bool is_initialized = false;
    unsigned long last_charging_update = 0;
    uint8_t charging_step = 0;
    
    // Helper functions
    uint32_t get_battery_color(int percentage);
    uint32_t interpolate_color(uint32_t color1, uint32_t color2, float ratio);
};

// Global battery LED instance
extern BatteryLED g_battery_led;
