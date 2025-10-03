# Heart-Shaped LED Animations

## Heart Layout

Your 21 LEDs are arranged in a heart shape:

```
    1   21    ← Top of heart (side by side)
   2     20
  3       19
 4         18
5           17
6           16
7           15
8           14
9           13
10         12
   11        ← Bottom point of heart
```

**LED Flow**: 1→2→3→...→10→11→12→...→20→21→(back to 1)

## Heart-Aware Animations

### 💓 Heartbeat (Enhanced)
- **Effect**: Pulses from top of heart (LEDs 1&21) down to bottom (LED 11)
- **Pattern**: Lub-dub heartbeat rhythm
- **Gradient**: Strongest at top, weaker toward bottom
- **Perfect for**: Realistic heart pulsing effect

### 🌹 Rose Bloom (Enhanced)  
- **Effect**: Expands symmetrically from top to bottom
- **Pattern**: Starts at LEDs 1&21, grows down to LED 11
- **Gradient**: Bright at expansion edge, dimmer behind
- **Perfect for**: Blooming rose or heart filling with love

### 💕 Romantic Chase (Enhanced)
- **Effect**: Pink light chases around heart perimeter
- **Pattern**: Flows 1→2→...→11→...→20→21→1
- **Trail**: 4-LED trailing glow following the chase
- **Perfect for**: Love flowing around the heart

## Heart Helper Functions

### `get_heart_distance_from_top(int led_index)`
Returns distance from top of heart (0-10):
- LEDs 1&21: Distance 0 (top)
- LED 11: Distance 10 (bottom)
- LEDs 2-10: Distance 1-9 (left side)
- LEDs 12-20: Distance 9-1 (right side)

### `set_heart_symmetric_leds(int distance, CRGB color)`
Sets symmetric LED pairs at given distance from top:
- Distance 0: LEDs 1&21
- Distance 1: LEDs 2&20
- Distance 2: LEDs 3&19
- ...
- Distance 10: LED 11

### `fill_heart_from_top(int max_distance, CRGB color)`
Fills heart from top down to specified distance.

## Animation Behaviors

### Heart-Aware Animations
These animations understand the heart shape:

1. **Heartbeat** - Pulses from top to bottom with gradient
2. **Rose Bloom** - Expands symmetrically from top
3. **Romantic Chase** - Flows around heart perimeter

### Regular Animations
These work normally across all 21 LEDs:

- Pink Stars - Random twinkling
- Pink Fade - Uniform fade
- Love Wave - Wave across strip
- Candlelight - Random flicker
- Sunset - Uniform color fade
- Love Pulse - Uniform pulse
- Soft Rainbow - Rainbow flow
- Pink Comet - Linear comet
- Valentine - Alternating pattern
- Dreamy - Uniform color blend

## Heart Shape Visualization

```
LED Positions in Heart:
       1   21     ← Distance 0 (top)
      2     20    ← Distance 1
     3       19   ← Distance 2
    4         18  ← Distance 3
   5           17 ← Distance 4
  6             16 ← Distance 5
 7               15 ← Distance 6
8                 14 ← Distance 7
9                 13 ← Distance 8
10               12 ← Distance 9
    11              ← Distance 10 (bottom)
```

## Usage Examples

### Heartbeat Animation
```cpp
// Pulses from top (1&21) to bottom (11)
// Stronger at top, weaker at bottom
for (int i = 0; i < NUM_LEDS; i++) {
    int distance = get_heart_distance_from_top(i);
    uint8_t brightness = map(distance, 0, 10, max_bright, max_bright/3);
    leds[i] = CHSV(0, 200, brightness);
}
```

### Rose Bloom Animation
```cpp
// Expands symmetrically from top
for (int dist = 0; dist <= max_distance; dist++) {
    uint8_t brightness = map(dist, 0, max_distance, 255, 80);
    set_heart_symmetric_leds(dist, CHSV(234, 200, brightness));
}
```

### Romantic Chase Animation
```cpp
// Chases around heart perimeter
int pos = animation_step % NUM_LEDS;
leds[pos] = CHSV(234, 200, 255);  // Current position
// Add trailing glow
leds[(pos-1+NUM_LEDS)%NUM_LEDS] = CHSV(234, 200, 180);
```

## Heart Distance Map

| LED | Distance | Position |
|-----|----------|----------|
| 1 | 0 | Top left |
| 21 | 0 | Top right |
| 2 | 1 | Left side |
| 20 | 1 | Right side |
| 3 | 2 | Left side |
| 19 | 2 | Right side |
| ... | ... | ... |
| 10 | 9 | Left side |
| 12 | 9 | Right side |
| 11 | 10 | Bottom point |

## Customization

### Create New Heart-Aware Animation
```cpp
void LedController::anim_my_heart_effect() {
    // Clear all LEDs
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    
    // Your heart-aware logic here
    for (int dist = 0; dist <= 10; dist++) {
        // Calculate color/brightness based on distance
        CRGB color = calculate_color(dist);
        set_heart_symmetric_leds(dist, color);
    }
    
    animation_step++;
}
```

### Modify Existing Animation
```cpp
// Make any animation heart-aware
for (int i = 0; i < NUM_LEDS; i++) {
    int distance = get_heart_distance_from_top(i);
    // Use distance to modify color/brightness
    uint8_t brightness = map(distance, 0, 10, 255, 100);
    leds[i] = CHSV(hue, saturation, brightness);
}
```

## Technical Details

### Heart Geometry
- **Total LEDs**: 21
- **Top width**: 2 LEDs (1&21)
- **Max distance**: 10 (top to bottom)
- **Symmetric pairs**: 10 pairs + 1 bottom LED

### Performance
- Helper functions are lightweight (simple math)
- No additional memory usage
- Same update rate as regular animations

### LED Mapping
```
Physical:  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
Array:     0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20
Distance:  0  1  2  3  4  5  6  7  8  9 10  9  8  7  6  5  4  3  2  1  0
```

## Future Enhancements

### Possible Heart-Aware Animations
1. **Heart Fill** - Fill heart with color from top to bottom
2. **Heart Drain** - Drain color from bottom to top
3. **Heart Beat Wave** - Wave that follows heart contour
4. **Cupid's Arrow** - Effect that pierces through heart
5. **Love Ripple** - Ripples emanating from heart center

### Advanced Features
1. **Heart Beat Sync** - Sync with actual heart rate sensor
2. **Dual Heart** - Two hearts beating in sync
3. **Heart Break** - Dramatic breaking effect
4. **Mending Heart** - Healing/repair animation

Perfect for your romantic proposal device! 💕✨
