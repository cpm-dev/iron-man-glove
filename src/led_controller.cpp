#include "led_controller.h"
#include "config.h"

LEDController::LEDController() 
    : currentMode(OFF), 
      animationStartTime(0), 
      activationTime(0),
      currentBrightness(BRIGHTNESS) {
}

void LEDController::begin() {
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
    FastLED.show();
    
    DEBUG_PRINTLN("LED Controller initialized");
}

void LEDController::setBrightness(uint8_t brightness) {
    currentBrightness = brightness;
    FastLED.setBrightness(brightness);
}

void LEDController::activate() {
    currentMode = POWER_UP;
    animationStartTime = millis();
    activationTime = millis();
    DEBUG_PRINTLN("LED Activation started");
}

bool LEDController::isActive() {
    return currentMode != OFF;
}

void LEDController::turnOff() {
    currentMode = OFF;
    FastLED.clear();
    FastLED.show();
}

CRGB LEDController::getIronManColor(uint8_t index) {
    // Alternate between red and gold for Iron Man effect
    if (index % 2 == 0) {
        return CRGB(COLOR_RED);
    } else {
        return CRGB(COLOR_GOLD);
    }
}

void LEDController::animatePowerUp() {
    unsigned long elapsed = millis() - animationStartTime;
    uint8_t numLit = map(elapsed, 0, 500, 0, NUM_LEDS);
    
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        if (i < numLit) {
            leds[i] = getIronManColor(i);
        } else {
            leds[i] = CRGB::Black;
        }
    }
    
    FastLED.show();
    
    // Transition to steady after power-up complete
    if (elapsed > 500) {
        currentMode = STEADY;
        animationStartTime = millis();
        DEBUG_PRINTLN("Power-up complete, entering steady mode");
    }
}

void LEDController::animateSteady() {
    // Breathing effect
    unsigned long elapsed = millis() - animationStartTime;
    float breathe = (sin(elapsed / 500.0) + 1.0) / 2.0; // 0.0 to 1.0
    uint8_t brightness = 150 + (breathe * 105); // 150-255 range
    
    FastLED.setBrightness(brightness);
    
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        leds[i] = getIronManColor(i);
    }
    
    FastLED.show();
    
    // Check if we should start fading out
    if (millis() - activationTime > ACTIVE_DURATION) {
        currentMode = FADE_OUT;
        animationStartTime = millis();
        DEBUG_PRINTLN("Starting fade out");
    }
}

void LEDController::animateFadeOut() {
    unsigned long elapsed = millis() - animationStartTime;
    uint8_t brightness = map(elapsed, 0, 1000, currentBrightness, 0);
    
    if (brightness == 0 || elapsed > 1000) {
        turnOff();
        DEBUG_PRINTLN("Fade out complete");
        return;
    }
    
    FastLED.setBrightness(brightness);
    
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        leds[i] = getIronManColor(i);
    }
    
    FastLED.show();
}

void LEDController::update() {
    switch (currentMode) {
        case POWER_UP:
            animatePowerUp();
            break;
            
        case STEADY:
            animateSteady();
            break;
            
        case FADE_OUT:
            animateFadeOut();
            break;
            
        case OFF:
        default:
            // Do nothing
            break;
    }
}
