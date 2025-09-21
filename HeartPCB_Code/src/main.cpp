#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <u8g2lib.h>
#include "core/display.h"
#include "core/button_handler.h"
#include "core/menu.h"

// Display object - shared with display module
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

void setup() {
    // Initialize all modules
    init_display();
    init_button_handler();
    init_menu();
}

void loop() {
    // Handle user input
    handle_button_input();
    
    // Get current selected item from button handler
    int selected_item = get_selected_item();
    
    // Draw the main screen with current selection
    draw_main_screen(selected_item);
    
    // Small delay to prevent excessive CPU usage
    delay(10);
}