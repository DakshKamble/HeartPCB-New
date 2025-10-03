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
    "Breathing",
    "Sparkle",
    "Fire",
    "Heartbeat",
    "Pink Fade",
    "Love Wave",
    "Pink Stars",
    "Chase",
    "Rose Bloom",
    "Candlelight",
    "Sunset",
    "Love Pulse",
    "Soft Rainbow",
    "Pink Comet",
    "Valentine",
    "Dreamy"
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
        case ANIM_BREATHING:
            anim_breathing();
            break;
        case ANIM_SPARKLE:
            anim_sparkle();
            break;
        case ANIM_FIRE:
            anim_fire();
            break;
        case ANIM_HEARTBEAT:
            anim_heartbeat();
            break;
        case ANIM_SOFT_PINK_FADE:
            anim_soft_pink_fade();
            break;
        case ANIM_LOVE_WAVE:
            anim_love_wave();
            break;
        case ANIM_PINK_STARS:
            anim_pink_stars();
            break;
        case ANIM_ROMANTIC_CHASE:
            anim_romantic_chase();
            break;
        case ANIM_ROSE_BLOOM:
            anim_rose_bloom();
            break;
        case ANIM_CANDLELIGHT:
            anim_candlelight();
            break;
        case ANIM_SUNSET_FADE:
            anim_sunset_fade();
            break;
        case ANIM_LOVE_PULSE:
            anim_love_pulse();
            break;
        case ANIM_SOFT_RAINBOW:
            anim_soft_rainbow();
            break;
        case ANIM_PINK_COMET:
            anim_pink_comet();
            break;
        case ANIM_VALENTINE:
            anim_valentine();
            break;
        case ANIM_DREAMY_FADE:
            anim_dreamy_fade();
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

void LedController::anim_heartbeat() {
    // Heart-aware heartbeat effect - pulses from top to bottom
    // Double pulse like a real heartbeat
    
    // Heartbeat pattern: beat-beat-pause (lub-dub pattern)
    int cycle = animation_step % 120;  // Slower cycle for romantic feel
    uint8_t base_brightness = 0;
    
    if (cycle < 15) {
        // First beat (lub)
        base_brightness = sin8(cycle * 17);
    } else if (cycle < 30) {
        // Pause between beats
        base_brightness = 0;
    } else if (cycle < 45) {
        // Second beat (dub)
        base_brightness = sin8((cycle - 30) * 17);
    } else {
        // Long pause
        base_brightness = 0;
    }
    
    // Clear all LEDs first
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    
    // Heart-shaped pulse - stronger at top, weaker toward bottom
    for (int i = 0; i < NUM_LEDS; i++) {
        int distance = get_heart_distance_from_top(i);
        uint8_t brightness = map(distance, 0, 10, base_brightness, base_brightness / 3);
        leds[i] = CHSV(0, 200, brightness);  // Red with slight desaturation
    }
    
    animation_step++;
}

void LedController::anim_soft_pink_fade() {
    // Gentle pink fade in and out
    // Very slow, calming effect
    
    // Slow sine wave for smooth fading
    float phase = animation_step / 80.0;  // Very slow
    uint8_t brightness = (sin(phase) + 1.0) * 127.5;  // 0-255 range
    
    // Soft pink color (HSV: hue 234 = pink in 8-bit range)
    CRGB color = CHSV(234, 180, brightness);
    fill_solid(leds, NUM_LEDS, color);
    
    animation_step++;
}

void LedController::anim_love_wave() {
    // Gentle wave of pink and red colors flowing across the strip
    // Slow, romantic movement
    
    for (int i = 0; i < NUM_LEDS; i++) {
        // Create a slow-moving wave
        uint8_t hue = ((animation_step / 2) + (i * 256 / NUM_LEDS)) % 256;
        
        // Map to pink/red range (hue 0-20 and 230-255)
        // Convert full spectrum position to pink/red range
        if (hue < 128) {
            hue = map(hue, 0, 127, 0, 15);  // Red to pink
        } else {
            hue = map(hue, 128, 255, 240, 255);  // Pink to red
        }
        
        // Soft, gentle brightness variation
        uint8_t brightness = 150 + sin8(animation_step + i * 10) / 3;
        
        leds[i] = CHSV(hue, 200, brightness);
    }
    
    animation_step++;
}

void LedController::anim_pink_stars() {
    // Twinkling pink and red stars - your requested effect!
    // Dense, fast twinkling between pink and red
    
    // Fade all LEDs slightly (slower fade = more dense)
    fadeToBlackBy(leds, NUM_LEDS, 3);
    
    // Randomly twinkle many LEDs (higher probability = more dense)
    if (random8() < 200) {  // Increased from 100 to 200 for more density
        int pos = random16(NUM_LEDS);
        // Random hue between red (0) and pink (234-250)
        uint8_t hue = random8(2) ? random8(0, 10) : random8(234, 250);
        uint8_t brightness = random8(180, 255);  // Brighter minimum
        leds[pos] = CHSV(hue, 200, brightness);
    }
    
    // Add second twinkle for even more density
    if (random8() < 150) {
        int pos = random16(NUM_LEDS);
        uint8_t hue = random8(2) ? random8(0, 10) : random8(234, 250);
        uint8_t brightness = random8(180, 255);
        leds[pos] = CHSV(hue, 200, brightness);
    }
}

void LedController::anim_romantic_chase() {
    // Heart-aware chasing pink lights - flows around heart shape
    
    fadeToBlackBy(leds, NUM_LEDS, 25);
    
    // Chase around the heart perimeter: 1→2→...→11→...→20→21→1
    int pos = (animation_step / 3) % NUM_LEDS;  // Slower chase
    leds[pos] = CHSV(234, 200, 255);  // Pink
    
    // Add trailing glow following heart shape
    int trail1 = (pos - 1 + NUM_LEDS) % NUM_LEDS;
    int trail2 = (pos - 2 + NUM_LEDS) % NUM_LEDS;
    int trail3 = (pos - 3 + NUM_LEDS) % NUM_LEDS;
    
    leds[trail1] = CHSV(234, 200, 180);
    leds[trail2] = CHSV(234, 200, 120);
    leds[trail3] = CHSV(234, 200, 60);
    
    animation_step++;
}

void LedController::anim_rose_bloom() {
    // Heart-aware expanding pink glow from top to bottom
    
    // Clear all LEDs first
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    
    // Expand from top of heart (LEDs 1&21) to bottom (LED 11)
    int max_distance = (animation_step / 6) % 12;  // Slower expansion
    
    for (int dist = 0; dist <= max_distance && dist <= 10; dist++) {
        uint8_t brightness = map(dist, 0, max_distance, 255, 80);
        CRGB color = CHSV(234, 200, brightness);
        set_heart_symmetric_leds(dist, color);
    }
    
    animation_step++;
}

void LedController::anim_candlelight() {
    // Flickering warm candlelight effect
    
    for (int i = 0; i < NUM_LEDS; i++) {
        // Random flicker
        uint8_t flicker = random8(200, 255);
        // Warm orange/yellow color
        leds[i] = CHSV(25, 200, flicker);
    }
}

void LedController::anim_sunset_fade() {
    // Slow transition from orange to pink like a sunset
    
    // Cycle through sunset colors
    uint8_t hue = map(animation_step % 200, 0, 199, 10, 240);  // Orange to pink
    uint8_t brightness = 200;
    
    fill_solid(leds, NUM_LEDS, CHSV(hue, 200, brightness));
    
    animation_step++;
}

void LedController::anim_love_pulse() {
    // Alternating red and pink pulses
    
    int cycle = animation_step % 100;
    uint8_t brightness = sin8(cycle * 2.55);
    
    // Alternate between red and pink
    uint8_t hue = (animation_step / 100) % 2 ? 0 : 234;
    
    fill_solid(leds, NUM_LEDS, CHSV(hue, 200, brightness));
    
    animation_step++;
}

void LedController::anim_soft_rainbow() {
    // Pastel rainbow with reduced saturation
    
    fill_rainbow(leds, NUM_LEDS, animation_step, 7);
    
    // Reduce saturation for pastel effect
    for (int i = 0; i < NUM_LEDS; i++) {
        CHSV hsv = rgb2hsv_approximate(leds[i]);
        hsv.s = 150;  // Lower saturation for pastel
        leds[i] = hsv;
    }
    
    animation_step += 2;
}

void LedController::anim_pink_comet() {
    // Pink shooting star/comet effect
    
    fadeToBlackBy(leds, NUM_LEDS, 30);
    
    int pos = (animation_step / 2) % NUM_LEDS;
    
    // Bright head
    leds[pos] = CHSV(234, 200, 255);
    
    // Trailing tail
    for (int i = 1; i < 8; i++) {
        int tail_pos = pos - i;
        if (tail_pos >= 0) {
            uint8_t brightness = 255 - (i * 30);
            leds[tail_pos] = CHSV(234, 200, brightness);
        }
    }
    
    animation_step++;
}

void LedController::anim_valentine() {
    // Alternating red and pink LEDs
    
    for (int i = 0; i < NUM_LEDS; i++) {
        // Alternate pattern with slow shift
        bool is_red = ((i + (animation_step / 20)) % 2) == 0;
        uint8_t hue = is_red ? 0 : 234;
        uint8_t brightness = 200;
        
        leds[i] = CHSV(hue, 200, brightness);
    }
    
    animation_step++;
}

void LedController::anim_dreamy_fade() {
    // Soft multi-color fade through romantic colors
    
    // Cycle through romantic color palette
    uint8_t colors[] = {0, 10, 234, 245, 160, 200};  // Red, orange, pink, magenta, blue, purple
    int num_colors = 6;
    
    int color_index = (animation_step / 100) % num_colors;
    int next_index = (color_index + 1) % num_colors;
    
    uint8_t blend_amount = (animation_step % 100) * 2.55;
    
    CHSV color1 = CHSV(colors[color_index], 180, 200);
    CHSV color2 = CHSV(colors[next_index], 180, 200);
    
    CRGB rgb1 = color1;
    CRGB rgb2 = color2;
    CRGB blended = blend(rgb1, rgb2, blend_amount);
    
    fill_solid(leds, NUM_LEDS, blended);
    
    animation_step++;
}

// Heart-specific helper functions
int LedController::get_heart_distance_from_top(int led_index) {
    // Heart layout: LEDs 1&21 at top, LED 11 at bottom
    // Distance from top (0-based indexing)
    if (led_index == 0 || led_index == 20) {  // LEDs 1&21 (0-based: 0&20)
        return 0;  // Top of heart
    } else if (led_index == 10) {  // LED 11 (0-based: 10)
        return 10;  // Bottom of heart
    } else if (led_index < 10) {  // Left side: LEDs 2-10 (0-based: 1-9)
        return led_index;
    } else {  // Right side: LEDs 12-20 (0-based: 11-19)
        return 20 - led_index;
    }
}

void LedController::set_heart_symmetric_leds(int distance, CRGB color) {
    // Set LEDs at symmetric positions from top
    if (distance == 0) {
        // Top of heart: LEDs 1&21
        leds[0] = color;   // LED 1
        leds[20] = color;  // LED 21
    } else if (distance <= 10) {
        // Symmetric pairs down the heart
        int left_led = distance;      // Left side
        int right_led = 20 - distance; // Right side
        if (left_led < NUM_LEDS) leds[left_led] = color;
        if (right_led >= 0 && right_led < NUM_LEDS) leds[right_led] = color;
    }
}

void LedController::fill_heart_from_top(int max_distance, CRGB color) {
    // Fill heart from top down to max_distance
    for (int dist = 0; dist <= max_distance && dist <= 10; dist++) {
        set_heart_symmetric_leds(dist, color);
    }
}
