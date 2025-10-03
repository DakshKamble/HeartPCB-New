#include "led_controller.h"

// Global instance
LedController g_leds;

// Animation names
static const char* animation_names[] = {
    "Off",
    "Red",
    "Green",
    "Blue",
    "Rainbow",
    "Rainbow Cycle",
    "Breathing",
    "Sparkle",
    "Fire"
};

void LedController::init() {
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(LED_BRIGHTNESS);
    clear();
}

void LedController::update() {
    unsigned long now = millis();
    
    // Update at ~60 FPS
    if (now - last_update < 16) {
        return;
    }
    last_update = now;
    
    // Run current animation
    switch (current_animation) {
        case ANIM_OFF:
            anim_off();
            break;
        case ANIM_SOLID_RED:
            anim_solid(CRGB::Red);
            break;
        case ANIM_SOLID_GREEN:
            anim_solid(CRGB::Green);
            break;
        case ANIM_SOLID_BLUE:
            anim_solid(CRGB::Blue);
            break;
        case ANIM_RAINBOW:
            anim_rainbow();
            break;
        case ANIM_RAINBOW_CYCLE:
            anim_rainbow_cycle();
            break;
        case ANIM_BREATHING:
            anim_breathing();
            break;
        case ANIM_SPARKLE:
            anim_sparkle();
            break;
        case ANIM_FIRE:
            anim_fire();
            break;
        default:
            break;
    }
    
    FastLED.show();
}

void LedController::set_animation(LedAnimation anim) {
    if (anim >= 0 && anim < ANIM_COUNT) {
        current_animation = anim;
        animation_step = 0;
    }
}

const char* LedController::get_animation_name() const {
    if (current_animation >= 0 && current_animation < ANIM_COUNT) {
        return animation_names[current_animation];
    }
    return "Unknown";
}

void LedController::set_all(CRGB color) {
    fill_solid(leds, NUM_LEDS, color);
    FastLED.show();
}

void LedController::clear() {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
}

void LedController::set_brightness(uint8_t brightness) {
    FastLED.setBrightness(brightness);
}

// Animation implementations

void LedController::anim_off() {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
}

void LedController::anim_solid(CRGB color) {
    fill_solid(leds, NUM_LEDS, color);
}

void LedController::anim_rainbow() {
    fill_rainbow(leds, NUM_LEDS, animation_step, 7);
    animation_step += 2;
}

void LedController::anim_rainbow_cycle() {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV((animation_step + (i * 256 / NUM_LEDS)) % 256, 255, 255);
    }
    animation_step += 2;
}

void LedController::anim_breathing() {
    // Sine wave breathing effect
    uint8_t brightness = (exp(sin(animation_step / 25.0 * PI)) - 0.36787944) * 108.0;
    fill_solid(leds, NUM_LEDS, CHSV(160, 255, brightness));  // Blue breathing
    animation_step++;
}

void LedController::anim_sparkle() {
    // Fade all LEDs slightly
    fadeToBlackBy(leds, NUM_LEDS, 20);
    
    // Add random sparkles
    if (random8() < 80) {
        int pos = random16(NUM_LEDS);
        leds[pos] = CHSV(random8(), 200, 255);
    }
}

void LedController::anim_fire() {
    // Simple fire effect
    static byte heat[NUM_LEDS];
    
    // Cool down every cell a little
    for (int i = 0; i < NUM_LEDS; i++) {
        heat[i] = qsub8(heat[i], random8(0, ((55 * 10) / NUM_LEDS) + 2));
    }
    
    // Heat from each cell drifts up
    for (int k = NUM_LEDS - 1; k >= 2; k--) {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
    }
    
    // Randomly ignite new sparks
    if (random8() < 120) {
        int y = random8(7);
        heat[y] = qadd8(heat[y], random8(160, 255));
    }
    
    // Convert heat to LED colors
    for (int j = 0; j < NUM_LEDS; j++) {
        CRGB color = HeatColor(heat[j]);
        leds[j] = color;
    }
}
