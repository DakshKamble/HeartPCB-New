#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "config.h"

// LED animation types
enum LedAnimation {
    ANIM_OFF,
    ANIM_SOLID_RED,
    ANIM_SOLID_GREEN,
    ANIM_SOLID_BLUE,
    ANIM_RAINBOW,
    ANIM_RAINBOW_CYCLE,
    ANIM_BREATHING,
    ANIM_SPARKLE,
    ANIM_FIRE,
    ANIM_COUNT  // Total number of animations
};

// LED controller class
class LedController {
public:
    void init();
    void update();
    
    // Animation control
    void set_animation(LedAnimation anim);
    LedAnimation get_animation() const { return current_animation; }
    const char* get_animation_name() const;
    
    // Direct control
    void set_all(CRGB color);
    void clear();
    void set_brightness(uint8_t brightness);
    
private:
    CRGB leds[NUM_LEDS];
    LedAnimation current_animation = ANIM_OFF;
    unsigned long last_update = 0;
    uint8_t animation_step = 0;
    
    // Animation implementations
    void anim_off();
    void anim_solid(CRGB color);
    void anim_rainbow();
    void anim_rainbow_cycle();
    void anim_breathing();
    void anim_sparkle();
    void anim_fire();
};

// Global LED controller instance
extern LedController g_leds;
