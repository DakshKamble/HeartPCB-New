#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <u8g2lib.h>
#include "core/config.h"
#include "core/state_manager.h"
#include "core/button_handler.h"
#include "core/screen_renderer.h"
#include "core/led_controller.h"
#include "core/battery_monitor.h"
#include "core/battery_led.h"
#include "core/dac_controller.h"

// Display object
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);

void setup() {
    // Initialize Serial for debugging
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n\n========== HeartPCB Starting ==========");
    
    g_state.init();
    g_button.init();
    g_renderer.init(&u8g2);
    g_leds.init();
    g_battery.init();
    g_battery_led.init();
    g_dac.init();
    
    Serial.println("All systems initialized!");
    Serial.println("======================================\n");
}

void loop() {
    // If DAC is active (oscilloscope mode), prioritize DAC updates
    if (g_dac.is_active()) {
        // High-priority DAC update loop - similar to your standalone code
        for (int i = 0; i < 100; i++) {  // Process 100 points quickly
            g_dac.update();
            delayMicroseconds(20);  // Exact timing from your working code
        }
        
        // Quick updates for other systems (less frequent)
        static unsigned long last_system_update = 0;
        if (millis() - last_system_update > 50) {  // Update every 50ms
            g_state.update();
            if (!g_state.is_splash()) {
                ButtonEvent event = g_button.update();
                g_state.handle_button_event(event);
            }
            g_renderer.render();
            last_system_update = millis();
        }
    } else {
        // Normal operation when DAC is not active
        g_state.update();
        
        if (!g_state.is_splash()) {
            ButtonEvent event = g_button.update();
            g_state.handle_button_event(event);
        }
        
        g_leds.update();
        g_battery.update();
        
        // Update battery LED
        if (g_battery.is_charging()) {
            g_battery_led.set_charging_animation(g_battery.get_percentage());
        } else if (g_state.is_propose_displaying()) {
            g_battery_led.set_morse_code_animation();
        } else {
            g_battery_led.turn_off();
        }
        
        // Battery status printing
        static bool was_in_battery = false;
        if (g_state.is_battery() && !was_in_battery) {
            g_battery.print_status();
            was_in_battery = true;
        } else if (!g_state.is_battery()) {
            was_in_battery = false;
        }
        
        g_renderer.render();
        delay(MAIN_LOOP_DELAY_MS);
    }
}