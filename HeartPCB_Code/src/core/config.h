#pragma once

// Hardware Configuration
#define BUTTON_PIN 5
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32

// Battery Monitor Configuration
#define BATTERY_ADC_PIN 34
#define BATTERY_CHARGING_PIN 23  // STAT pin from charging module (HIGH when charging)
#define VOLTAGE_DIVIDER_FACTOR 1.274
#define ADC_RESOLUTION 4095.0  // 12-bit ADC
#define ADC_REFERENCE_VOLTAGE 3.3  // ESP32 reference voltage

// LED Strip Configuration
#define LED_PIN 2
#define NUM_LEDS 21
#define LED_BRIGHTNESS 128
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

// Timing Configuration
#define SPLASH_DURATION_MS 2000
#define PROPOSE_DISPLAY_DURATION_MS 10000
#define BUTTON_DEBOUNCE_MS 50
#define LONG_PRESS_THRESHOLD_MS 500
#define HEART_FLASH_INTERVAL_MS 500
#define MAIN_LOOP_DELAY_MS 10

// Display Configuration
#define SPLASH_TEXT "Hi Daksh"
#define SPLASH_TEXT_X 2
#define SPLASH_TEXT_Y 25

// Menu Configuration
#define MENU_ITEM_COUNT 5
#define MENU_ITEM_PROPOSE "Propose"
#define MENU_ITEM_OSCILLOSCOPE "Oscilloscope"
#define MENU_ITEM_OTA "OTA Update"
#define MENU_ITEM_ANIMATIONS "Animations"
#define MENU_ITEM_BATTERY "Battery"

// Propose Screen Configuration
#define PROPOSE_WAITING_TEXT "Press to Accept"
#define PROPOSE_LOVE_TEXT "I LOVE YOU"

// Oscilloscope Screen Configuration
#define OSCILLOSCOPE_TITLE "Entered Oscilloscope"
#define OSCILLOSCOPE_EXIT_TEXT "Press to exit"

// OTA Update Screen Configuration
#define OTA_TITLE "OTA Update Mode"
#define OTA_EXIT_TEXT "Press to exit"

// Animations Screen Configuration
#define ANIMATIONS_TITLE "Animations Mode"
#define ANIMATIONS_EXIT_TEXT "Press to exit"

// Battery Screen Configuration
#define BATTERY_TITLE "Battery Info"
#define BATTERY_EXIT_TEXT "Press to exit"
