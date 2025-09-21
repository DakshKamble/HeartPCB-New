# HeartPCB Code - Modular Architecture

## Overview
This is an embedded Arduino project for a heart-shaped PCB with an OLED display and button input.

## Project Structure

```
src/
├── main.cpp              # Main program entry point
└── core/                 # Core modules
    ├── display.h/.cpp    # Display management
    ├── button_handler.h/.cpp # Button input handling
    ├── menu.h/.cpp       # Menu system
    └── bitmaps.h/.cpp    # Icon/bitmap storage
```

## Module Responsibilities

### main.cpp
- Initialize all modules
- Main program loop
- Coordinates between modules

### Display Module (`display.h/.cpp`)
- Display initialization and management
- Screen drawing functions
- Low-level display operations

### Button Handler (`button_handler.h/.cpp`)
- Button input processing
- Debouncing logic
- Button state management

### Menu System (`menu.h/.cpp`)
- Menu data storage
- Menu navigation logic
- Screen state management

### Bitmaps (`bitmaps.h/.cpp`)
- Icon and bitmap data storage
- Graphical assets

## How to Extend

1. **Add new screens**: Modify `menu.cpp` to add new menu items
2. **Add new display functions**: Add to `display.cpp` 
3. **Add new input methods**: Extend `button_handler.cpp`
4. **Add new graphics**: Add bitmaps to `bitmaps.cpp`

## Key Features

- **Modular Design**: Each component has clear responsibilities
- **Clean Interfaces**: Functions hide implementation details
- **Easy to Understand**: Simple, well-commented code
- **Expandable**: Easy to add new features without breaking existing code
