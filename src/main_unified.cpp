#include <Arduino.h>
#include "config.h"
#include "led_facade.h"

// Include the appropriate LED implementation based on build configuration
#ifdef LED_TYPE_FASTLED
    #include "fastled_facade.h"
    FastLEDFacade ledFacade;
#else
    #include "f5led_facade.h"
    F5LEDFacade ledFacade;
#endif

// LED test mode vs full mode with motion sensor
#ifdef TEST_MODE
    // Test mode - no motion sensor
    #define USE_MOTION_SENSOR false
#else
    // Full mode - include motion sensor
    #define USE_MOTION_SENSOR true
    #include "motion_detector.h"
    MotionDetector motionDetector;
#endif

// System state
bool systemReady = false;
unsigned long lastActivation = 0;
bool isActive = false;

// Animation state
enum AnimationState {
    ANIM_OFF,
    ANIM_POWER_UP,
    ANIM_STEADY,
    ANIM_FADE_OUT
};

AnimationState currentAnimation = ANIM_OFF;
unsigned long animationStartTime = 0;

// Helper function to get color for LED index
void getColorForIndex(uint8_t index, uint8_t &r, uint8_t &g, uint8_t &b) {
    #ifdef LED_TYPE_FASTLED
        // Alternate red and gold for Iron Man theme
        if (index % 2 == 0) {
            r = 255; g = 0; b = 0;      // Red
        } else {
            r = 255; g = 180; b = 0;    // Gold
        }
    #else
        // For F5 single-color LEDs, just use blue
        r = 0; g = 0; b = 255;
    #endif
}

void startAnimation() {
    currentAnimation = ANIM_POWER_UP;
    animationStartTime = millis();
    lastActivation = millis();
    isActive = true;
    DEBUG_PRINTLN("*** ANIMATION STARTED ***");
}

void updateAnimation() {
    unsigned long elapsed = millis() - animationStartTime;
    unsigned long totalElapsed = millis() - lastActivation;
    
    switch (currentAnimation) {
        case ANIM_OFF:
            // Do nothing
            break;
            
        case ANIM_POWER_UP: {
            // Power up sequence - light LEDs one by one
            uint8_t numLit = map(elapsed, 0, 500, 0, ledFacade.getNumLEDs());
            
            ledFacade.clear();
            for (uint8_t i = 0; i < numLit && i < ledFacade.getNumLEDs(); i++) {
                uint8_t r, g, b;
                getColorForIndex(i, r, g, b);
                ledFacade.setLED(i, r, g, b);
            }
            ledFacade.show();
            
            if (elapsed > 500) {
                currentAnimation = ANIM_STEADY;
                animationStartTime = millis();
                DEBUG_PRINTLN("Power-up complete → Steady");
            }
            break;
        }
        
        case ANIM_STEADY: {
            // Breathing effect
            float breathe = (sin(elapsed / 500.0) + 1.0) / 2.0; // 0.0 to 1.0
            uint8_t brightness = 150 + (breathe * 105); // 150-255 range
            
            ledFacade.setBrightness(brightness);
            
            for (uint8_t i = 0; i < ledFacade.getNumLEDs(); i++) {
                uint8_t r, g, b;
                getColorForIndex(i, r, g, b);
                ledFacade.setLED(i, r, g, b);
            }
            ledFacade.show();
            
            // Check if should start fading out
            if (totalElapsed > ACTIVE_DURATION) {
                currentAnimation = ANIM_FADE_OUT;
                animationStartTime = millis();
                DEBUG_PRINTLN("Steady → Fade out");
            }
            break;
        }
        
        case ANIM_FADE_OUT: {
            // Fade out over 1 second
            uint8_t brightness = map(elapsed, 0, 1000, BRIGHTNESS, 0);
            
            if (elapsed > 1000 || brightness == 0) {
                ledFacade.clear();
                ledFacade.show();
                currentAnimation = ANIM_OFF;
                isActive = false;
                ledFacade.setBrightness(BRIGHTNESS); // Reset brightness
                DEBUG_PRINTLN("Fade out complete → Off");
            } else {
                ledFacade.setBrightness(brightness);
                
                for (uint8_t i = 0; i < ledFacade.getNumLEDs(); i++) {
                    uint8_t r, g, b;
                    getColorForIndex(i, r, g, b);
                    ledFacade.setLED(i, r, g, b);
                }
                ledFacade.show();
            }
            break;
        }
    }
}

void runTestSequence() {
    // Test sequence for LED validation
    static unsigned long testStartTime = 0;
    static uint8_t testPhase = 0;
    unsigned long currentTime = millis();
    
    if (testStartTime == 0) {
        testStartTime = currentTime;
    }
    
    unsigned long elapsed = currentTime - testStartTime;
    
    switch (testPhase) {
        case 0: // All on
            if (elapsed == 0) {
                Serial.println("Test: All LEDs ON");
                for (uint8_t i = 0; i < ledFacade.getNumLEDs(); i++) {
                    uint8_t r, g, b;
                    getColorForIndex(i, r, g, b);
                    ledFacade.setLED(i, r, g, b);
                }
                ledFacade.show();
            }
            if (elapsed > 2000) {
                testPhase++;
                testStartTime = currentTime;
            }
            break;
            
        case 1: // All off
            if (elapsed == 0) {
                Serial.println("Test: All LEDs OFF");
                ledFacade.clear();
                ledFacade.show();
            }
            if (elapsed > 2000) {
                testPhase++;
                testStartTime = currentTime;
            }
            break;
            
        case 2: // One by one
            if (elapsed == 0) {
                Serial.println("Test: One by one");
            }
            {
                uint8_t lit = (elapsed / 500) % ledFacade.getNumLEDs();
                ledFacade.clear();
                uint8_t r, g, b;
                getColorForIndex(lit, r, g, b);
                ledFacade.setLED(lit, r, g, b);
                ledFacade.show();
            }
            if (elapsed > 4000) {
                testPhase++;
                testStartTime = currentTime;
            }
            break;
            
        case 3: // Breathing
            if (elapsed == 0) {
                Serial.println("Test: Breathing effect");
            }
            {
                float breathe = (sin(elapsed / 500.0) + 1.0) / 2.0;
                uint8_t brightness = 50 + (breathe * 205);
                ledFacade.setBrightness(brightness);
                
                for (uint8_t i = 0; i < ledFacade.getNumLEDs(); i++) {
                    uint8_t r, g, b;
                    getColorForIndex(i, r, g, b);
                    ledFacade.setLED(i, r, g, b);
                }
                ledFacade.show();
            }
            if (elapsed > 5000) {
                testPhase = 0; // Loop
                testStartTime = currentTime;
                ledFacade.setBrightness(BRIGHTNESS);
                Serial.println("\n--- Test sequence complete! Restarting... ---\n");
            }
            break;
    }
}

void setup() {
    // Initialize serial for debugging
    #if DEBUG
    Serial.begin(115200);
    while (!Serial && millis() < 3000) {
        delay(10); // Wait up to 3 seconds for serial
    }
    Serial.println("\n=== Iron Man Glove ===");
    #ifdef LED_TYPE_FASTLED
    Serial.println("LED Type: FastLED (WS2812B)");
    #else
    Serial.println("LED Type: F5 Regular LEDs");
    #endif
    Serial.print("Number of LEDs: ");
    Serial.println(NUM_LEDS);
    #ifdef TEST_MODE
    Serial.println("Mode: TEST");
    #else
    Serial.println("Mode: FULL (with motion sensor)");
    #endif
    Serial.println();
    #endif
    
    // Initialize LED facade
    DEBUG_PRINTLN("Initializing LED controller...");
    ledFacade.begin();
    
    #if USE_MOTION_SENSOR
    // Initialize motion detector
    DEBUG_PRINTLN("Initializing motion detector...");
    if (!motionDetector.begin()) {
        DEBUG_PRINTLN("ERROR: Failed to initialize MPU6050!");
        DEBUG_PRINTLN("Check wiring:");
        DEBUG_PRINTLN("  SDA -> A4");
        DEBUG_PRINTLN("  SCL -> A5");
        DEBUG_PRINTLN("  VCC -> 3.3V");
        DEBUG_PRINTLN("  GND -> GND");
        
        // Flash LEDs to indicate error
        while (true) {
            ledFacade.setAll(255, 0, 0);
            ledFacade.show();
            delay(200);
            ledFacade.clear();
            ledFacade.show();
            delay(200);
        }
    }
    #endif
    
    systemReady = true;
    DEBUG_PRINTLN("=== System Ready ===\n");
}

void loop() {
    if (!systemReady) {
        return;
    }
    
    #ifdef TEST_MODE
        // Run continuous test sequence
        runTestSequence();
    #else
        // Full mode with motion sensor
        #if USE_MOTION_SENSOR
        if (!isActive && motionDetector.isHandRaised()) {
            DEBUG_PRINTLN("*** HAND RAISED - ACTIVATING! ***");
            startAnimation();
        }
        #endif
        
        // Update animation if active
        if (isActive) {
            updateAnimation();
        }
    #endif
    
    delay(20); // ~50Hz update rate
}
