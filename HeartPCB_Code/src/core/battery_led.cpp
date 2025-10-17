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

void BatteryLED::set_love_animation() {
    if (!is_initialized) return;
    
    unsigned long now = millis();
    if (now - last_charging_update < 80) { // Update every 80ms for gentle pulse
        return;
    }
    last_charging_update = now;
    
    // Gentle pink pulsing animation for love
    float pulse = (sin(charging_step * 0.12) + 1.0) / 2.0; // 0.0 to 1.0
    uint8_t brightness = (uint8_t)(pulse * 200 + 55); // Range from 55 to 255 for gentle pulse
    
    // Pink color (hue around 330 degrees)
    builtin_led.setPixelColor(0, builtin_led.Color(brightness, brightness/4, brightness/2)); // Pink RGB ratio
    builtin_led.show();
    
    charging_step++;
}

void BatteryLED::set_morse_code_animation() {
    if (!is_initialized) return;
    
    unsigned long now = millis();
    
    // Fast Morse code timing (in milliseconds)
    const int DOT_DURATION = 80;      // Short flash
    const int DASH_DURATION = 240;    // Long flash (3x dot)
    const int SYMBOL_GAP = 80;        // Gap between dots/dashes
    const int LETTER_GAP = 240;       // Gap between letters (3x dot)
    const int WORD_GAP = 560;         // Gap between words (7x dot)
    
    // "I LOVE YOU GARGI" in Morse code
    // I: ..    L: .-..    O: ---    V: ...-    E: .
    // Y: -.--    O: ---    U: ..-
    // G: --.    A: .-    R: .-.    G: --.    I: ..
    static const char* morse_message = ".. .-.. --- ...- . -.-- --- ..- --. .- .-. --. ..";
    static int message_pos = 0;
    static unsigned long last_update = 0;
    static bool led_on = false;
    static unsigned long current_duration = 0;
    
    // Initialize timing on first call or reset
    if (last_update == 0) {
        last_update = now;
        message_pos = 0;
        led_on = false;
    }
    
    // Check if current timing period is complete
    if (now - last_update >= current_duration) {
        last_update = now;
        
        if (led_on) {
            // Turn off LED and set gap duration
            builtin_led.clear();
            builtin_led.show();
            led_on = false;
            
            // Move to next character
            message_pos++;
            
            // Determine gap duration
            if (message_pos >= strlen(morse_message)) {
                // End of message - restart with word gap
                message_pos = 0;
                current_duration = WORD_GAP;
            } else {
                char current_char = morse_message[message_pos];
                if (current_char == ' ') {
                    current_duration = LETTER_GAP;
                    message_pos++; // Skip the space
                    if (message_pos >= strlen(morse_message)) {
                        message_pos = 0;
                        current_duration = WORD_GAP;
                    }
                } else {
                    current_duration = SYMBOL_GAP;
                }
            }
        } else {
            // Turn on LED with appropriate duration
            if (message_pos < strlen(morse_message)) {
                char current_char = morse_message[message_pos];
                
                if (current_char == '.') {
                    // Dot - short pink flash
                    builtin_led.setPixelColor(0, builtin_led.Color(255, 64, 128)); // Bright pink
                    current_duration = DOT_DURATION;
                    led_on = true;
                } else if (current_char == '-') {
                    // Dash - long pink flash
                    builtin_led.setPixelColor(0, builtin_led.Color(255, 64, 128)); // Bright pink
                    current_duration = DASH_DURATION;
                    led_on = true;
                } else if (current_char == ' ') {
                    // Space - skip and set letter gap
                    current_duration = LETTER_GAP;
                    message_pos++;
                    if (message_pos >= strlen(morse_message)) {
                        message_pos = 0;
                        current_duration = WORD_GAP;
                    }
                }
                builtin_led.show();
            }
        }
    }
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
