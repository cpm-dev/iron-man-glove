#include <Arduino.h>
#include "config.h"
#include "motion_detector.h"
#include "led_controller.h"

// Global objects
MotionDetector motionDetector;
LEDController ledController;

// System state
bool systemReady = false;

void setup() {
    // Initialize serial for debugging
    #if DEBUG
    Serial.begin(115200);
    while (!Serial) {
        delay(10); // Wait for serial port to connect
    }
    DEBUG_PRINTLN("\n=== Iron Man Glove Starting ===");
    #endif
    
    // Initialize LED controller
    DEBUG_PRINTLN("Initializing LED Controller...");
    ledController.begin();
    
    // Quick startup animation (all LEDs flash once)
    for (int i = 0; i < NUM_LEDS; i++) {
        // This is done directly with FastLED since ledController is for runtime
    }
    
    // Initialize motion detector
    DEBUG_PRINTLN("Initializing Motion Detector...");
    if (!motionDetector.begin()) {
        DEBUG_PRINTLN("ERROR: Failed to initialize MPU6050!");
        DEBUG_PRINTLN("Check wiring:");
        DEBUG_PRINTLN("  SDA -> A4");
        DEBUG_PRINTLN("  SCL -> A5");
        DEBUG_PRINTLN("  VCC -> 3.3V");
        DEBUG_PRINTLN("  GND -> GND");
        
        // Flash LEDs to indicate error
        while (true) {
            ledController.activate();
            delay(200);
            ledController.turnOff();
            delay(200);
        }
    }
    
    systemReady = true;
    DEBUG_PRINTLN("=== System Ready ===\n");
}

void loop() {
    if (!systemReady) {
        return;
    }
    
    // Check for hand raise motion
    if (motionDetector.isHandRaised()) {
        DEBUG_PRINTLN("*** ACTIVATING IRON MAN MODE ***");
        ledController.activate();
    }
    
    // Update LED animations
    ledController.update();
    
    // Small delay to prevent overwhelming the I2C bus
    delay(20); // ~50Hz update rate
}
