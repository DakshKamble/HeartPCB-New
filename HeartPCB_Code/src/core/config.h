#pragma once

// Hardware Configuration
#define BUTTON_PIN D1
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32

// Battery Monitor Configuration
#define BATTERY_ADC_PIN A0
#define BATTERY_CHARGING_PIN D2  // STAT pin from charging module (HIGH when charging)
#define VOLTAGE_DIVIDER_FACTOR 1.27447
#define ADC_RESOLUTION 16383.0  // 14-bit ADC (2^14 - 1)
#define ADC_REFERENCE_VOLTAGE 3.3 // XIAO RA4M1 reference voltage
#define BATTERY_LED_BRIGHTNESS 77 // Built-in LED brightness (0-255, 30% = 77)

// LED Strip Configuration
#define LED_PIN D3
#define NUM_LEDS 21
#define LED_BRIGHTNESS 64

// DAC Configuration (MCP4922)
#define DAC_CS_PIN D7
#define DAC_UPDATE_INTERVAL_US 20  // 20 microseconds for smooth heart drawing

// Timing Configuration
#define SPLASH_DURATION_MS 2000
#define PROPOSE_DISPLAY_DURATION_MS 10000
#define BUTTON_DEBOUNCE_MS 50
#define LONG_PRESS_THRESHOLD_MS 500
#define HEART_FLASH_INTERVAL_MS 500
#define MAIN_LOOP_DELAY_MS 10

// Display Configuration
#define SPLASH_TEXT "Hi Gargi"
#define SPLASH_TEXT_X 2
#define SPLASH_TEXT_Y 25

// Menu Configuration
#define MENU_ITEM_COUNT 4
#define MENU_ITEM_PROPOSE "Propose"
#define MENU_ITEM_OSCILLOSCOPE "Oscilloscope"
#define MENU_ITEM_ANIMATIONS "Animations"
#define MENU_ITEM_BATTERY "Battery"

// Propose Screen Configuration
#define PROPOSE_WAITING_TEXT "Press to Accept"
#define PROPOSE_LOVE_TEXT "I LOVE YOU"

// Oscilloscope Screen Configuration
#define OSCILLOSCOPE_TITLE "Probe me !"


// Animations Screen Configuration
#define ANIMATIONS_TITLE "Animations Mode"
#define ANIMATIONS_EXIT_TEXT "Press to exit"

// Battery Screen Configuration
#define BATTERY_TITLE "Battery Info"
#define BATTERY_EXIT_TEXT "Press to exit"
