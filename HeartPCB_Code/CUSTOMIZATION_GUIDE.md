# Customization Guide

This guide shows you how to customize the HeartPCB firmware without touching the core logic.

## Quick Customization (config.h)

All user-facing settings are in **`src/core/config.h`**. Edit this file to customize behavior.

### Change Splash Screen

```cpp
#define SPLASH_TEXT "Your Name"
#define SPLASH_TEXT_X 10
#define SPLASH_TEXT_Y 25
#define SPLASH_DURATION_MS 3000  // Show for 3 seconds
```

### Change Menu Items

```cpp
#define MENU_ITEM_PROPOSE "Marry Me?"
#define MENU_ITEM_OSCILLOSCOPE "Debug Mode"
```

### Change Propose Screen Text

```cpp
#define PROPOSE_WAITING_TEXT "Say Yes!"
#define PROPOSE_LOVE_TEXT "YOU'RE AMAZING"
#define PROPOSE_DISPLAY_DURATION_MS 15000  // Show for 15 seconds
```

### Change Button Behavior

```cpp
#define BUTTON_DEBOUNCE_MS 100           // Slower debounce
#define LONG_PRESS_THRESHOLD_MS 1000     // 1 second for long press
```

### Change Timing

```cpp
#define HEART_FLASH_INTERVAL_MS 250      // Faster heart flash
#define MAIN_LOOP_DELAY_MS 5             // Faster refresh rate
```

### Change Hardware Pins

```cpp
#define BUTTON_PIN 12                    // Use GPIO 12 instead
```

## Adding a New Screen

### 1. Add State (state_manager.h)

```cpp
enum AppState {
    STATE_SPLASH,
    STATE_MENU,
    STATE_PROPOSE_WAITING,
    STATE_PROPOSE_DISPLAYING,
    STATE_OSCILLOSCOPE,
    STATE_YOUR_NEW_SCREEN  // Add here
};
```

### 2. Add Menu Item (config.h)

```cpp
#define MENU_ITEM_COUNT 3  // Increase count
#define MENU_ITEM_YOUR_SCREEN "My Screen"
```

### 3. Add Transition Logic (state_manager.cpp)

In `StateManager::handle_button_event()`:

```cpp
case BTN_LONG_PRESS:
    if (current_state == STATE_MENU) {
        if (menu_selection == 0) {
            transition_to(STATE_PROPOSE_WAITING);
        } else if (menu_selection == 1) {
            transition_to(STATE_OSCILLOSCOPE);
        } else if (menu_selection == 2) {
            transition_to(STATE_YOUR_NEW_SCREEN);  // Add here
        }
    }
    break;
```

Add exit behavior:

```cpp
case BTN_SHORT_PRESS:
    if (current_state == STATE_MENU) {
        cycle_menu_selection();
    } else if (current_state == STATE_YOUR_NEW_SCREEN) {
        transition_to(STATE_MENU);  // Add here
    }
    // ... rest of cases
```

### 4. Add Drawing Function (screen_renderer.cpp)

```cpp
void ScreenRenderer::draw_your_new_screen() {
    begin_frame(g_state.should_invert_display());
    
    u8g2->setFont(u8g2_font_t0_12_tr);
    u8g2->drawStr(20, 15, "Your Screen!");
    
    end_frame();
}
```

### 5. Add to Renderer Switch (screen_renderer.cpp)

In `ScreenRenderer::render()`:

```cpp
switch (g_state.get_state()) {
    case STATE_SPLASH:
        draw_splash();
        break;
    // ... other cases
    case STATE_YOUR_NEW_SCREEN:
        draw_your_new_screen();  // Add here
        break;
}
```

### 6. Add Menu Display (screen_renderer.cpp)

In `ScreenRenderer::draw_menu()`:

```cpp
if (selection == 0) {
    // Propose
} else if (selection == 1) {
    // Oscilloscope
} else if (selection == 2) {
    // Your new screen
    u8g2->drawXBM(10, 10, 16, 16, YourIcon);
    u8g2->setFont(u8g2_font_t0_12_tr);
    u8g2->drawStr(35, 19, MENU_ITEM_YOUR_SCREEN);
}
```

### 7. Declare Function (screen_renderer.h)

```cpp
private:
    // ... other functions
    void draw_your_new_screen();
```

**Done!** Your new screen is fully integrated.

## Adding New Graphics

### 1. Generate Bitmap

Use an online tool like [image2cpp](https://javl.github.io/image2cpp/) to convert images to XBM format.

### 2. Add to bitmaps.cpp

```cpp
// Your icon (16x16)
const unsigned char YourIcon[] = {
    0x00,0x00,0x7e,0x7e,0x42,0x42,0x42,0x42,
    0x42,0x42,0x42,0x42,0x7e,0x7e,0x00,0x00
};
```

### 3. Declare in bitmaps.h

```cpp
extern const unsigned char YourIcon[];
```

### 4. Use in Drawing Functions

```cpp
u8g2->drawXBM(x, y, width, height, YourIcon);
```

## Adding New Button Events

### 1. Add Event Type (state_manager.h)

```cpp
enum ButtonEvent {
    BTN_NONE,
    BTN_SHORT_PRESS,
    BTN_LONG_PRESS,
    BTN_LONG_PRESS_ACTIVE,
    BTN_DOUBLE_PRESS  // Add here
};
```

### 2. Generate Event (button_handler.cpp)

```cpp
ButtonEvent ButtonHandler::update() {
    // ... existing logic
    
    // Detect double press
    if (/* your condition */) {
        return BTN_DOUBLE_PRESS;
    }
    
    return event;
}
```

### 3. Handle Event (state_manager.cpp)

```cpp
void StateManager::handle_button_event(ButtonEvent event) {
    switch (event) {
        // ... existing cases
        
        case BTN_DOUBLE_PRESS:
            // Your logic here
            break;
    }
}
```

## Common Customizations

### Make Heart Flash Faster

```cpp
#define HEART_FLASH_INTERVAL_MS 250  // Default: 500
```

### Longer Splash Screen

```cpp
#define SPLASH_DURATION_MS 5000  // Default: 2000
```

### Require Longer Press

```cpp
#define LONG_PRESS_THRESHOLD_MS 1000  // Default: 500
```

### Change Display Orientation

In `main.cpp`:

```cpp
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R2);  // Rotate 180°
```

Options: `U8G2_R0` (0°), `U8G2_R1` (90°), `U8G2_R2` (180°), `U8G2_R3` (270°)

## Testing Tips

### Test State Transitions

Add Serial debugging in `state_manager.cpp`:

```cpp
void StateManager::transition_to(AppState new_state) {
    Serial.print("Transition: ");
    Serial.print(current_state);
    Serial.print(" -> ");
    Serial.println(new_state);
    
    current_state = new_state;
    state_enter_time = millis();
    invert_display = false;
}
```

### Test Button Events

Add debugging in `main.cpp`:

```cpp
void loop() {
    g_state.update();
    
    ButtonEvent event = g_button.update();
    if (event != BTN_NONE) {
        Serial.print("Button event: ");
        Serial.println(event);
    }
    
    g_state.handle_button_event(event);
    g_renderer.render();
    delay(MAIN_LOOP_DELAY_MS);
}
```

## Architecture Overview

```
┌─────────────┐
│   main.cpp  │  Coordinates everything
└──────┬──────┘
       │
   ┌───┴────────────────────────┐
   │                            │
   ▼                            ▼
┌──────────────┐         ┌──────────────┐
│ ButtonHandler│         │ StateManager │
│ (Input)      │────────▶│ (Logic)      │
└──────────────┘         └──────┬───────┘
                                │
                                ▼
                         ┌──────────────┐
                         │ScreenRenderer│
                         │ (Output)     │
                         └──────────────┘
```

**Data Flow**: Button → Event → State Change → Render

This architecture keeps everything decoupled and easy to modify!
