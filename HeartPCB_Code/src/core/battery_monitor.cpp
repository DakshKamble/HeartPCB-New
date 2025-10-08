#include "battery_monitor.h"

// Global instance
BatteryMonitor g_battery;

void BatteryMonitor::init() {
    // Configure ADC pin
    
    // Configure charging detection pin
    pinMode(BATTERY_CHARGING_PIN, INPUT_PULLUP);  // Pull up, pin will be LOW when charging
    
    // Configure ADC for 14-bit resolution
    analogReadResolution(14);  // Set to 14-bit resolution (0-16383)
    
    // Initial reading
    update();
    
    Serial.println("Battery Monitor initialized");
    Serial.println(BATTERY_ADC_PIN);
    Serial.print("Charging Pin: ");
    Serial.println(BATTERY_CHARGING_PIN);
    Serial.print("Voltage Divider Factor: ");
    Serial.println(VOLTAGE_DIVIDER_FACTOR);
}

void BatteryMonitor::update() {
    unsigned long now = millis();
    
    // Update at specified interval
    if (now - last_update < UPDATE_INTERVAL) {
        return;
    }
    last_update = now;
    
    // Read and calculate voltage
    battery_voltage = read_voltage();
    battery_percentage = calculate_percentage(battery_voltage);
}

float BatteryMonitor::read_voltage() {
    // Read ADC value (average of multiple readings for stability)
    const int num_samples = 10;
    int adc_sum = 0;
    
    for (int i = 0; i < num_samples; i++) {
        adc_sum += analogRead(BATTERY_ADC_PIN);
        delay(1);
    }
    
    raw_adc_value = adc_sum / num_samples;
    
    // Convert ADC value to voltage
    // Formula: Voltage = (ADC_value / ADC_max) * Vref * divider_factor
    float adc_voltage = (raw_adc_value / ADC_RESOLUTION) * ADC_REFERENCE_VOLTAGE;
    float actual_voltage = adc_voltage * VOLTAGE_DIVIDER_FACTOR;
    
    return actual_voltage;
}

int BatteryMonitor::calculate_percentage(float voltage) {
    // Clamp voltage to valid range
    if (voltage < BATTERY_MIN_VOLTAGE) {
        return 0;
    }
    if (voltage > BATTERY_MAX_VOLTAGE) {
        return 100;
    }
    
    // Linear interpolation between min and max
    float percentage = ((voltage - BATTERY_MIN_VOLTAGE) / 
                       (BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE)) * 100.0;
    
    return (int)percentage;
}

bool BatteryMonitor::is_charging() const {
    // Read charging pin (LOW when charging)
    return digitalRead(BATTERY_CHARGING_PIN) == LOW;
}

const char* BatteryMonitor::get_status_text() const {
    if (battery_voltage < BATTERY_CRITICAL) {
        return "CRITICAL";
    } else if (battery_percentage < 20) {
        return "LOW";
    } else if (battery_percentage < 50) {
        return "MEDIUM";
    } else if (is_charging()) {
        return "CHARGING";
    } else {
        return "GOOD";
    }
}

void BatteryMonitor::print_status() const {
    Serial.println("========== BATTERY STATUS ==========");
    Serial.print("Raw ADC Value: ");
    Serial.println(raw_adc_value);
    
    // Calculate ADC voltage (before divider correction)
    float adc_voltage = (raw_adc_value / ADC_RESOLUTION) * ADC_REFERENCE_VOLTAGE;
    Serial.print("ADC Voltage (before divider): ");
    Serial.print(adc_voltage, 3);
    Serial.println(" V");
    
    Serial.print("Battery Voltage (after divider): ");
    Serial.print(battery_voltage, 2);
    Serial.println(" V");
    Serial.print("Battery Percentage: ");
    Serial.print(battery_percentage);
    Serial.println(" %");
    Serial.print("Status: ");
    Serial.println(get_status_text());
    Serial.println("====================================");
}
