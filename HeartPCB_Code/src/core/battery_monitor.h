#pragma once
#include <Arduino.h>
#include "config.h"

// Battery monitor class
class BatteryMonitor {
public:
    void init();
    void update();
    
    // Voltage readings
    float get_voltage() const { return battery_voltage; }
    int get_percentage() const { return battery_percentage; }
    int get_raw_adc() const { return raw_adc_value; }
    
    // Battery status
    bool is_charging() const;
    const char* get_status_text() const;
    
    // Print to serial
    void print_status() const;
    
private:
    float battery_voltage = 0.0;
    int battery_percentage = 0;
    int raw_adc_value = 0;
    
    unsigned long last_update = 0;
    static const unsigned long UPDATE_INTERVAL = 1000;  // Update every 1 second
    
    // Battery voltage thresholds (adjust for your battery type)
    static constexpr float BATTERY_MIN_VOLTAGE = 3.0;   // 0%
    static constexpr float BATTERY_MAX_VOLTAGE = 4.2;   // 100%
    static constexpr float BATTERY_CRITICAL = 3.3;      // Critical level
    
    // Helper functions
    float read_voltage();
    int calculate_percentage(float voltage);
};

// Global battery monitor instance
extern BatteryMonitor g_battery;
