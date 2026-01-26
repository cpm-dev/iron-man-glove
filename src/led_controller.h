#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Arduino.h>
#include <FastLED.h>

enum AnimationMode {
    OFF,
    POWER_UP,
    STEADY,
    FADE_OUT
};

class LEDController {
public:
    LEDController();
    
    // Initialize LED strip
    void begin();
    
    // Trigger the Iron Man activation sequence
    void activate();
    
    // Update LED animation (call in loop)
    void update();
    
    // Turn off all LEDs
    void turnOff();
    
    // Check if animation is currently active
    bool isActive();
    
    // Set brightness (0-255)
    void setBrightness(uint8_t brightness);
    
private:
    CRGB leds[NUM_LEDS];
    
    AnimationMode currentMode;
    unsigned long animationStartTime;
    unsigned long activationTime;
    uint8_t currentBrightness;
    
    // Animation functions
    void animatePowerUp();
    void animateSteady();
    void animateFadeOut();
    
    // Helper to create Iron Man colors
    CRGB getIronManColor(uint8_t index);
};

#endif // LED_CONTROLLER_H