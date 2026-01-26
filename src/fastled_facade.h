#ifndef FASTLED_FACADE_H
#define FASTLED_FACADE_H

#include "led_facade.h"
#include <FastLED.h>
#include "config.h"

// FastLED implementation for WS2812B addressable LEDs
class FastLEDFacade : public ILEDFacade {
private:
    CRGB leds[NUM_LEDS];
    uint8_t currentBrightness;
    
public:
    FastLEDFacade() : currentBrightness(BRIGHTNESS) {}
    
    void begin() override {
        FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
        FastLED.setBrightness(BRIGHTNESS);
        FastLED.clear();
        FastLED.show();
        
        DEBUG_PRINTLN("FastLED initialized (WS2812B)");
    }
    
    void setLED(uint8_t index, uint8_t r, uint8_t g, uint8_t b) override {
        if (index < NUM_LEDS) {
            leds[index] = CRGB(r, g, b);
        }
    }
    
    void setAll(uint8_t r, uint8_t g, uint8_t b) override {
        for (uint8_t i = 0; i < NUM_LEDS; i++) {
            leds[i] = CRGB(r, g, b);
        }
    }
    
    void clear() override {
        FastLED.clear();
    }
    
    void show() override {
        FastLED.show();
    }
    
    void setBrightness(uint8_t brightness) override {
        currentBrightness = brightness;
        FastLED.setBrightness(brightness);
    }
    
    uint8_t getNumLEDs() override {
        return NUM_LEDS;
    }
};

#endif // FASTLED_FACADE_H