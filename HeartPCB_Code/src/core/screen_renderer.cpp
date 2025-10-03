#include "screen_renderer.h"
#include "bitmaps.h"
#include "led_controller.h"

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
    u8g2->drawXBM(112, 5, 12, 7, BatteryIcon);
    u8g2->drawBox(114, 7, 7, 3);
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
        case STATE_OTA_UPDATE:
            draw_ota_update();
            break;
        case STATE_ANIMATIONS:
            draw_animations();
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
        // OTA Update menu item
        u8g2->drawXBM(8, 8, 17, 16, DownloadIcon);
        u8g2->setFont(u8g2_font_t0_12_tr);
        u8g2->drawStr(38, 19, MENU_ITEM_OTA);
    } else if (selection == 3) {
        // Animations menu item
        u8g2->drawXBM(10, 8, 15, 16, DisplayBrightnessIcon);
        u8g2->setFont(u8g2_font_t0_12_tr);
        u8g2->drawStr(38, 19, MENU_ITEM_ANIMATIONS);
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
    
    u8g2->setFont(u8g2_font_t0_12_tr);
    u8g2->drawStr(10, 15, OSCILLOSCOPE_TITLE);
    u8g2->drawStr(15, 28, OSCILLOSCOPE_EXIT_TEXT);
    
    end_frame();
}

void ScreenRenderer::draw_ota_update() {
    begin_frame(g_state.should_invert_display());
    
    u8g2->setFont(u8g2_font_t0_12_tr);
    u8g2->drawStr(15, 15, OTA_TITLE);
    u8g2->drawStr(15, 28, OTA_EXIT_TEXT);
    
    end_frame();
}

void ScreenRenderer::draw_animations() {
    begin_frame(g_state.should_invert_display());
    
    // Show current animation name
    u8g2->setFont(u8g2_font_t0_12_tr);
    const char* anim_name = g_leds.get_animation_name();
    u8g2->drawStr(5, 12, "Animation:");
    u8g2->setFont(u8g2_font_t0_15_tr);
    u8g2->drawStr(5, 28, anim_name);
    
    // Draw navigation hint
    u8g2->setFont(u8g2_font_4x6_tr);
    u8g2->drawStr(85, 28, "Press>");
    
    end_frame();
}
