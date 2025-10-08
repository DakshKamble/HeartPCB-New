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

LedController::LedController() : strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800) {}

void LedController::init() {
    strip.begin();
    strip.setBrightness(LED_BRIGHTNESS);
    clear();
}

void LedController::update() {
    unsigned long now = millis();
    
    if (now - last_update < 16) { // ~60 FPS
        return;
    }
    last_update = now;
    
    switch (current_animation) {
        case ANIM_OFF:          anim_off(); break;
        case ANIM_SOLID_RED:    anim_solid(strip.Color(255, 0, 0)); break;
        case ANIM_SOLID_GREEN:  anim_solid(strip.Color(0, 255, 0)); break;
        case ANIM_SOLID_BLUE:   anim_solid(strip.Color(0, 0, 255)); break;
        case ANIM_RAINBOW:      anim_rainbow(); break;
        case ANIM_BREATHING:    anim_breathing(); break;
        case ANIM_SPARKLE:      anim_sparkle(); break;
        case ANIM_FIRE:         anim_fire(); break;
        case ANIM_HEARTBEAT:    anim_heartbeat(); break;
        case ANIM_SOFT_PINK_FADE: anim_soft_pink_fade(); break;
        case ANIM_LOVE_WAVE:    anim_love_wave(); break;
        case ANIM_PINK_STARS:   anim_pink_stars(); break;
        case ANIM_ROMANTIC_CHASE: anim_romantic_chase(); break;
        case ANIM_ROSE_BLOOM:   anim_rose_bloom(); break;
        case ANIM_CANDLELIGHT:  anim_candlelight(); break;
        case ANIM_SUNSET_FADE:  anim_sunset_fade(); break;
        case ANIM_LOVE_PULSE:   anim_love_pulse(); break;
        case ANIM_SOFT_RAINBOW: anim_soft_rainbow(); break;
        case ANIM_PINK_COMET:   anim_pink_comet(); break;
        case ANIM_VALENTINE:    anim_valentine(); break;
        case ANIM_DREAMY_FADE:  anim_dreamy_fade(); break;
        default: break;
    }
    
    strip.show();
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

void LedController::set_all(uint32_t color) {
    strip.fill(color);
    strip.show();
}

void LedController::clear() {
    strip.clear();
    strip.show();
}

void LedController::set_brightness(uint8_t brightness) {
    strip.setBrightness(brightness);
    strip.show();
}

// HSV to RGB conversion
uint32_t LedController::color_hsv(long hue, uint8_t sat, uint8_t val) {
    uint8_t r, g, b;
    hue %= 360; // h is now in [0, 359]
    long h = hue / 60;
    long f = (hue % 60) * 255 / 60;
    uint8_t p = (val * (255 - sat)) / 255;
    uint8_t q = (val * (255 - (sat * f) / 255)) / 255;
    uint8_t t = (val * (255 - (sat * (255 - f)) / 255)) / 255;

    switch (h) {
        case 0: r = val; g = t; b = p; break;
        case 1: r = q; g = val; b = p; break;
        case 2: r = p; g = val; b = t; break;
        case 3: r = p; g = q; b = val; break;
        case 4: r = t; g = p; b = val; break;
        default: r = val; g = p; b = q; break;
    }
    return strip.Color(r, g, b);
}

// Animation implementations

void LedController::anim_off() {
    strip.fill(0);
}

void LedController::anim_solid(uint32_t color) {
    strip.fill(color);
}

void LedController::anim_rainbow() {
    for (int i = 0; i < NUM_LEDS; i++) {
        int hue = ((animation_step * 2) + (i * 360 / NUM_LEDS)) % 360;
        strip.setPixelColor(i, color_hsv(hue, 255, 255));
    }
    animation_step++;
}

void LedController::anim_breathing() {
    uint8_t brightness = (exp(sin(animation_step / 25.0 * PI)) - 0.36787944) * 108.0;
    uint32_t color = color_hsv(240, 255, brightness); // Blue hue = 240 in 360 scale
    strip.fill(color);
    animation_step++;
}

void LedController::anim_sparkle() {
    // Fade all LEDs slightly
    for (int i = 0; i < NUM_LEDS; i++) {
        uint32_t color = strip.getPixelColor(i);
        uint8_t r = (color >> 16) & 0xFF;
        uint8_t g = (color >> 8) & 0xFF;
        uint8_t b = color & 0xFF;
        strip.setPixelColor(i, strip.Color(r * 0.9, g * 0.9, b * 0.9));
    }

    // Add random sparkles
    if (random(100) < 20) { // Adjusted probability
        int pos = random(NUM_LEDS);
        strip.setPixelColor(pos, color_hsv(random(360), 200, 255));
    }
}

void LedController::anim_fire() {
    static byte heat[NUM_LEDS];
    
    // Cool down
    for (int i = 0; i < NUM_LEDS; i++) {
        heat[i] = max(0, heat[i] - random(0, ((55 * 10) / NUM_LEDS) + 2));
    }
    
    // Heat drifts up
    for (int k = NUM_LEDS - 1; k >= 2; k--) {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
    }
    
    // Ignite new sparks
    if (random(100) < 60) {
        int y = random(7);
        heat[y] = min(255, heat[y] + random(160, 255));
    }
    
    // Convert heat to color
    for (int j = 0; j < NUM_LEDS; j++) {
        uint8_t hue = map(heat[j], 0, 255, 0, 60); // Red to Yellow
        uint8_t sat = 255;
        uint8_t val = heat[j];
        strip.setPixelColor(j, color_hsv(hue, sat, val));
    }
}

void LedController::anim_heartbeat() {
    int cycle = animation_step % 120;
    uint8_t base_brightness = 0;
    
    if (cycle < 15) {
        base_brightness = sin(cycle * (PI / 15.0)) * 255;
    } else if (cycle >= 30 && cycle < 45) {
        base_brightness = sin((cycle - 30) * (PI / 15.0)) * 255;
    }

    strip.clear();
    
    for (int i = 0; i < NUM_LEDS; i++) {
        int distance = get_heart_distance_from_top(i);
        uint8_t brightness = map(distance, 0, 10, base_brightness, base_brightness / 3);
        strip.setPixelColor(i, color_hsv(0, 200, brightness)); // Red
    }
    
    animation_step++;
}

void LedController::anim_soft_pink_fade() {
    float phase = animation_step / 80.0;
    uint8_t brightness = (sin(phase) + 1.0) * 127.5;
    uint32_t color = color_hsv(330, 180, brightness); // Pink hue ~330
    strip.fill(color);
    animation_step++;
}

void LedController::anim_love_wave() {
    for (int i = 0; i < NUM_LEDS; i++) {
        uint16_t hue = ((animation_step * 2) + (i * 360 / NUM_LEDS)) % 360;
        if (hue < 180) {
            hue = map(hue, 0, 179, 0, 30); // Red to pink
        } else {
            hue = map(hue, 180, 359, 330, 360); // Pink to red
        }
        uint8_t brightness = 150 + sin(animation_step / 10.0 + i / 5.0) * 50;
        strip.setPixelColor(i, color_hsv(hue, 200, brightness));
    }
    animation_step++;
}

void LedController::anim_pink_stars() {
    for (int i = 0; i < NUM_LEDS; i++) {
        uint32_t color = strip.getPixelColor(i);
        uint8_t r = (color >> 16) & 0xFF;
        uint8_t g = (color >> 8) & 0xFF;
        uint8_t b = color & 0xFF;
        strip.setPixelColor(i, strip.Color(r * 0.95, g * 0.95, b * 0.95));
    }

    if (random(100) < 50) { 
        int pos = random(NUM_LEDS);
        uint16_t hue = random(2) ? random(0, 10) : random(330, 350);
        uint8_t brightness = random(180, 255);
        strip.setPixelColor(pos, color_hsv(hue, 200, brightness));
    }
}

void LedController::anim_romantic_chase() {
    for (int i = 0; i < NUM_LEDS; i++) {
        uint32_t color = strip.getPixelColor(i);
        uint8_t r = (color >> 16) & 0xFF;
        uint8_t g = (color >> 8) & 0xFF;
        uint8_t b = color & 0xFF;
        strip.setPixelColor(i, strip.Color(r * 0.85, g * 0.85, b * 0.85));
    }
    
    int pos = (animation_step / 3) % NUM_LEDS;
    strip.setPixelColor(pos, color_hsv(330, 200, 255)); // Pink
    animation_step++;
}

void LedController::anim_rose_bloom() {
    strip.clear();
    int max_distance = (animation_step / 6) % 12;
    
    for (int dist = 0; dist <= max_distance && dist <= 10; dist++) {
        uint8_t brightness = map(dist, 0, max_distance, 255, 80);
        uint32_t color = color_hsv(330, 200, brightness);
        set_heart_symmetric_leds(dist, color);
    }
    animation_step++;
}

void LedController::anim_candlelight() {
    for (int i = 0; i < NUM_LEDS; i++) {
        uint8_t flicker = random(200, 255);
        strip.setPixelColor(i, color_hsv(30, 200, flicker)); // Orange/Yellow
    }
}

void LedController::anim_sunset_fade() {
    uint16_t hue = map(animation_step % 200, 0, 199, 10, 330); // Orange to pink
    strip.fill(color_hsv(hue, 200, 200));
    animation_step++;
}

void LedController::anim_love_pulse() {
    int cycle = animation_step % 100;
    uint8_t brightness = sin(cycle * (PI / 50.0)) * 255;
    uint16_t hue = (animation_step / 100) % 2 ? 0 : 330; // Red or Pink
    strip.fill(color_hsv(hue, 200, brightness));
    animation_step++;
}

void LedController::anim_soft_rainbow() {
    for (int i = 0; i < NUM_LEDS; i++) {
        int hue = ((animation_step * 2) + (i * 360 / NUM_LEDS)) % 360;
        strip.setPixelColor(i, color_hsv(hue, 150, 255)); // Lower saturation
    }
    animation_step++;
}

void LedController::anim_pink_comet() {
    for (int i = 0; i < NUM_LEDS; i++) {
        uint32_t color = strip.getPixelColor(i);
        uint8_t r = (color >> 16) & 0xFF;
        uint8_t g = (color >> 8) & 0xFF;
        uint8_t b = color & 0xFF;
        strip.setPixelColor(i, strip.Color(r * 0.8, g * 0.8, b * 0.8));
    }

    int pos = (animation_step / 2) % NUM_LEDS;
    strip.setPixelColor(pos, color_hsv(330, 200, 255));
    animation_step++;
}

void LedController::anim_valentine() {
    for (int i = 0; i < NUM_LEDS; i++) {
        bool is_red = ((i + (animation_step / 20)) % 2) == 0;
        uint16_t hue = is_red ? 0 : 330;
        strip.setPixelColor(i, color_hsv(hue, 200, 200));
    }
    animation_step++;
}

void LedController::anim_dreamy_fade() {
    uint16_t colors[] = {0, 30, 330, 300, 240, 270}; // Red, orange, pink, magenta, blue, purple
    int num_colors = 6;
    
    int color_index = (animation_step / 100) % num_colors;
    int next_index = (color_index + 1) % num_colors;
    
    float blend_amount = (animation_step % 100) / 100.0;
    
    uint32_t color1 = color_hsv(colors[color_index], 180, 200);
    uint32_t color2 = color_hsv(colors[next_index], 180, 200);

    uint8_t r1 = (color1 >> 16) & 0xFF, g1 = (color1 >> 8) & 0xFF, b1 = color1 & 0xFF;
    uint8_t r2 = (color2 >> 16) & 0xFF, g2 = (color2 >> 8) & 0xFF, b2 = color2 & 0xFF;

    uint8_t r = r1 + (r2 - r1) * blend_amount;
    uint8_t g = g1 + (g2 - g1) * blend_amount;
    uint8_t b = b1 + (b2 - b1) * blend_amount;

    strip.fill(strip.Color(r,g,b));
    
    animation_step++;
}

// Heart-specific helper functions
int LedController::get_heart_distance_from_top(int led_index) {
    if (led_index == 0 || led_index == 20) { return 0; }
    if (led_index == 10) { return 10; }
    if (led_index < 10) { return led_index; }
    return 20 - led_index;
}

void LedController::set_heart_symmetric_leds(int distance, uint32_t color) {
    if (distance == 0) {
        strip.setPixelColor(0, color);
        strip.setPixelColor(20, color);
    } else if (distance <= 10) {
        int left_led = distance;
        int right_led = 20 - distance;
        if (left_led < NUM_LEDS) strip.setPixelColor(left_led, color);
        if (right_led >= 0 && right_led < NUM_LEDS) strip.setPixelColor(right_led, color);
    }
}

void LedController::fill_heart_from_top(int max_distance, uint32_t color) {
    for (int dist = 0; dist <= max_distance && dist <= 10; dist++) {
        set_heart_symmetric_leds(dist, color);
    }
}
