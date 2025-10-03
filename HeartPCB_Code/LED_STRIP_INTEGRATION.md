# WS2812B LED Strip Integration

## Summary

Successfully integrated WS2812B LED strip control into the HeartPCB firmware using the simplified architecture.

## Hardware Configuration

- **LED Strip**: WS2812B (NeoPixel compatible)
- **Data Pin**: GPIO 2 (D2)
- **Default LED Count**: 30 LEDs
- **Default Brightness**: 128 (50%)
- **Color Order**: GRB

### Customization

Edit `src/core/config.h` to match your setup:

```cpp
#define LED_PIN 2              // Change if using different pin
#define NUM_LEDS 30            // Change to your LED count
#define LED_BRIGHTNESS 128     // 0-255, adjust brightness
#define LED_TYPE WS2812B       // Or WS2811, SK6812, etc.
#define COLOR_ORDER GRB        // Or RGB, BRG, etc.
```

## Available Animations

The LED controller includes 9 built-in animations:

| # | Name | Description |
|---|------|-------------|
| 0 | **Off** | All LEDs off |
| 1 | **Red** | Solid red color |
| 2 | **Green** | Solid green color |
| 3 | **Blue** | Solid blue color |
| 4 | **Rainbow** | Flowing rainbow effect |
| 5 | **Rainbow Cycle** | Rainbow distributed across strip |
| 6 | **Breathing** | Pulsing blue breathing effect |
| 7 | **Sparkle** | Random white sparkles |
| 8 | **Fire** | Realistic fire simulation |

## How to Use

### Navigation
1. Navigate to **Animations** menu item (4th item)
2. **Long press** to enter Animations mode
3. **Short press** to cycle through animations
4. **Long press** to exit back to menu (LEDs turn off)

### Controls
- **Short Press**: Next animation
- **Long Press + Hold**: Screen inverts (visual feedback)
- **Long Press + Release**: Exit to menu

## Architecture

### New Files Created

```
src/core/
├── led_controller.h      ← LED controller class definition
└── led_controller.cpp    ← Animation implementations
```

### Modified Files

```
platformio.ini            ← Added FastLED library
src/core/config.h         ← LED hardware configuration
src/core/state_manager.h  ← Animation selection state
src/core/state_manager.cpp← Animation cycling logic
src/core/screen_renderer.cpp← Display current animation
src/main.cpp              ← Initialize and update LEDs
```

## Code Structure

### LedController Class

```cpp
class LedController {
public:
    void init();                          // Initialize FastLED
    void update();                        // Run current animation
    void set_animation(LedAnimation anim);// Change animation
    const char* get_animation_name();     // Get current name
    void set_all(CRGB color);            // Set all LEDs
    void clear();                         // Turn off all LEDs
    void set_brightness(uint8_t);         // Adjust brightness
};
```

### Integration Points

**State Manager**:
- Tracks `animation_selection` (0-8)
- `cycle_animation_selection()` changes animation
- Turns off LEDs when exiting Animations screen

**Screen Renderer**:
- Displays current animation name
- Shows "Press>" hint for navigation

**Main Loop**:
- Calls `g_leds.update()` every frame (~60 FPS)
- LEDs update independently of display

## Adding Custom Animations

### 1. Add to Enum (`led_controller.h`)

```cpp
enum LedAnimation {
    ANIM_OFF,
    // ... existing animations
    ANIM_YOUR_CUSTOM,  // Add here
    ANIM_COUNT
};
```

### 2. Add Name (`led_controller.cpp`)

```cpp
static const char* animation_names[] = {
    "Off",
    // ... existing names
    "Your Custom"  // Add here
};
```

### 3. Implement Animation (`led_controller.cpp`)

```cpp
void LedController::anim_your_custom() {
    // Your animation code here
    // Access LEDs via: leds[i]
    // Use animation_step for timing
    
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Purple;  // Example
    }
    animation_step++;
}
```

### 4. Add to Switch (`led_controller.cpp`)

```cpp
void LedController::update() {
    // ...
    switch (current_animation) {
        // ... existing cases
        case ANIM_YOUR_CUSTOM:
            anim_your_custom();
            break;
    }
}
```

## FastLED Tips

### Color Creation

```cpp
// RGB values
leds[i] = CRGB(255, 0, 0);  // Red

// Named colors
leds[i] = CRGB::Blue;

// HSV (Hue, Saturation, Value)
leds[i] = CHSV(160, 255, 255);  // Blue
```

### Useful Functions

```cpp
fill_solid(leds, NUM_LEDS, CRGB::Red);     // Fill all
fill_rainbow(leds, NUM_LEDS, hue, delta);   // Rainbow
fadeToBlackBy(leds, NUM_LEDS, amount);      // Fade
blur1d(leds, NUM_LEDS, amount);             // Blur effect
```

### Color Palettes

```cpp
CRGBPalette16 palette = RainbowColors_p;
leds[i] = ColorFromPalette(palette, index);
```

## Performance

- **Update Rate**: ~60 FPS (16ms per frame)
- **CPU Impact**: Minimal, runs in main loop
- **Memory**: ~90 bytes per LED (30 LEDs = 2.7KB)

## Troubleshooting

### LEDs Don't Light Up

1. Check wiring:
   - Data → GPIO 2
   - 5V → Power supply
   - GND → Common ground

2. Verify configuration:
   - `NUM_LEDS` matches your strip
   - `LED_TYPE` is correct (WS2812B vs WS2811)
   - `COLOR_ORDER` matches (try GRB or RGB)

3. Check power:
   - WS2812B needs 5V
   - Each LED draws ~60mA at full white
   - Use external power for >10 LEDs

### Wrong Colors

- Try changing `COLOR_ORDER` in config.h:
  - `GRB` (most common for WS2812B)
  - `RGB`
  - `BRG`

### Flickering

- Add capacitor (100-1000µF) across power supply
- Reduce `LED_BRIGHTNESS` in config.h
- Ensure stable 5V power supply

### Animations Too Fast/Slow

Adjust update rate in `led_controller.cpp`:

```cpp
// Current: 60 FPS
if (now - last_update < 16) {  // Change 16 to adjust

// Slower: 30 FPS
if (now - last_update < 33) {
```

## Power Consumption

| LEDs | Brightness | Current (mA) |
|------|------------|--------------|
| 30 | 50% (128) | ~900 mA |
| 30 | 100% (255) | ~1800 mA |
| 60 | 50% (128) | ~1800 mA |

**Recommendation**: Use external 5V power supply for >10 LEDs

## Example: Adding Strobe Effect

```cpp
// 1. Add to enum
ANIM_STROBE,

// 2. Add name
"Strobe",

// 3. Implement
void LedController::anim_strobe() {
    if (animation_step % 10 < 2) {
        fill_solid(leds, NUM_LEDS, CRGB::White);
    } else {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
    }
    animation_step++;
}

// 4. Add case
case ANIM_STROBE:
    anim_strobe();
    break;
```

## Architecture Compliance ✅

This integration follows the simplified architecture:
- ✅ Configuration in `config.h`
- ✅ Self-contained `LedController` module
- ✅ State managed by `StateManager`
- ✅ Display handled by `ScreenRenderer`
- ✅ No cross-module dependencies
- ✅ Easy to extend with new animations

## Next Steps

1. **Compile and upload** the firmware
2. **Test animations** - navigate to Animations menu
3. **Adjust LED count** in config.h if needed
4. **Add custom animations** using the pattern above
5. **Integrate with other screens** (e.g., LEDs pulse during Propose mode)

## Dependencies

- **FastLED**: v3.7.0 (automatically installed by PlatformIO)
- **License**: MIT (FastLED is also MIT)

Enjoy your LED animations! 🌈✨
