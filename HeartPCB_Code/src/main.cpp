#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <u8g2lib.h>
#include "core/display.h"
#include "core/button_handler.h"
#include "core/menu.h"

// Display object - shared with display module
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);

void setup() {
    // Initialize all modules
    init_display();
    init_button_handler();
    init_menu();
}

void loop() {
    // Check if splash screen should exit automatically
    if (is_showing_splash() && should_exit_splash()) {
        exit_splash_to_menu();
    }
    
    // Handle user input (only if not in splash mode)
    if (!is_showing_splash()) {
        handle_button_input();
    }
    
    // Check if propose mode should return to menu automatically
    if (is_item_selected() && get_current_screen() == 0 && should_return_to_menu()) {
        exit_to_menu();
        set_propose_state(0); // Reset to waiting state
    }
    
    // Get current selected item from button handler
    int selected_item = get_selected_item();
    
    // Handle display inversion for long press (check state first)
    set_display_invert(is_long_press_active());
    
    // Draw the main screen with current selection
    draw_main_screen(selected_item);
    
    // Small delay to prevent excessive CPU usage
    delay(10);
}