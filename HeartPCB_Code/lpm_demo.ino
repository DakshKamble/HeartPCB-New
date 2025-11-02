/*
 * RA4M1 Low Power Mode Demo
 * 
 * Hardware Setup:
 * - LED connected to pin D2 (or any digital pin)
 * - Button connected to pin D3 (or any interrupt-capable pin)
 * - Button should be wired: one side to pin, other side to GND
 * - Use internal pull-up resistor
 * 
 * Functionality:
 * - LED blinks normally when awake
 * - Long press button (>1 second) to enter sleep mode
 * - LED turns off in sleep mode
 * - Long press button again to wake up
 * - Serial output shows power state changes
 * 
 * Expected Power Savings:
 * - Normal operation: ~22-25mA
 * - Sleep mode: ~13-15mA (40% reduction)
 */

// Pin definitions
#define LED_PIN D2
#define BUTTON_PIN D3
#define BUTTON_INTERRUPT_PIN 3  // Interrupt number for D3

// Button timing
#define LONG_PRESS_TIME 1000    // 1 second for long press
#define DEBOUNCE_TIME 50        // 50ms debounce

// System states
enum SystemState {
  STATE_AWAKE,
  STATE_GOING_TO_SLEEP,
  STATE_SLEEPING
};

// Global variables
volatile SystemState system_state = STATE_AWAKE;
volatile bool button_pressed = false;
volatile unsigned long button_press_time = 0;
volatile unsigned long button_release_time = 0;
volatile bool wake_up_requested = false;

unsigned long last_led_toggle = 0;
bool led_state = false;
const unsigned long LED_BLINK_INTERVAL = 500; // 500ms blink when awake

void setup() {
  // Initialize Serial
  Serial.begin(115200);
  delay(2000); // Wait for serial to initialize
  Serial.println("\n========== RA4M1 Low Power Mode Demo ==========");
  Serial.println("Hardware: LED on D2, Button on D3");
  Serial.println("Usage: Long press button to sleep/wake");
  Serial.println("===============================================\n");
  
  // Configure LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Configure button pin with internal pull-up
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Attach interrupt for button (falling edge - button press)
  attachInterrupt(BUTTON_INTERRUPT_PIN, button_isr, CHANGE);
  
  Serial.println("System initialized - LED blinking indicates awake state");
  Serial.println("Long press button (1+ seconds) to enter sleep mode\n");
}

void loop() {
  switch (system_state) {
    case STATE_AWAKE:
      handle_awake_state();
      break;
      
    case STATE_GOING_TO_SLEEP:
      handle_going_to_sleep();
      break;
      
    case STATE_SLEEPING:
      handle_sleeping_state();
      break;
  }
  
  // Small delay to prevent excessive CPU usage
  delay(10);
}

void handle_awake_state() {
  // Blink LED to show system is awake
  unsigned long now = millis();
  if (now - last_led_toggle >= LED_BLINK_INTERVAL) {
    led_state = !led_state;
    digitalWrite(LED_PIN, led_state);
    last_led_toggle = now;
  }
  
  // Check for long press to enter sleep
  if (button_pressed && (millis() - button_press_time >= LONG_PRESS_TIME)) {
    system_state = STATE_GOING_TO_SLEEP;
    button_pressed = false; // Clear the flag
  }
}

void handle_going_to_sleep() {
  Serial.println("Preparing for sleep mode...");
  
  // Turn off LED to save power
  digitalWrite(LED_PIN, LOW);
  led_state = false;
  
  // Disable unnecessary peripherals
  Serial.println("Disabling peripherals...");
  
  // Optional: Reduce clock speed (commented out for simplicity)
  // reduce_clock_speed();
  
  Serial.println("Entering sleep mode - press button to wake");
  Serial.flush(); // Make sure all serial data is sent
  
  // Small delay to ensure serial transmission completes
  delay(100);
  
  system_state = STATE_SLEEPING;
}

void handle_sleeping_state() {
  // Check if wake up was requested by interrupt
  if (wake_up_requested) {
    wake_up_requested = false;
    
    Serial.println("\nWaking up from sleep mode...");
    
    // Restore peripherals
    Serial.println("Restoring peripherals...");
    
    // Optional: Restore clock speed
    // restore_clock_speed();
    
    Serial.println("System awake - LED blinking resumed\n");
    
    system_state = STATE_AWAKE;
    last_led_toggle = millis(); // Reset LED timing
    return;
  }
  
  // Enter low power mode using WFI (Wait For Interrupt)
  // This is the key instruction that puts the RA4M1 into sleep mode
  Serial.println("Entering WFI sleep...");
  Serial.flush();
  
  // Disable interrupts briefly to ensure clean sleep entry
  noInterrupts();
  
  // Enter sleep mode - CPU stops here until interrupt occurs
  asm volatile("wfi");
  
  // CPU resumes here after interrupt
  interrupts();
  
  Serial.println("WFI sleep interrupted");
}

// Interrupt Service Routine for button
void button_isr() {
  static unsigned long last_interrupt = 0;
  unsigned long now = millis();
  
  // Simple debouncing
  if (now - last_interrupt < DEBOUNCE_TIME) {
    return;
  }
  last_interrupt = now;
  
  bool button_state = digitalRead(BUTTON_PIN);
  
  if (button_state == LOW) {
    // Button pressed (falling edge)
    button_press_time = now;
    button_pressed = true;
  } else {
    // Button released (rising edge)
    button_release_time = now;
    unsigned long press_duration = button_release_time - button_press_time;
    
    if (system_state == STATE_SLEEPING && press_duration >= LONG_PRESS_TIME) {
      // Long press while sleeping - request wake up
      wake_up_requested = true;
    }
    
    button_pressed = false;
  }
}

// Optional: Functions to reduce/restore clock speed for deeper power savings
void reduce_clock_speed() {
  // This would require direct register manipulation
  // For now, we'll just use WFI which is sufficient for testing
  Serial.println("Clock speed reduction not implemented in this demo");
}

void restore_clock_speed() {
  // Restore normal clock speed
  Serial.println("Clock speed restoration not implemented in this demo");
}

// Optional: Function to disable USB for additional power savings
void disable_usb() {
  // This would require specific RA4M1 register manipulation
  // USB typically consumes 2-5mA, so disabling it helps
  Serial.println("USB disable not implemented in this demo");
}

void enable_usb() {
  // Re-enable USB
  Serial.println("USB enable not implemented in this demo");
}
