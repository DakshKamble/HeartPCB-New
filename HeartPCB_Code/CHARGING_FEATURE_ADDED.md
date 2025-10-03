# Battery Charging Feature Implementation

## Overview

Added battery charging detection and visual indicators using GPIO pin D23 connected to the STAT pin of the charging module.

## Hardware Configuration

### Pin Assignment
- **GPIO 23 (D23)**: Connected to STAT pin of charging module
- **Logic**: HIGH when battery is charging, LOW when not charging
- **Pull-down**: Internal pull-down resistor enabled

### Charging Module Integration
```
Charging Module STAT Pin → GPIO 23 (D23)
- HIGH: Battery is charging
- LOW: Battery not charging or fully charged
```

## Software Implementation

### Configuration Added
```cpp
// In config.h
#define BATTERY_CHARGING_PIN 23  // STAT pin from charging module
```

### Battery Monitor Updates

#### Initialization
```cpp
void BatteryMonitor::init() {
    // Configure charging detection pin
    pinMode(BATTERY_CHARGING_PIN, INPUT_PULLDOWN);
    // ... existing code
}
```

#### Charging Detection
```cpp
bool BatteryMonitor::is_charging() const {
    // Read charging pin (HIGH when charging)
    return digitalRead(BATTERY_CHARGING_PIN) == HIGH;
}
```

## Visual Indicators

### 1. Menu Battery Icon (Top Right)

#### Normal State
- Static battery outline
- Fill level based on percentage
- No additional indicators

#### Charging State
- **Pulsing Animation**: Battery fill pulses from empty to full
- **Animation Speed**: 200ms per step (8 steps total)
- **Charging Indicator**: "C" symbol next to battery
- **Visual Effect**: Continuous pulsing to show active charging

### 2. Battery Info Screen

#### Normal State
- Large battery frame
- Static fill based on percentage
- Percentage display on right

#### Charging State
- **Wave Animation**: Moving wave pattern inside battery
- **Animation Speed**: 100ms per step
- **Text Display**: "CHARGING" text above percentage
- **Visual Effect**: Flowing wave animation showing energy flow

## Animation Details

### Menu Icon Animation
```cpp
// Pulsing fill animation
static int anim_step = 0;
int anim_width = (anim_step * 7) / 7;  // 0 to 7 pixels
anim_step = (anim_step + 1) % 8;       // 8-step cycle
```

### Battery Screen Animation
```cpp
// Wave effect animation  
static int wave_offset = 0;
for (int i = 0; i < 48; i += 4) {
    int wave_pos = (i + wave_offset) % 48;
    u8g2->drawBox(11 + wave_pos, 9, 2, 14);  // Moving bars
}
wave_offset = (wave_offset + 1) % 48;
```

## Status Text Integration

The battery status text now includes charging state:
```cpp
const char* BatteryMonitor::get_status_text() const {
    if (battery_voltage < BATTERY_CRITICAL) {
        return "CRITICAL";
    } else if (battery_percentage < 20) {
        return "LOW";
    } else if (battery_percentage < 50) {
        return "MEDIUM";
    } else if (is_charging()) {
        return "CHARGING";  // ← New charging status
    } else {
        return "GOOD";
    }
}
```

## Usage

### Testing with Switch
1. Connect switch between GPIO 23 and 3.3V
2. Switch ON = Charging (animations active)
3. Switch OFF = Not charging (normal display)

### Production with Charging Module
1. Connect charging module STAT pin to GPIO 23
2. STAT pin goes HIGH during charging
3. Automatic detection and animation display

## Visual Behavior

### Menu Screen
```
Normal:     [████▒▒▒] 60%
Charging:   [▒▒▒▒▒▒▒] C  ← Pulsing animation + "C" indicator
```

### Battery Screen
```
Normal:
┌─────────────────────────────────────────────────────┐
│████████████████████████████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒│  60%
└─────────────────────────────────────────────────────┘

Charging:
┌─────────────────────────────────────────────────────┐  CHARGING
│██▒▒██▒▒██▒▒██▒▒██▒▒██▒▒██▒▒██▒▒██▒▒██▒▒██▒▒██▒▒██│  60%
└─────────────────────────────────────────────────────┘
    ↑ Moving wave animation
```

## Technical Details

### Performance
- **CPU Impact**: Minimal (simple GPIO read + animation timers)
- **Memory Usage**: No additional RAM required
- **Update Rate**: 
  - Menu icon: 200ms per animation step
  - Battery screen: 100ms per animation step

### Power Consumption
- GPIO read: Negligible power consumption
- Animation rendering: Same as normal display updates
- No impact on battery life measurement

### Reliability
- Hardware-based detection (not voltage heuristics)
- Debounced through charging module STAT pin
- Real-time status updates

## Error Handling

### Pin Configuration
```cpp
// Pull-down ensures clean LOW state when not charging
pinMode(BATTERY_CHARGING_PIN, INPUT_PULLDOWN);
```

### Fallback Behavior
- If charging pin fails: Shows normal battery display
- No system crashes or undefined behavior
- Graceful degradation

## Future Enhancements

### Possible Improvements
1. **Charging Rate Display**: Show charging current/power
2. **Time to Full**: Estimate charging completion time
3. **Charging History**: Log charging sessions
4. **Temperature Monitoring**: Add battery temperature sensing
5. **Smart Charging**: Implement charging optimization

### Advanced Features
1. **Wireless Charging Detection**: Support for Qi charging pads
2. **Multi-Stage Charging**: Different animations for different charging phases
3. **Charging Sounds**: Audio feedback during charging
4. **LED Charging Indicator**: Use heart LEDs to show charging status

## Troubleshooting

### Common Issues

#### No Charging Animation
1. Check GPIO 23 connection
2. Verify charging module STAT pin output
3. Test with multimeter (should be 3.3V when charging)

#### Incorrect Charging Detection
1. Ensure proper pull-down configuration
2. Check for loose connections
3. Verify charging module compatibility

#### Animation Not Smooth
1. Check system performance
2. Verify timer intervals in code
3. Ensure adequate power supply

### Debug Commands
```cpp
// Add to serial output for debugging
Serial.print("Charging Pin State: ");
Serial.println(digitalRead(BATTERY_CHARGING_PIN));
Serial.print("Is Charging: ");
Serial.println(g_battery.is_charging() ? "YES" : "NO");
```

## Compatibility

### Charging Modules
- TP4056 charging module (STAT pin)
- MCP73831 charging IC (STAT pin)  
- Any charging IC with open-drain STAT output

### Pin Requirements
- 3.3V logic level
- Open-drain or push-pull output
- Current capability: <1mA (GPIO input)

Perfect for your romantic proposal device with professional charging indication! 🔋💕✨
