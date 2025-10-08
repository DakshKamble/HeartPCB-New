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
    
    Serial.println("All systems initialized!");
    Serial.println("======================================\n");
}

void loop() {
    // Update state machine (handles auto-transitions)
    g_state.update();
    
    // Process button input and handle events
    if (!g_state.is_splash()) {
        ButtonEvent event = g_button.update();
        g_state.handle_button_event(event);
    }
    
    // Update LED animations
    g_leds.update();
    
    // Update battery monitor
    g_battery.update();
    
    // Update battery LED - only show when charging
    if (g_battery.is_charging()) {
        g_battery_led.set_charging_animation(g_battery.get_percentage());
    } else {
        g_battery_led.turn_off();
    }
    
    // Print battery status when in Battery screen
    static bool was_in_battery = false;
    if (g_state.is_battery() && !was_in_battery) {
        g_battery.print_status();
        was_in_battery = true;
    } else if (!g_state.is_battery()) {
        was_in_battery = false;
    }
    
    // Render current screen
    g_renderer.render();
    
    delay(MAIN_LOOP_DELAY_MS);
}