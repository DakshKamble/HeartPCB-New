#include "state_manager.h"
#include "led_controller.h"

// Global instance
StateManager g_state;

void StateManager::init() {
    current_state = STATE_SPLASH;
    menu_selection = 0;
    invert_display = false;
    splash_start_time = millis();
    state_enter_time = splash_start_time;
}

void StateManager::update() {
    auto_transitions();
}

void StateManager::transition_to(AppState new_state) {
    current_state = new_state;
    state_enter_time = millis();
    invert_display = false;
}

void StateManager::auto_transitions() {
    unsigned long now = millis();
    
    switch (current_state) {
        case STATE_SPLASH:
            if (now - splash_start_time >= SPLASH_DURATION_MS) {
                transition_to(STATE_MENU);
            }
            break;
            
        case STATE_PROPOSE_DISPLAYING:
            if (now - state_enter_time >= PROPOSE_DISPLAY_DURATION_MS) {
                transition_to(STATE_MENU);
            }
            break;
            
        default:
            break;
    }
}

void StateManager::cycle_menu_selection() {
    menu_selection = (menu_selection + 1) % MENU_ITEM_COUNT;
}

void StateManager::cycle_animation_selection() {
    animation_selection = (animation_selection + 1) % ANIM_COUNT;
    g_leds.set_animation((LedAnimation)animation_selection);
}

void StateManager::handle_button_event(ButtonEvent event) {
    switch (event) {
        case BTN_SHORT_PRESS:
            if (current_state == STATE_MENU) {
                cycle_menu_selection();
            } else if (current_state == STATE_PROPOSE_WAITING) {
                transition_to(STATE_PROPOSE_DISPLAYING);
            } else if (current_state == STATE_OSCILLOSCOPE) {
                transition_to(STATE_MENU);
            } else if (current_state == STATE_OTA_UPDATE) {
                transition_to(STATE_MENU);
            } else if (current_state == STATE_ANIMATIONS) {
                cycle_animation_selection();
            }
            break;
            
        case BTN_LONG_PRESS_RELEASE:
            // Selection happens on release after long press
            if (current_state == STATE_MENU) {
                // Enter selected menu item
                if (menu_selection == 0) {
                    transition_to(STATE_PROPOSE_WAITING);
                } else if (menu_selection == 1) {
                    transition_to(STATE_OSCILLOSCOPE);
                } else if (menu_selection == 2) {
                    transition_to(STATE_OTA_UPDATE);
                } else if (menu_selection == 3) {
                    transition_to(STATE_ANIMATIONS);
                }
            } else if (current_state == STATE_ANIMATIONS) {
                // Long press in animations exits to menu
                g_leds.set_animation(ANIM_OFF);
                transition_to(STATE_MENU);
            }
            break;
            
        case BTN_LONG_PRESS_ACTIVE:
            // Visual feedback while holding
            invert_display = true;
            break;
            
        case BTN_NONE:
            invert_display = false;
            break;
    }
}

bool StateManager::should_show_heart() const {
    if (current_state != STATE_PROPOSE_DISPLAYING) {
        return false;
    }
    // Flash based on config interval
    return (millis() % (HEART_FLASH_INTERVAL_MS * 2)) < HEART_FLASH_INTERVAL_MS;
}
