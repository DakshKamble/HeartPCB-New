# LED Persistent Animation Mode

## Summary

Updated LED behavior so animations **persist across screens** and continue running until explicitly turned off.

## New Behavior

### ✅ Animation Persists
- Select an animation in Animations menu
- Exit with long press
- **Animation keeps running** in the background
- Works across all screens (Menu, Propose, Oscilloscope, OTA, etc.)

### ✅ How to Stop Animation
1. Enter Animations menu (long press on "Animations")
2. Short press to cycle to **"Off"**
3. Exit (long press)
4. LEDs turn off

## Usage Flow

```
1. Navigate to Animations menu item
   ↓
2. Long press → Enter Animations screen
   ↓
3. Short press → Cycle through animations
   - Off
   - Red
   - Green
   - Blue
   - Rainbow
   - Rainbow Cycle
   - Breathing
   - Sparkle
   - Fire
   ↓
4. Select desired animation (e.g., "Rainbow")
   ↓
5. Long press → Exit to menu
   ↓
6. Rainbow animation CONTINUES running
   ↓
7. Navigate to other screens → Animation still running
   ↓
8. To stop: Go back to Animations → Select "Off" → Exit
```

## Changes Made

### Modified Files

**`state_manager.cpp`**
- Removed `g_leds.set_animation(ANIM_OFF)` from exit logic
- Animation now persists when exiting Animations screen

**`screen_renderer.cpp`**
- Updated hints to show "Short:Next" and "Long:Exit"
- Clearer user guidance

## Screen Display

### Animations Screen Shows:
```
Animation:
Rainbow          Short:Next
                 Long:Exit
```

## Examples

### Example 1: Set Rainbow and Keep It
1. Enter Animations menu
2. Cycle to "Rainbow"
3. Exit → Rainbow continues
4. Navigate to Propose → Rainbow still running
5. Navigate to OTA → Rainbow still running

### Example 2: Turn Off LEDs
1. Enter Animations menu
2. Cycle to "Off"
3. Exit → LEDs turn off

### Example 3: Change Animation
1. Currently running "Fire"
2. Enter Animations menu
3. Cycle to "Breathing"
4. Exit → Now running "Breathing"

## Technical Details

### LED Update
- LEDs update at ~60 FPS in main loop
- `g_leds.update()` called every frame
- Independent of current screen state

### Memory
- Animation state persists in `LedController`
- Current animation: `g_leds.get_animation()`
- No additional memory overhead

### Performance
- No impact on other screens
- LED updates are non-blocking
- Display refresh unaffected

## Benefits

✅ **Set and forget** - Choose animation once, runs everywhere  
✅ **Explicit control** - Only stops when you select "Off"  
✅ **Background operation** - LEDs work while using other features  
✅ **Visual feedback** - See animation name and controls on screen  

## Use Cases

### Ambient Lighting
- Set "Breathing" or "Rainbow Cycle"
- Use device normally
- LEDs provide ambient lighting

### Status Indicator
- Set "Green" when ready
- Set "Red" when busy
- Set "Blue" when waiting

### Party Mode
- Set "Rainbow" or "Fire"
- LEDs run continuously
- No need to stay in Animations menu

## Power Consideration

**Note**: LEDs consume power continuously when animation is running.

- **30 LEDs at 50% brightness**: ~900 mA
- **Recommendation**: Select "Off" when not needed to save power
- **Battery life**: Will drain faster with LEDs on

## Future Enhancements (Optional)

### Possible Additions:
1. **Auto-off timer** - LEDs turn off after X minutes
2. **Screen-specific animations** - Different animation per screen
3. **Brightness control** - Adjust LED brightness from menu
4. **Speed control** - Adjust animation speed
5. **Custom colors** - RGB color picker

These can be added later if needed!

## Compile and Test

Run:
```bash
platformio run --target upload
```

Then test:
1. Enter Animations menu
2. Select "Rainbow"
3. Exit and navigate to other screens
4. Verify LEDs keep running
5. Return to Animations and select "Off"
6. Verify LEDs turn off

Enjoy your persistent LED animations! 🌈✨
