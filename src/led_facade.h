#ifndef LED_FACADE_H
#define LED_FACADE_H

#include <Arduino.h>

// Abstract interface for LED control
// Allows swapping between FastLED (WS2812B) and regular LEDs (F5)
class ILEDFacade {
public:
    virtual ~ILEDFacade() {}
    
    // Initialize the LED hardware
    virtual void begin() = 0;
    
    // Set a single LED color (r, g, b: 0-255)
    virtual void setLED(uint8_t index, uint8_t r, uint8_t g, uint8_t b) = 0;
    
    // Set all LEDs to the same color
    virtual void setAll(uint8_t r, uint8_t g, uint8_t b) = 0;
    
    // Clear all LEDs (turn off)
    virtual void clear() = 0;
    
    // Update/show the LEDs (apply changes)
    virtual void show() = 0;
    
    // Set global brightness (0-255)
    virtual void setBrightness(uint8_t brightness) = 0;
    
    // Get number of LEDs
    virtual uint8_t getNumLEDs() = 0;
};

#endif // LED_FACADE_H
