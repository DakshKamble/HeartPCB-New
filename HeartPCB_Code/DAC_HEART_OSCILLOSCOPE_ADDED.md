# DAC Heart Oscilloscope Integration

## Summary

Successfully integrated MCP4922 DAC controller to output heart waveform for XY oscilloscope display. The heart waveform is now generated when the "Oscilloscope" menu item is selected.

## Features Added

### ✅ MCP4922 DAC Controller
- **File**: `dac_controller.h/.cpp`
- Generates smooth heart waveform on XY oscilloscope
- 4000-point precomputed heart curve
- 20 microsecond update interval for smooth drawing
- Automatic start/stop based on oscilloscope mode

### ✅ Oscilloscope Mode Integration
- **Display Text**: "Probe me !" (as requested)
- **DAC Output**: Heart waveform active only when in oscilloscope mode
- **Auto-Control**: Starts when entering, stops when exiting oscilloscope

### ✅ Heart Waveform Mathematics
Based on the parametric heart equation:
```
X = 16 * sin³(t)
Y = 13*cos(t) - 5*cos(2t) - 2*cos(3t) - cos(4t)
```

Scaled and centered for 12-bit DAC output (0-4095):
- **X Range**: ~0-4096 (scaled by 128)
- **Y Range**: ~0-4080 (scaled by 120)

## Hardware Configuration

### MCP4922 DAC Setup
- **Chip Select Pin**: D7 (configurable in `config.h`)
- **SPI Interface**: 20 MHz, MSB first, Mode 0
- **Resolution**: 12-bit (4096 levels)
- **Channels**: A (X-axis), B (Y-axis)

### Oscilloscope Connection
```
MCP4922 DAC A (X) → Oscilloscope CH1 (X input)
MCP4922 DAC B (Y) → Oscilloscope CH2 (Y input)
Set oscilloscope to XY mode for heart display
```

## Usage

### Navigation to Heart Display
1. Navigate to **Oscilloscope** menu item (2nd item)
2. **Long press** to enter oscilloscope mode
3. **Display shows**: "Probe me !"
4. **DAC outputs**: Continuous heart waveform
5. **Short press** to exit back to menu

### Oscilloscope Settings
- **Mode**: XY (not time-based)
- **Voltage/div**: Adjust to see full heart shape
- **Coupling**: DC
- **Trigger**: Off (continuous display)

## Technical Implementation

### Files Created
```
src/core/
├── dac_controller.h      ← DAC controller class definition
└── dac_controller.cpp    ← Heart waveform implementation
```

### Files Modified
```
src/core/config.h         ← Added DAC pin and timing configuration
src/core/state_manager.cpp ← Added DAC start/stop on state transitions
src/main.cpp              ← Added DAC initialization and update calls
```

### Configuration Options

Edit `src/core/config.h`:

```cpp
// DAC Configuration (MCP4922)
#define DAC_CS_PIN D7                    // Chip Select pin
#define DAC_UPDATE_INTERVAL_US 20        // Update interval in microseconds
```

### Heart Waveform Parameters

In `dac_controller.cpp`:

```cpp
const int N = 4000;                      // Number of points (higher = smoother)
// Scaling factors for DAC range
X[i] = (uint16_t)((tempX + 16) * 128);   // X scaling
Y[i] = (uint16_t)((tempY + 17) * 120);   // Y scaling
```

## Performance

- **Update Rate**: 50 kHz (20 μs intervals)
- **Heart Refresh**: ~12.5 Hz (4000 points ÷ 50 kHz)
- **CPU Impact**: Minimal, only active in oscilloscope mode
- **Memory**: 16 KB for waveform arrays (4000 × 2 × 2 bytes)

## Customization

### Adjust Heart Size
Modify scaling factors in `precompute_heart_waveform()`:

```cpp
// Larger heart
X[i] = (uint16_t)((tempX + 16) * 150);   // Increase multiplier
Y[i] = (uint16_t)((tempY + 17) * 140);   // Increase multiplier

// Smaller heart  
X[i] = (uint16_t)((tempX + 16) * 100);   // Decrease multiplier
Y[i] = (uint16_t)((tempY + 17) * 90);    // Decrease multiplier
```

### Adjust Drawing Speed
Change update interval in `config.h`:

```cpp
// Faster drawing (smoother but higher CPU)
#define DAC_UPDATE_INTERVAL_US 10        // 10 μs = 100 kHz

// Slower drawing (lower CPU usage)
#define DAC_UPDATE_INTERVAL_US 50        // 50 μs = 20 kHz
```

### Change Heart Resolution
Modify point count in `dac_controller.h`:

```cpp
// Higher resolution (smoother curve)
static const int N = 8000;

// Lower resolution (less memory)
static const int N = 2000;
```

## Oscilloscope Display Tips

### Getting the Best Heart Shape

1. **Set XY Mode**: Essential for proper heart display
2. **Adjust Voltage Scale**: Start with 1V/div on both channels
3. **Center the Display**: Use position controls to center the heart
4. **Fine-tune Scale**: Adjust until heart fills ~80% of screen
5. **Check Connections**: Ensure proper DAC to oscilloscope wiring

### Troubleshooting

**No Display on Oscilloscope**:
- Verify XY mode is enabled
- Check DAC power supply (3.3V)
- Confirm SPI connections (MOSI, SCK, CS)
- Test with multimeter on DAC outputs

**Distorted Heart Shape**:
- Adjust oscilloscope voltage scales
- Check for ground loops
- Verify DAC reference voltage
- Try different update intervals

**Heart Not Centered**:
- Modify offset values in scaling equations
- Use oscilloscope position controls
- Check DAC output voltage range

## Integration with HeartPCB Architecture ✅

This implementation follows the clean architecture principles:

- ✅ **Self-contained module** (`DACController`)
- ✅ **Configuration in `config.h`**
- ✅ **State-managed activation** (only in oscilloscope mode)
- ✅ **No cross-module dependencies**
- ✅ **Easy to extend** with new waveforms
- ✅ **Proper resource management** (start/stop on state transitions)

## Future Enhancements (Optional)

### Possible Additions

1. **Multiple Waveforms**: Add sine, square, triangle waves
2. **Frequency Control**: Adjust heart drawing speed from menu
3. **Amplitude Control**: Scale heart size dynamically
4. **Lissajous Patterns**: Additional mathematical curves
5. **Waveform Menu**: Select different patterns in oscilloscope mode

### Advanced Features

1. **Function Generator Mode**: Complete signal generator functionality
2. **Frequency Sweep**: Automated frequency changes
3. **Modulation**: AM/FM capabilities
4. **Preset Patterns**: Save/load custom waveforms

## Romantic Engineering Note 💕

This heart waveform feature adds a beautiful technical demonstration to your romantic device. When Gargi sees the perfect mathematical heart curve on the oscilloscope screen, she'll know that your love is both heartfelt AND precisely engineered! 

The "Probe me !" message adds a playful technical twist that any engineering-minded person would appreciate.

## Compile and Test

Run:
```bash
platformio run --target upload
```

Then:
1. Navigate to **Oscilloscope** menu
2. **Long press** to enter
3. Connect oscilloscope probes to MCP4922 outputs
4. Set oscilloscope to XY mode
5. Enjoy the perfect heart display! 💖

**By Daksh (Gargi's boyfriend)** - Engineering love, one waveform at a time! ✨
