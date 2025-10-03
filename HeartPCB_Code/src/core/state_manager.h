#pragma once
#include <Arduino.h>
#include "config.h"

// Application states
enum AppState {
    STATE_SPLASH,
    STATE_MENU,
    STATE_PROPOSE_WAITING,
    STATE_PROPOSE_DISPLAYING,
    STATE_OSCILLOSCOPE,
    STATE_OTA_UPDATE,
    STATE_ANIMATIONS,
    STATE_BATTERY
};

// Button events
enum ButtonEvent {
    BTN_NONE,
    BTN_SHORT_PRESS,
    BTN_LONG_PRESS_ACTIVE,  // Held down, for visual feedback
    BTN_LONG_PRESS_RELEASE  // Released after long press
};

// State manager class
class StateManager {
public:
    void init();
    void update();
    
    // State queries
    AppState get_state() const { return current_state; }
    bool is_splash() const { return current_state == STATE_SPLASH; }
    bool is_menu() const { return current_state == STATE_MENU; }
    bool is_propose_waiting() const { return current_state == STATE_PROPOSE_WAITING; }
    bool is_propose_displaying() const { return current_state == STATE_PROPOSE_DISPLAYING; }
    bool is_oscilloscope() const { return current_state == STATE_OSCILLOSCOPE; }
    bool is_ota_update() const { return current_state == STATE_OTA_UPDATE; }
    bool is_animations() const { return current_state == STATE_ANIMATIONS; }
    bool is_battery() const { return current_state == STATE_BATTERY; }
    
    // Menu selection
    int get_menu_selection() const { return menu_selection; }
    void cycle_menu_selection();
    
    // Animation selection
    int get_animation_selection() const { return animation_selection; }
    void cycle_animation_selection();
    
    // State transitions
    void handle_button_event(ButtonEvent event);
    
    // Visual feedback
    bool should_invert_display() const { return invert_display; }
    bool should_show_heart() const;
    
private:
    AppState current_state = STATE_SPLASH;
    int menu_selection = 0;
    int animation_selection = 0;
    bool invert_display = false;
    
    unsigned long state_enter_time = 0;
    unsigned long splash_start_time = 0;
    
    void transition_to(AppState new_state);
    void auto_transitions();
};

// Global state manager instance
extern StateManager g_state;
