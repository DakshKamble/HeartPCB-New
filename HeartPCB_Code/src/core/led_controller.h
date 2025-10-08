#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"

// LED animation types
enum LedAnimation {
    ANIM_OFF,
    ANIM_SOLID_RED,
    ANIM_SOLID_GREEN,
    ANIM_SOLID_BLUE,
    ANIM_RAINBOW,
    ANIM_BREATHING,
    ANIM_SPARKLE,
    ANIM_FIRE,
    ANIM_HEARTBEAT,        // Romantic pulsing red
    ANIM_SOFT_PINK_FADE,   // Gentle pink fade
    ANIM_LOVE_WAVE,        // Soft pink/red wave
    ANIM_PINK_STARS,       // Twinkling pink/red stars
    ANIM_ROMANTIC_CHASE,   // Chasing pink lights
    ANIM_ROSE_BLOOM,       // Expanding pink glow
    ANIM_CANDLELIGHT,      // Flickering warm glow
    ANIM_SUNSET_FADE,      // Orange to pink transition
    ANIM_LOVE_PULSE,       // Alternating red/pink pulse
    ANIM_SOFT_RAINBOW,     // Pastel rainbow
    ANIM_PINK_COMET,       // Pink shooting star
    ANIM_VALENTINE,        // Red and pink alternating
    ANIM_DREAMY_FADE,      // Multi-color soft fade
    ANIM_COUNT  // Total number of animations
};

// LED controller class
class LedController {
public:
    LedController();
    void init();
    void update();
    
    // Animation control
    void set_animation(LedAnimation anim);
    LedAnimation get_animation() const { return current_animation; }
    const char* get_animation_name() const;
    
    // Direct control
    void set_all(uint32_t color);
    void clear();
    void set_brightness(uint8_t brightness);

private:
    Adafruit_NeoPixel strip;
    LedAnimation current_animation = ANIM_OFF;
    unsigned long last_update = 0;
    uint8_t animation_step = 0;

    // Helper for color conversion
    uint32_t color_hsv(long hue, uint8_t sat, uint8_t val);

    // Animation implementations
    void anim_off();
    void anim_solid(uint32_t color);
    void anim_rainbow();
    void anim_breathing();
    void anim_sparkle();
    void anim_fire();
    void anim_heartbeat();
    void anim_soft_pink_fade();
    void anim_love_wave();
    void anim_pink_stars();
    void anim_romantic_chase();
    void anim_rose_bloom();
    void anim_candlelight();
    void anim_sunset_fade();
    void anim_love_pulse();
    void anim_soft_rainbow();
    void anim_pink_comet();
    void anim_valentine();
    void anim_dreamy_fade();
    
    // Heart-specific helper functions
    int get_heart_distance_from_top(int led_index);
    void set_heart_symmetric_leds(int distance, uint32_t color);
    void fill_heart_from_top(int max_distance, uint32_t color);
};

// Global LED controller instance
extern LedController g_leds;
