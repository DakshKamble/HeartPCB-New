#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <u8g2lib.h>
#include "core/config.h"
#include "core/state_manager.h"
#include "core/button_handler.h"
#include "core/screen_renderer.h"
#include "core/led_controller.h"

// Display object
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);

void setup() {
    g_state.init();
    g_button.init();
    g_renderer.init(&u8g2);
    g_leds.init();
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
    
    // Render current screen
    g_renderer.render();
    
    delay(MAIN_LOOP_DELAY_MS);
}