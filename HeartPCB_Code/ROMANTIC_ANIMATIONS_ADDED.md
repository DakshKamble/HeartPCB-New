# Romantic LED Animations Added

## Summary

Added 3 new romantic, slow, and soft LED animations perfect for a love-themed device.

## New Animations

### 1. 💓 Heartbeat
**Name**: "Heartbeat"  
**Effect**: Realistic double-pulse heartbeat pattern (lub-dub)  
**Colors**: Soft red/pink  
**Speed**: Slow and gentle  
**Pattern**: Beat-beat-pause rhythm

**Technical Details**:
- 120-step cycle for slow, romantic feel
- Two pulses (15 steps each) followed by long pause
- Desaturated red (HSV: 0, 200, variable) for softer look
- Mimics actual heartbeat timing

### 2. 🌸 Pink Fade
**Name**: "Pink Fade"  
**Effect**: Gentle fade in and out  
**Colors**: Soft pink  
**Speed**: Very slow and calming  
**Pattern**: Smooth sine wave breathing

**Technical Details**:
- 80-step cycle for ultra-slow fade
- Smooth sine wave (0-255 brightness)
- Soft pink color (HSV: 330, 180, variable)
- Perfect for ambient romantic lighting

### 3. 💕 Love Wave
**Name**: "Love Wave"  
**Effect**: Flowing wave of pink and red  
**Colors**: Pink to red gradient  
**Speed**: Slow, gentle movement  
**Pattern**: Traveling wave across strip

**Technical Details**:
- Slow wave movement (half speed)
- Hue range: 0-15 (red) and 240-255 (pink)
- Brightness variation: 150-235 for soft glow
- Each LED slightly offset for wave effect

## Complete Animation List (11 total)

| # | Name | Type | Speed | Colors |
|---|------|------|-------|--------|
| 0 | Off | - | - | None |
| 1 | Red | Solid | - | Red |
| 2 | Green | Solid | - | Green |
| 3 | Blue | Solid | - | Blue |
| 4 | Rainbow | Flow | Medium | Full spectrum |
| 5 | Breathing | Pulse | Medium | Blue |
| 6 | Sparkle | Random | Fast | White |
| 7 | Fire | Flicker | Medium | Orange/Red |
| 8 | **Heartbeat** | **Pulse** | **Slow** | **Soft Red/Pink** |
| 9 | **Pink Fade** | **Fade** | **Very Slow** | **Soft Pink** |
| 10 | **Love Wave** | **Wave** | **Slow** | **Pink/Red** |

## Usage

### Select Romantic Animations

1. Navigate to **Animations** menu
2. **Long press** to enter
3. **Short press** to cycle through animations
4. Select one of the new romantic animations:
   - **Heartbeat** - For intimate moments
   - **Pink Fade** - For calm, romantic ambiance
   - **Love Wave** - For gentle, flowing romance
5. **Long press** to exit (animation continues)

### Perfect For

**Heartbeat**:
- Proposal moments
- Romantic dinners
- Intimate settings
- Valentine's Day

**Pink Fade**:
- Ambient lighting
- Relaxation
- Bedroom mood lighting
- Soft background glow

**Love Wave**:
- Date nights
- Romantic gestures
- Photo/video backgrounds
- Gentle movement effect

## Technical Implementation

### Files Modified

```
src/core/
├── led_controller.h      ← Added 3 animation enums
├── led_controller.cpp    ← Added 3 implementations
```

### Animation Parameters

**Heartbeat**:
```cpp
- Cycle: 120 steps (2 seconds at 60 FPS)
- Pattern: 15-15-90 (beat-pause-beat-long pause)
- Color: CHSV(0, 200, brightness)
```

**Pink Fade**:
```cpp
- Cycle: 80 steps per phase
- Brightness: sin(step/80) × 127.5
- Color: CHSV(330, 180, brightness)
```

**Love Wave**:
```cpp
- Speed: animation_step / 2 (half speed)
- Hue range: 0-15, 240-255 (pink/red only)
- Brightness: 150 + sin8(step + i*10) / 3
```

## Customization

### Adjust Speed

Edit `led_controller.cpp`:

**Heartbeat** - Change cycle length:
```cpp
int cycle = animation_step % 120;  // Increase for slower
```

**Pink Fade** - Change fade speed:
```cpp
float phase = animation_step / 80.0;  // Increase divisor for slower
```

**Love Wave** - Change wave speed:
```cpp
uint8_t hue = ((animation_step / 2) + ...  // Increase divisor for slower
```

### Adjust Colors

**Heartbeat** - Change to pure red:
```cpp
CRGB color = CHSV(0, 255, brightness);  // Full saturation
```

**Pink Fade** - Change pink tone:
```cpp
CRGB color = CHSV(340, 200, brightness);  // More magenta
CRGB color = CHSV(320, 150, brightness);  // Lighter pink
```

**Love Wave** - Adjust color range:
```cpp
hue = map(hue, 0, 127, 0, 20);  // Wider red range
hue = map(hue, 128, 255, 230, 255);  // Wider pink range
```

### Adjust Brightness

All animations respect the global brightness setting:
```cpp
// In config.h
#define LED_BRIGHTNESS 128  // 0-255
```

Or adjust per-animation:
```cpp
// In animation function
uint8_t brightness = 200;  // Max brightness for this animation
```

## Color Reference

### HSV Color Wheel
- **0-15**: Red
- **240-255**: Pink/Magenta
- **330**: Soft Pink
- **0 (sat 200)**: Desaturated Red

### Romantic Color Palette
```cpp
CHSV(0, 200, 255)    // Soft Red
CHSV(330, 180, 255)  // Soft Pink
CHSV(345, 200, 255)  // Rose Pink
CHSV(10, 220, 255)   // Warm Red
```

## Performance

- **CPU Impact**: Minimal (same as other animations)
- **Update Rate**: 60 FPS
- **Memory**: No additional RAM required
- **Power**: Same as other animations at same brightness

## Romantic Lighting Tips

### Heartbeat
- Use at 50% brightness for subtle effect
- Perfect for synchronized with music
- Combine with red solid color for variety

### Pink Fade
- Use at 30-50% brightness for ambient
- Great for all-night mood lighting
- Very power efficient (low average brightness)

### Love Wave
- Use at 60-80% brightness for visibility
- Beautiful on longer LED strips (30+ LEDs)
- Creates gentle movement without distraction

## Future Enhancements (Optional)

### Possible Additions

1. **Candle Flicker** - Romantic candlelight simulation
2. **Sunset Fade** - Warm orange to pink transition
3. **Starlight** - Gentle random pink sparkles
4. **Rose Bloom** - Expanding pink glow from center
5. **Synchronized Heartbeat** - Multiple devices in sync

### Advanced Features

1. **Speed Control** - Adjust animation speed from menu
2. **Color Picker** - Choose custom romantic colors
3. **Brightness Levels** - Quick brightness presets
4. **Transition Effects** - Smooth fade between animations

## Compile and Test

Run:
```bash
platformio run --target upload
```

Then:
1. Navigate to Animations menu
2. Cycle to "Heartbeat", "Pink Fade", or "Love Wave"
3. Enjoy the romantic lighting! 💕

Perfect for your proposal device! 🌹✨
