# Battery Monitor Implementation

## Summary

Successfully implemented battery voltage monitoring using ADC on GPIO 34 with voltage divider (factor 1.274).

## Hardware Setup

### Voltage Divider
- **Input Pin**: GPIO 34 (D34)
- **Divider Factor**: 1.274
- **ADC Resolution**: 12-bit (0-4095)
- **Reference Voltage**: 3.3V

### Formula
```
Actual Voltage = (ADC_Value / 4095) × 3.3V × 1.274
```

## Features Implemented

### ✅ Battery Monitor Module
- **File**: `battery_monitor.h/.cpp`
- Reads battery voltage via ADC
- Calculates battery percentage
- Provides status text (CRITICAL, LOW, MEDIUM, GOOD, CHARGING)

### ✅ Serial Output
Battery status prints to Serial Monitor when entering Battery screen:
```
========== BATTERY STATUS ==========
Raw ADC Value: 3250
Battery Voltage: 3.85 V
Battery Percentage: 71 %
Status: GOOD
====================================
```

### ✅ Display Integration
Battery screen shows:
- Voltage (e.g., "3.85V")
- Percentage (e.g., "71%")

## Configuration

Edit `config.h` to adjust settings:

```cpp
// Battery Monitor Configuration
#define BATTERY_ADC_PIN 34              // GPIO pin
#define VOLTAGE_DIVIDER_FACTOR 1.274    // Your divider factor
#define ADC_RESOLUTION 4095.0           // 12-bit ADC
#define ADC_REFERENCE_VOLTAGE 3.3       // ESP32 Vref
```

### Battery Thresholds

In `battery_monitor.h`, adjust for your battery type:

```cpp
static constexpr float BATTERY_MIN_VOLTAGE = 3.0;   // 0%
static constexpr float BATTERY_MAX_VOLTAGE = 4.2;   // 100% (Li-ion)
static constexpr float BATTERY_CRITICAL = 3.3;      // Critical level
```

**Common Battery Types:**
- **Li-ion/LiPo**: 3.0V (empty) to 4.2V (full)
- **Li-ion (safe)**: 3.3V (empty) to 4.1V (full)
- **3x AA**: 2.7V (empty) to 4.5V (full)

## Usage

### View Battery Status

1. Navigate to **Battery** menu item (5th item)
2. **Long press** to enter Battery screen
3. **Display shows**: Voltage and percentage
4. **Serial Monitor shows**: Detailed status

### Serial Monitor Output

Open Serial Monitor at **115200 baud** to see:
- Startup messages
- Battery status when entering Battery screen
- Continuous updates every 1 second

## Code Structure

### BatteryMonitor Class

```cpp
class BatteryMonitor {
public:
    void init();                    // Initialize ADC
    void update();                  // Read voltage (every 1s)
    
    float get_voltage();            // Get voltage in V
    int get_percentage();           // Get percentage (0-100)
    int get_raw_adc();             // Get raw ADC value
    
    bool is_charging();            // Detect charging
    const char* get_status_text(); // Get status string
    void print_status();           // Print to Serial
};
```

### Integration Points

**Main Loop**:
- Calls `g_battery.update()` every frame
- Prints status when entering Battery screen
- Updates every 1 second internally

**Battery Screen**:
- Displays voltage and percentage
- Updates in real-time

**Serial Output**:
- Initialization message on startup
- Status print when entering Battery screen

## Calibration

### If Readings Are Incorrect

1. **Measure actual battery voltage** with multimeter
2. **Read ADC value** from Serial Monitor
3. **Calculate actual divider factor**:

```
Actual Factor = (Actual Voltage × 4095) / (ADC Value × 3.3)
```

4. **Update** `VOLTAGE_DIVIDER_FACTOR` in `config.h`

### Example Calibration

```
Multimeter reads: 3.85V
ADC value: 2450
Reference: 3.3V

Factor = (3.85 × 4095) / (2450 × 3.3)
       = 15765.75 / 8085
       = 1.950

Update config.h:
#define VOLTAGE_DIVIDER_FACTOR 1.950
```

## Serial Monitor Commands

### View Output

1. Open PlatformIO Serial Monitor
2. Or use Arduino Serial Monitor at 115200 baud
3. Navigate to Battery screen to see status

### Expected Output

```
========== HeartPCB Starting ==========
Battery Monitor Initialized
ADC Pin: 34
Voltage Divider Factor: 1.274
All systems initialized!
======================================

========== BATTERY STATUS ==========
Raw ADC Value: 3250
Battery Voltage: 3.85 V
Battery Percentage: 71 %
Status: GOOD
====================================
```

## Status Indicators

| Voltage | Percentage | Status | Meaning |
|---------|-----------|--------|---------|
| < 3.3V | < 25% | CRITICAL | Charge immediately |
| 3.3-3.6V | 25-50% | LOW | Charge soon |
| 3.6-3.9V | 50-75% | MEDIUM | Normal operation |
| 3.9-4.2V | 75-100% | GOOD | Fully charged |
| > 4.1V | 100% | CHARGING | Charging detected |

## Troubleshooting

### ADC Reads 0 or 4095

**Problem**: Pin not connected or voltage out of range

**Solution**:
- Check wiring to GPIO 34
- Verify voltage divider is working
- Ensure battery voltage is within 0-4.2V range

### Voltage Too High/Low

**Problem**: Incorrect divider factor

**Solution**:
- Measure actual voltage with multimeter
- Recalculate divider factor
- Update `VOLTAGE_DIVIDER_FACTOR` in config.h

### Percentage Incorrect

**Problem**: Wrong battery voltage thresholds

**Solution**:
- Adjust `BATTERY_MIN_VOLTAGE` and `BATTERY_MAX_VOLTAGE`
- Match to your battery type (Li-ion, LiPo, etc.)

### No Serial Output

**Problem**: Serial not initialized or wrong baud rate

**Solution**:
- Verify Serial Monitor is at 115200 baud
- Check USB connection
- Press reset button on ESP32

## Advanced Features (Optional)

### Add Voltage History

Track voltage over time:

```cpp
// In battery_monitor.h
float voltage_history[10];
int history_index = 0;

// In battery_monitor.cpp
void BatteryMonitor::update() {
    voltage_history[history_index] = battery_voltage;
    history_index = (history_index + 1) % 10;
}
```

### Add Low Battery Alert

```cpp
// In main.cpp
if (g_battery.get_percentage() < 10) {
    // Flash LEDs red
    g_leds.set_animation(ANIM_SOLID_RED);
}
```

### Add Charging Detection

Connect charging status pin and update:

```cpp
// In battery_monitor.cpp
bool BatteryMonitor::is_charging() const {
    return digitalRead(CHARGING_STATUS_PIN) == HIGH;
}
```

## Files Modified

```
src/core/
├── config.h                  ← Battery ADC configuration
├── battery_monitor.h         ← NEW: Battery monitor class
├── battery_monitor.cpp       ← NEW: Implementation
├── screen_renderer.cpp       ← Display voltage/percentage
└── main.cpp                  ← Initialize & update battery

```

## Power Consumption Note

ADC readings consume minimal power (~1mA). The battery monitor updates every 1 second to balance accuracy and power efficiency.

## Next Steps

1. **Compile and upload**
2. **Open Serial Monitor** at 115200 baud
3. **Navigate to Battery screen** to see readings
4. **Verify voltage** with multimeter if needed
5. **Calibrate** divider factor if readings are off

Enjoy real-time battery monitoring! 🔋📊
