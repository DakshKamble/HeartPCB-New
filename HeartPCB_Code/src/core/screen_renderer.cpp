#include "screen_renderer.h"
#include "bitmaps.h"
#include "led_controller.h"
#include "battery_monitor.h"

// Global instance
ScreenRenderer g_renderer;

void ScreenRenderer::init(U8G2* display) {
    u8g2 = display;
    u8g2->begin();
    u8g2->setFontMode(1);
    u8g2->setBitmapMode(1);
}

void ScreenRenderer::begin_frame(bool inverted) {
    u8g2->clearBuffer();
    
    if (inverted) {
        u8g2->setDrawColor(1);
        u8g2->drawBox(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
        u8g2->setDrawColor(0);
    } else {
        u8g2->setDrawColor(1);
    }
}

void ScreenRenderer::end_frame() {
    u8g2->sendBuffer();
}

void ScreenRenderer::draw_battery_icon() {
    // Draw battery outline
    u8g2->drawXBM(112, 5, 12, 7, BatteryIcon);
    
    // Get battery info
    int percentage = g_battery.get_percentage();
    bool is_charging = g_battery.is_charging();
    
    // Calculate fill width (max 7 pixels)
    int fill_width = (percentage * 7) / 100;
    if (fill_width > 7) fill_width = 7;
    if (fill_width < 0) fill_width = 0;
    
    if (is_charging) {
        // Charging animation - pulsing fill
        static unsigned long last_anim = 0;
        static int anim_step = 0;
        
        if (millis() - last_anim > 200) {  // 200ms animation steps
            anim_step = (anim_step + 1) % 8;
            last_anim = millis();
        }
        
        // Animated fill that pulses from 0 to full
        int anim_width = (anim_step * 7) / 7;
        if (anim_width > 0) {
            u8g2->drawBox(114, 7, anim_width, 3);
        }
        
        // Draw charging indicator next to battery
        u8g2->setFont(u8g2_font_4x6_tr);
        u8g2->drawStr(105, 11, "C");  // Charging indicator
    } else {
        // Normal fill based on percentage
        if (fill_width > 0) {
            u8g2->drawBox(114, 7, fill_width, 3);
        }
    }
}

void ScreenRenderer::draw_status_icons() {
    draw_battery_icon();
    u8g2->drawXBM(114, 19, 7, 8, BeaconIcon);
}

void ScreenRenderer::render() {
    bool inverted = g_state.should_invert_display();
    
    switch (g_state.get_state()) {
        case STATE_SPLASH:
            draw_splash();
            break;
        case STATE_MENU:
            draw_menu();
            break;
        case STATE_PROPOSE_WAITING:
            draw_propose_waiting();
            break;
        case STATE_PROPOSE_DISPLAYING:
            draw_propose_displaying();
            break;
        case STATE_OSCILLOSCOPE:
            draw_oscilloscope();
            break;
        case STATE_ANIMATIONS:
            draw_animations();
            break;
        case STATE_BATTERY:
            draw_battery();
            break;
    }
}

void ScreenRenderer::draw_splash() {
    begin_frame(g_state.should_invert_display());
    
    u8g2->setFont(u8g2_font_profont29_tr);
    u8g2->drawStr(SPLASH_TEXT_X, SPLASH_TEXT_Y, SPLASH_TEXT);
    
    end_frame();
}

void ScreenRenderer::draw_menu() {
    begin_frame(g_state.should_invert_display());
    
    int selection = g_state.get_menu_selection();
    
    draw_status_icons();
    
    if (selection == 0) {
        // Propose menu item
        u8g2->drawXBM(2, 4, 28, 24, HeartIcon);
        u8g2->setFont(u8g2_font_t0_12_tr);
        u8g2->drawStr(46, 19, MENU_ITEM_PROPOSE);
    } else if (selection == 1) {
        // Oscilloscope menu item
        u8g2->drawXBM(13, 14, 15, 16, CrosshairsIcon);
        u8g2->drawXBM(2, 2, 15, 16, CrosshairsIcon);
        u8g2->setFont(u8g2_font_t0_12_tr);
        u8g2->drawStr(31, 19, MENU_ITEM_OSCILLOSCOPE);
    } else if (selection == 2) {
        // Animations menu item
        u8g2->drawXBM(10, 8, 15, 16, DisplayBrightnessIcon);
        u8g2->setFont(u8g2_font_t0_12_tr);
        u8g2->drawStr(38, 19, MENU_ITEM_ANIMATIONS);
    } else if (selection == 3) {
        // Battery menu item
        u8g2->drawXBM(8, 9, 24, 16, BatteryChargingIcon);
        u8g2->setFont(u8g2_font_t0_12_tr);
        u8g2->drawStr(46, 20, MENU_ITEM_BATTERY);
    }
    
    // Navigation arrows
    u8g2->drawXBM(64, 26, 8, 4, ArrowDown);
    u8g2->drawXBM(64, 2, 8, 4, ArrowUp);
    
    end_frame();
}

void ScreenRenderer::draw_propose_waiting() {
    begin_frame(g_state.should_invert_display());
    
    u8g2->setFont(u8g2_font_t0_12_tr);
    u8g2->drawStr(17, 20, PROPOSE_WAITING_TEXT);
    
    end_frame();
}

void ScreenRenderer::draw_propose_displaying() {
    begin_frame(g_state.should_invert_display());
    
    u8g2->setFont(u8g2_font_t0_15_tr);
    u8g2->drawStr(8, 21, PROPOSE_LOVE_TEXT);
    
    // Flashing heart
    if (g_state.should_show_heart()) {
        u8g2->drawXBM(98, 4, 28, 24, HeartIcon);
    }
    
    end_frame();
}

void ScreenRenderer::draw_oscilloscope() {
    begin_frame(g_state.should_invert_display());
    
    // Use large bold font for "Probe me !"
    u8g2->setFont(u8g2_font_t0_17b_tr);  // Bold 17px font
    
    // Center the text horizontally and vertically
    int text_width = u8g2->getStrWidth(OSCILLOSCOPE_TITLE);
    int text_x = (DISPLAY_WIDTH - text_width) / 2;
    int text_y = (DISPLAY_HEIGHT + 17) / 2;  // Center vertically (font height ~17px)
    
    u8g2->drawStr(text_x, text_y, OSCILLOSCOPE_TITLE);
    
    end_frame();
}

void ScreenRenderer::draw_animations() {
    begin_frame(g_state.should_invert_display());
    
    if (g_leds.is_brightness_control_active()) {
        // Show brightness control mode
        u8g2->setFont(u8g2_font_t0_12_tr);
        u8g2->drawStr(5, 12, "Brightness Control");
        
        // Show current brightness percentage
        uint8_t brightness = g_leds.get_brightness();
        int brightness_percent = (brightness * 100) / 255;
        
        char brightness_str[16];
        snprintf(brightness_str, sizeof(brightness_str), "%d%%", brightness_percent);
        
        u8g2->setFont(u8g2_font_t0_17b_tr);  // Large bold font
        int text_width = u8g2->getStrWidth(brightness_str);
        int text_x = (DISPLAY_WIDTH - text_width) / 2;
        u8g2->drawStr(text_x, 28, brightness_str);
        
        // Draw brightness bar
        int bar_width = (brightness * 100) / 255;  // 0-100 pixels
        u8g2->drawFrame(14, 2, 102, 6);  // Outer frame
        if (bar_width > 0) {
            u8g2->drawBox(15, 3, bar_width, 4);  // Fill bar
        }
    } else {
        // Show current animation name
        u8g2->setFont(u8g2_font_t0_12_tr);
        const char* anim_name = g_leds.get_animation_name();
        u8g2->drawStr(5, 12, "Animation:");
        u8g2->setFont(u8g2_font_t0_15_tr);
        u8g2->drawStr(5, 28, anim_name);
    }
    
    end_frame();
}

void ScreenRenderer::draw_battery() {
    begin_frame(g_state.should_invert_display());
    
    // Get battery info
    int percentage = g_battery.get_percentage();
    bool is_charging = g_battery.is_charging();
    
    // Draw battery frame outline (62x26 at position 7,3)
    u8g2->drawXBM(7, 3, 62, 26, BatteryFrameIcon);
    
    // Calculate fill width based on percentage (max 48 pixels wide)
    int fill_width = (percentage * 48) / 100;
    if (fill_width > 48) fill_width = 48;
    if (fill_width < 0) fill_width = 0;
    
    if (is_charging) {
        // Charging animation - wave effect
        static unsigned long last_anim = 0;
        static int wave_offset = 0;
        
        if (millis() - last_anim > 100) {  // 100ms animation steps
            wave_offset = (wave_offset + 1) % 48;
            last_anim = millis();
        }
        
        // Draw current level
        if (fill_width > 0) {
            u8g2->drawBox(11, 7, fill_width, 18);
        }
        
        // Draw animated charging wave (right to left)
        for (int i = 0; i < 48; i += 4) {
            int wave_pos = (48 - ((i + wave_offset) % 48)) - 2;  // Reverse direction
            if (wave_pos >= 0 && wave_pos < 46) {
                u8g2->drawBox(11 + wave_pos, 9, 2, 14);
            }
        }
        
        // Display charging status (center aligned)
        u8g2->setFont(u8g2_font_t0_12_tr);
        int charging_width = u8g2->getStrWidth("CHARGING");
        int charging_x = 75 + (53 - charging_width) / 2;  // Center in remaining space
        u8g2->drawStr(charging_x, 15, "CHARGING");
    } else {
        // Normal fill
        if (fill_width > 0) {
            u8g2->drawBox(11, 7, fill_width, 18);
        }
    }
    
    // Display percentage text (no space between number and %)
    char percent_str[16];
    snprintf(percent_str, sizeof(percent_str), "%d%%", percentage);
    u8g2->setFont(u8g2_font_t0_17b_tr);
    
    // Center align the percentage text
    int text_width = u8g2->getStrWidth(percent_str);
    int text_x = 75 + (53 - text_width) / 2;  // Center in remaining space (128-75=53)
    u8g2->drawStr(text_x, 28, percent_str);
    
    end_frame();
}
