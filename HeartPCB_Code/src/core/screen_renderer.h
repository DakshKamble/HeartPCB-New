#pragma once
#include <u8g2lib.h>
#include "config.h"
#include "state_manager.h"

// Screen renderer - handles all drawing logic
class ScreenRenderer {
public:
    void init(U8G2* display);
    void render();
    
private:
    U8G2* u8g2 = nullptr;
    
    // Helper functions
    void begin_frame(bool inverted);
    void end_frame();
    void draw_status_icons();
    void draw_battery_icon();
    
    // Screen drawing functions
    void draw_splash();
    void draw_menu();
    void draw_propose_waiting();
    void draw_propose_displaying();
    void draw_oscilloscope();
    void draw_ota_update();
    void draw_animations();
    void draw_battery();
};

// Global renderer instance
extern ScreenRenderer g_renderer;
