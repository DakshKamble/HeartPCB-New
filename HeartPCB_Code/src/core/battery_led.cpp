#include "battery_led.h"

// Global instance
BatteryLED g_battery_led;

void BatteryLED::init() {
    // Initialize the built-in RGB LED
    builtin_led = Adafruit_NeoPixel(1, RGB_BUILTIN, NEO_GRB + NEO_KHZ800);
    
    // Set up the power pin for the RGB LED
    pinMode(PIN_RGB_EN, OUTPUT);
    digitalWrite(PIN_RGB_EN, HIGH); // Turn on power to the LED
    
    builtin_led.begin();
    builtin_led.setBrightness(BATTERY_LED_BRIGHTNESS); // Use configurable brightness
    builtin_led.clear();
    builtin_led.show();
    
    is_initialized = true;
    
    Serial.println("Battery LED initialized");
}

void BatteryLED::update_battery_color(int battery_percentage) {
    if (!is_initialized) return;
    
    uint32_t color = get_battery_color(battery_percentage);
    builtin_led.setPixelColor(0, color);
    builtin_led.show();
}

void BatteryLED::set_charging_animation(int battery_percentage) {
    if (!is_initialized) return;
    
    unsigned long now = millis();
    if (now - last_charging_update < 50) { // Update every 50ms for smoother animation
        return;
    }
    last_charging_update = now;
    
    // Get the base color for current battery percentage
    uint32_t base_color = get_battery_color(battery_percentage);
    
    // Extract RGB components
    uint8_t r = (base_color >> 16) & 0xFF;
    uint8_t g = (base_color >> 8) & 0xFF;
    uint8_t b = base_color & 0xFF;
    
    // Create pulsing effect with enhanced brightness
    float pulse = (sin(charging_step * 0.15) + 1.0) / 2.0; // 0.0 to 1.0
    float brightness_multiplier = 0.3 + (pulse * 0.7); // Range from 30% to 100% brightness
    
    // Apply pulsing to the battery percentage color
    uint8_t pulsed_r = (uint8_t)(r * brightness_multiplier);
    uint8_t pulsed_g = (uint8_t)(g * brightness_multiplier);
    uint8_t pulsed_b = (uint8_t)(b * brightness_multiplier);
    
    builtin_led.setPixelColor(0, builtin_led.Color(pulsed_r, pulsed_g, pulsed_b));
    builtin_led.show();
    
    charging_step++;
}

void BatteryLED::turn_off() {
    if (!is_initialized) return;
    
    builtin_led.clear();
    builtin_led.show();
}

uint32_t BatteryLED::get_battery_color(int percentage) {
    // Clamp percentage to 0-100
    percentage = constrain(percentage, 0, 100);
    
    // Define color points: Red (0%) -> Yellow (50%) -> Green (100%)
    uint32_t red = builtin_led.Color(255, 0, 0);      // 0%
    uint32_t yellow = builtin_led.Color(255, 255, 0); // 50%
    uint32_t green = builtin_led.Color(0, 255, 0);    // 100%
    
    if (percentage <= 50) {
        // Interpolate between red and yellow (0% to 50%)
        float ratio = percentage / 50.0;
        return interpolate_color(red, yellow, ratio);
    } else {
        // Interpolate between yellow and green (50% to 100%)
        float ratio = (percentage - 50) / 50.0;
        return interpolate_color(yellow, green, ratio);
    }
}

uint32_t BatteryLED::interpolate_color(uint32_t color1, uint32_t color2, float ratio) {
    // Extract RGB components from color1
    uint8_t r1 = (color1 >> 16) & 0xFF;
    uint8_t g1 = (color1 >> 8) & 0xFF;
    uint8_t b1 = color1 & 0xFF;
    
    // Extract RGB components from color2
    uint8_t r2 = (color2 >> 16) & 0xFF;
    uint8_t g2 = (color2 >> 8) & 0xFF;
    uint8_t b2 = color2 & 0xFF;
    
    // Interpolate each component
    uint8_t r = r1 + (r2 - r1) * ratio;
    uint8_t g = g1 + (g2 - g1) * ratio;
    uint8_t b = b1 + (b2 - b1) * ratio;
    
    return builtin_led.Color(r, g, b);
}
