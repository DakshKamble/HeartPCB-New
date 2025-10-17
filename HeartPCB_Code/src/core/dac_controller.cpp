#include "dac_controller.h"

// Global instance
DACController g_dac;

const float pi = 3.1415926;

void DACController::init() {
    // Configure SPI for MCP4922 - exactly like your working code
    pinMode(DAC_CS_PIN, OUTPUT);
    digitalWrite(DAC_CS_PIN, HIGH);
    SPI.begin();
    SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
    
    // Precompute the heart waveform
    precompute_heart_waveform();
    
    initialized = true;
    active = false;
    current_point = 0;
    
    Serial.println("DAC Controller initialized - Heart waveform ready");
    Serial.print("DAC CS Pin: ");
    Serial.println(DAC_CS_PIN);
    Serial.print("Waveform points: ");
    Serial.println(N);
}

void DACController::precompute_heart_waveform() {
    float rStep = 2 * pi / N;
    float r = 0;
    
    // Precompute the heart curve and scale to 12-bit (0–4095)
    for (int i = 0; i < N; i++) {
        double tempX = 16 * pow(sin(r), 3);
        double tempY = 13 * cos(r) - 5 * cos(2 * r) - 2 * cos(3 * r) - cos(4 * r);
        
        // Scale and center into DAC range
        X[i] = (uint16_t)((tempX + 16) * 128);  // ~0–4096 range
        Y[i] = (uint16_t)((tempY + 17) * 120);  // adjust scaling to fill screen
        
        r += rStep;
    }
}

void DACController::writeMCP4922(bool channelB, uint16_t value) {
    if (!initialized) return;
    
    value &= 0x0FFF;  // Ensure 12-bit value
    uint16_t command = (channelB ? 0xB000 : 0x7000) | value;
    
    // Use proper SPI transaction for MCP4922 - exactly like your working code
    digitalWrite(DAC_CS_PIN, LOW);
    SPI.transfer16(command);
    digitalWrite(DAC_CS_PIN, HIGH);
}

void DACController::update() {
    if (!initialized || !active) {
        return;
    }
    
    // Simple update - just output the next point (timing handled by main loop)
    writeMCP4922(false, X[current_point]); // DAC A = X
    writeMCP4922(true, Y[current_point]);  // DAC B = Y
    
    // Move to next point
    current_point = (current_point + 1) % N;
}

void DACController::start_heart_output() {
    if (!initialized) return;
    
    active = true;
    current_point = 0;
    last_update = micros();
    
    Serial.println("DAC: Starting heart waveform output");
    Serial.println("Connect oscilloscope to XY mode:");
    Serial.println("- X input <- MCP4922 VOUTA");
    Serial.println("- Y input <- MCP4922 VOUTB");
    Serial.println("- Common ground");
    
    // Test output - send a few test points
    Serial.println("Sending test points...");
    for (int i = 0; i < 10; i++) {
        Serial.print("Point ");
        Serial.print(i);
        Serial.print(": X=");
        Serial.print(X[i]);
        Serial.print(", Y=");
        Serial.println(Y[i]);
    }
}

void DACController::stop_heart_output() {
    if (!initialized) return;
    
    active = false;
    
    // Set both DAC outputs to center (2048 = ~1.65V for 3.3V reference)
    writeMCP4922(false, 2048); // DAC A = X center
    writeMCP4922(true, 2048);  // DAC B = Y center
    
    Serial.println("DAC: Heart waveform output stopped");
}
