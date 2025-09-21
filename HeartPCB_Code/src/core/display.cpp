#include "display.h"
#include "menu.h"

//Display object (external, defined in main.cpp)
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

//Initialize display
void init_display() {
    u8g2.begin();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
}

//Clear display buffer
void clear_display() {
    u8g2.clearBuffer();
}

//Send buffer to display
void update_display() {
    u8g2.sendBuffer();
}

//Draw battery icon with charge level
void draw_battery_icon() {
    u8g2.drawXBM(112, 5, 12, 7, BatteryIcon);
    u8g2.drawBox(114, 7, 7, 3); // Battery fill
}

//Draw status icons
void draw_status_icons() {
    draw_battery_icon();
    u8g2.drawXBM(114, 19, 7, 8, BeaconIcon);
}

//Draw main screen with menu selection
void draw_main_screen(int selected_item) {
    if (selected_item == 0) {
        draw_propose_screen();
    } else if (selected_item == 1) {
        draw_oscilloscope_screen();
    }
}

//Draw Propose mode screen
void draw_propose_screen() {
    clear_display();
    
    // Draw status icons
    draw_status_icons();
    
    // Draw main heart icon
    u8g2.drawXBM(2, 4, 28, 24, HeartIcon);
    
    // Draw "Propose" text
    u8g2.setFont(u8g2_font_t0_12_tr);
    u8g2.drawStr(46, 19, "Propose");
    
    // Draw navigation arrows
    u8g2.drawXBM(64, 26, 8, 4, ArrowDown);
    u8g2.drawXBM(64, 2, 8, 4, ArrowUp);
    
    update_display();
}

//Draw Oscilloscope mode screen
void draw_oscilloscope_screen() {
    clear_display();
    
    // Draw status icons (battery and beacon)
    draw_status_icons();
    
    // Draw "Oscilloscope" text
    u8g2.setFont(u8g2_font_t0_12_tr);
    u8g2.drawStr(31, 19, "Oscilloscope");
    
    // Draw crosshairs icons (two positions as in your example)
    u8g2.drawXBM(13, 14, 15, 16, CrosshairsIcon);
    u8g2.drawXBM(2, 2, 15, 16, CrosshairsIcon);
    
    // Draw navigation arrows
    u8g2.drawXBM(64, 26, 8, 4, ArrowDown);
    u8g2.drawXBM(64, 2, 8, 4, ArrowUp);
    
    update_display();
}
