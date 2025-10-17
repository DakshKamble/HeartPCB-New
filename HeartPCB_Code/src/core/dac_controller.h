#pragma once
#include <Arduino.h>
#include <SPI.h>
#include "config.h"

// DAC controller for MCP4922 - Heart waveform on XY oscilloscope
// By Daksh (Gargi's boyfriend)
class DACController {
public:
    void init();
    void update();
    void start_heart_output();
    void stop_heart_output();
    bool is_active() const { return active; }
    
private:
    bool active = false;
    bool initialized = false;
    int current_point = 0;
    unsigned long last_update = 0;
    
    // Heart waveform data
    static const int N = 4000;  // number of points in the waveform
    uint16_t X[N];
    uint16_t Y[N];
    
    // Helper functions
    void writeMCP4922(bool channelB, uint16_t value);
    void precompute_heart_waveform();
};

// Global DAC controller instance
extern DACController g_dac;
