#include <Arduino.h>
#include <FastLED.h>

// LED Configuration
#define LED_PIN 6
#define NUM_LEDS 10
#define BRIGHTNESS 200

// LED array
CRGB leds[NUM_LEDS];

// Colors (Iron Man theme)
#define COLOR_RED CRGB(255, 0, 0)
#define COLOR_GOLD CRGB(255, 180, 0)

void setup() {
    // Initialize serial for debugging
    Serial.begin(115200);
    Serial.println("\n=== Iron Man Glove - LED Test ===");
    
    // Initialize LED strip
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
    FastLED.show();
    
    Serial.println("LED Controller initialized!");
    Serial.println("Starting test sequence...\n");
    
    delay(1000);
}

void loop() {
    // Test 1: All Red
    Serial.println("Test 1: All LEDs RED");
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = COLOR_RED;
    }
    FastLED.show();
    delay(2000);
    
    // Test 2: All Gold
    Serial.println("Test 2: All LEDs GOLD");
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = COLOR_GOLD;
    }
    FastLED.show();
    delay(2000);
    
    // Test 3: Alternating Red/Gold
    Serial.println("Test 3: Alternating RED/GOLD");
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = (i % 2 == 0) ? COLOR_RED : COLOR_GOLD;
    }
    FastLED.show();
    delay(2000);
    
    // Test 4: One by one power-up
    Serial.println("Test 4: Power-up sequence");
    FastLED.clear();
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = (i % 2 == 0) ? COLOR_RED : COLOR_GOLD;
        FastLED.show();
        delay(100);
    }
    delay(1000);
    
    // Test 5: Breathing effect
    Serial.println("Test 5: Breathing effect");
    for (int breath = 0; breath < 3; breath++) {
        // Fill all LEDs
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = (i % 2 == 0) ? COLOR_RED : COLOR_GOLD;
        }
        
        // Fade in
        for (int brightness = 50; brightness <= 255; brightness += 5) {
            FastLED.setBrightness(brightness);
            FastLED.show();
            delay(20);
        }
        
        // Fade out
        for (int brightness = 255; brightness >= 50; brightness -= 5) {
            FastLED.setBrightness(brightness);
            FastLED.show();
            delay(20);
        }
    }
    
    // Reset brightness
    FastLED.setBrightness(BRIGHTNESS);
    
    // Test 6: Rainbow
    Serial.println("Test 6: Rainbow cycle");
    for (int hue = 0; hue < 255; hue++) {
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CHSV(hue + (i * 255 / NUM_LEDS), 255, 255);
        }
        FastLED.show();
        delay(10);
    }
    delay(1000);
    
    // Test 7: Off
    Serial.println("Test 7: All OFF");
    FastLED.clear();
    FastLED.show();
    delay(2000);
    
    Serial.println("\n--- Test sequence complete! Restarting... ---\n");
    delay(1000);
}

