#ifndef F5LED_FACADE_H
#define F5LED_FACADE_H

#include "led_facade.h"
#include "config.h"

// F5 LED implementation for regular 5mm LEDs with individual pins
class F5LEDFacade : public ILEDFacade {
private:
    // Pin assignments for each LED (must be PWM-capable)
    const uint8_t ledPins[NUM_LEDS] = {
        #if NUM_LEDS >= 1
        LED_PIN_1,
        #endif
        #if NUM_LEDS >= 2
        LED_PIN_2,
        #endif
        #if NUM_LEDS >= 3
        LED_PIN_3,
        #endif
        #if NUM_LEDS >= 4
        LED_PIN_4,
        #endif
        #if NUM_LEDS >= 5
        LED_PIN_5,
        #endif
        #if NUM_LEDS >= 6
        LED_PIN_6,
        #endif
    };
    
    // Store brightness for each LED (0-255)
    uint8_t ledBrightness[NUM_LEDS];
    uint8_t globalBrightness;
    
    // Store desired RGB values (for single-color LEDs, we use brightness only)
    uint8_t ledR[NUM_LEDS];
    uint8_t ledG[NUM_LEDS];
    uint8_t ledB[NUM_LEDS];
    
    // Calculate effective brightness from RGB (for monochrome LEDs)
    uint8_t calculateBrightness(uint8_t r, uint8_t g, uint8_t b) {
        // For blue LEDs, use the blue channel
        // For other colors, use the max of RGB as brightness
        #ifdef LED_COLOR_BLUE
            return b;
        #elif defined(LED_COLOR_RED)
            return r;
        #elif defined(LED_COLOR_GREEN)
            return g;
        #else
            // Default: use max brightness from any channel
            uint8_t max = r;
            if (g > max) max = g;
            if (b > max) max = b;
            return max;
        #endif
    }
    
    // Apply global brightness scaling
    uint8_t scaleByGlobalBrightness(uint8_t value) {
        return (uint16_t)value * globalBrightness / 255;
    }
    
public:
    F5LEDFacade() : globalBrightness(BRIGHTNESS) {
        for (uint8_t i = 0; i < NUM_LEDS; i++) {
            ledBrightness[i] = 0;
            ledR[i] = 0;
            ledG[i] = 0;
            ledB[i] = 0;
        }
    }
    
    void begin() override {
        // Initialize all LED pins as outputs
        for (uint8_t i = 0; i < NUM_LEDS; i++) {
            pinMode(ledPins[i], OUTPUT);
            digitalWrite(ledPins[i], LOW);
        }
        
        DEBUG_PRINT("F5 LED initialized on pins: ");
        for (uint8_t i = 0; i < NUM_LEDS; i++) {
            DEBUG_PRINT(ledPins[i]);
            if (i < NUM_LEDS - 1) DEBUG_PRINT(", ");
        }
        DEBUG_PRINTLN("");
    }
    
    void setLED(uint8_t index, uint8_t r, uint8_t g, uint8_t b) override {
        if (index >= NUM_LEDS) return;
        
        ledR[index] = r;
        ledG[index] = g;
        ledB[index] = b;
        ledBrightness[index] = calculateBrightness(r, g, b);
    }
    
    void setAll(uint8_t r, uint8_t g, uint8_t b) override {
        for (uint8_t i = 0; i < NUM_LEDS; i++) {
            setLED(i, r, g, b);
        }
    }
    
    void clear() override {
        for (uint8_t i = 0; i < NUM_LEDS; i++) {
            ledBrightness[i] = 0;
            ledR[i] = 0;
            ledG[i] = 0;
            ledB[i] = 0;
            digitalWrite(ledPins[i], LOW);
        }
    }
    
    void show() override {
        // Update all LEDs with current brightness values
        for (uint8_t i = 0; i < NUM_LEDS; i++) {
            uint8_t scaledBrightness = scaleByGlobalBrightness(ledBrightness[i]);
            analogWrite(ledPins[i], scaledBrightness);
        }
    }
    
    void setBrightness(uint8_t brightness) override {
        globalBrightness = brightness;
    }
    
    uint8_t getNumLEDs() override {
        return NUM_LEDS;
    }
};

#endif // F5LED_FACADE_H