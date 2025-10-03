#include "display.h"
#include "menu.h"

//Display object (external, defined in main.cpp)
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

//Display state tracking
static bool display_is_inverted = false;

//Initialize display
void init_display() {
    u8g2.begin();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setDrawColor(1); // Start with normal white drawing color
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
    // Check current screen state
    if (is_showing_splash()) {
        // Splash screen mode
        draw_splash_screen();
    } else if (is_in_menu()) {
        // Menu browsing mode
        if (selected_item == 0) {
            draw_propose_screen();
        } else if (selected_item == 1) {
            draw_oscilloscope_screen();
        }
    } else if (is_item_selected()) {
        // Selection mode - show the selected item's content
        if (get_current_screen() == 0) {
            draw_propose_selected_screen();
        } else if (get_current_screen() == 1) {
            draw_oscilloscope_selected_screen();
        }
    }
}

//Draw Propose mode screen
void draw_propose_screen() {
    clear_display();
    
    // Handle background for inverted mode
    if (display_is_inverted) {
        u8g2.setDrawColor(1);
        u8g2.drawBox(0, 0, 128, 64); // Fill with white background
        u8g2.setDrawColor(0); // Draw in black
    } else {
        u8g2.setDrawColor(1); // Normal white drawing
    }
    
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
    
    // Handle background for inverted mode
    if (display_is_inverted) {
        u8g2.setDrawColor(1);
        u8g2.drawBox(0, 0, 128, 64); // Fill with white background
        u8g2.setDrawColor(0); // Draw in black
    } else {
        u8g2.setDrawColor(1); // Normal white drawing
    }
    
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

//Set display inversion (for long press feedback)
void set_display_invert(bool inverted) {
    // Only change if state is different to prevent flickering
    if (display_is_inverted != inverted) {
        display_is_inverted = inverted;
        // The actual inversion drawing is handled in the screen drawing functions
    }
}

//Draw Propose selected screen (handles sub-states)
void draw_propose_selected_screen() {
    if (get_propose_state() == PROPOSE_WAITING) {
        draw_propose_waiting_screen();
    } else if (get_propose_state() == PROPOSE_DISPLAYING) {
        draw_propose_love_screen();
    }
}

//Draw Oscilloscope selected screen (placeholder)
void draw_oscilloscope_selected_screen() {
    clear_display();
    
    // Handle background for inverted mode
    if (display_is_inverted) {
        u8g2.setDrawColor(1);
        u8g2.drawBox(0, 0, 128, 32); // Updated for 128x32 display
        u8g2.setDrawColor(0);
    } else {
        u8g2.setDrawColor(1);
    }
    
    // Draw "Entered Oscilloscope" message
    u8g2.setFont(u8g2_font_t0_12_tr);
    u8g2.drawStr(10, 15, "Entered Oscilloscope");
    u8g2.drawStr(15, 28, "Press to exit");
    
    update_display();
}

//Draw "Press to Accept" screen
void draw_propose_waiting_screen() {
    clear_display();
    
    // Handle background for inverted mode
    if (display_is_inverted) {
        u8g2.setDrawColor(1);
        u8g2.drawBox(0, 0, 128, 32); // Updated for 128x32 display
        u8g2.setDrawColor(0);
    } else {
        u8g2.setDrawColor(1);
    }
    
    // Draw "Press to Accept" message
    u8g2.setFont(u8g2_font_t0_12_tr);
    u8g2.drawStr(17, 20, "Press to Accept");
    
    update_display();
}

//Draw "I Love You" screen with flashing heart
void draw_propose_love_screen() {
    clear_display();
    
    // Handle background for inverted mode
    if (display_is_inverted) {
        u8g2.setDrawColor(1);
        u8g2.drawBox(0, 0, 128, 32); // Updated for 128x32 display
        u8g2.setDrawColor(0);
    } else {
        u8g2.setDrawColor(1);
    }
    
    // Draw "I LOVE YOU" text
    u8g2.setFont(u8g2_font_t0_15_tr);
    u8g2.drawStr(8, 21, "I LOVE YOU");
    
    // Draw flashing heart (only show if flashing is on)
    if (should_show_flashing_heart()) {
        u8g2.drawXBM(98, 4, 28, 24, FlashingHeartIcon);
    }
    
    update_display();
}

//Determine if heart should be visible (flashing effect)
bool should_show_flashing_heart() {
    // Flash every 500ms (on for 500ms, off for 500ms)
    return (millis() % 1000) < 500;
}

//Draw splash screen
void draw_splash_screen() {
    clear_display();
    
    // Handle background for inverted mode
    if (display_is_inverted) {
        u8g2.setDrawColor(1);
        u8g2.drawBox(0, 0, 128, 32); // Updated for 128x32 display
        u8g2.setDrawColor(0);
    } else {
        u8g2.setDrawColor(1);
    }
    
    // Draw splash text with custom font
    u8g2.setFont(u8g2_font_profont29_tr);
    u8g2.drawStr(get_splash_text_x(), get_splash_text_y(), get_splash_text());
    
    update_display();
}
