#ifndef CONFIG_H
#define CONFIG_H

// ===== LED Hardware Type Selection =====
// Uncomment ONE of these to select your LED hardware
// Or define via build flags in platformio.ini

#ifndef LED_TYPE_FASTLED
#ifndef LED_TYPE_F5
    #define LED_TYPE_F5     // Default to F5 LEDs if not specified
#endif
#endif

// ===== LED Configuration =====
#define NUM_LEDS 4             // Number of LEDs in your setup

// FastLED configuration (WS2812B addressable LEDs)
#ifdef LED_TYPE_FASTLED
    #define LED_PIN 6          // Data pin for WS2812B strip
#endif

// F5 LED configuration (regular 5mm LEDs)
#ifdef LED_TYPE_F5
    // Pin assignments for each LED (must be PWM-capable: 3, 5, 6, 9, 10, 11)
    #define LED_PIN_1 3
    #define LED_PIN_2 5
    #define LED_PIN_3 6
    #define LED_PIN_4 9
    #if NUM_LEDS >= 5
        #define LED_PIN_5 10
    #endif
    #if NUM_LEDS >= 6
        #define LED_PIN_6 11
    #endif
    
    // LED color (for single-color LEDs)
    #define LED_COLOR_BLUE     // Options: LED_COLOR_BLUE, LED_COLOR_RED, LED_COLOR_GREEN
#endif

// ===== Motion Detection Settings =====
#define MOTION_THRESHOLD 1.2   // G-force threshold (1.2 = ~20-30° tilt)
#define ACTIVATION_ANGLE 45    // Degrees from horizontal to activate
#define DEBOUNCE_TIME 100      // milliseconds
#define ACTIVE_DURATION 3000   // How long LEDs stay on (ms)

// ===== LED Animation Settings =====
#define BRIGHTNESS 200         // 0-255 (reduce for longer battery life)
#define FADE_SPEED 10          // Speed of fade in/out
#define ANIMATION_FPS 60       // Frames per second for animations

// ===== Power Management =====
#define LOW_POWER_MODE true    // Enable sleep between readings
#define SAMPLE_RATE 50         // Accelerometer sample rate (Hz)

// ===== Debug Settings =====
#if DEBUG
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif

// ===== LED Color Scheme =====
// Iron Man Red/Gold theme
#define COLOR_RED 255, 0, 0
#define COLOR_GOLD 255, 180, 0
#define COLOR_BLUE 0, 0, 255
#define COLOR_OFF 0, 0, 0

// For F5 LEDs (single color), these map to brightness levels
#ifdef LED_TYPE_F5
    #ifdef LED_COLOR_BLUE
        #define COLOR_PRIMARY COLOR_BLUE
        #define COLOR_SECONDARY COLOR_BLUE  // Same color, just on/off
    #elif defined(LED_COLOR_RED)
        #define COLOR_PRIMARY COLOR_RED
        #define COLOR_SECONDARY COLOR_RED
    #elif defined(LED_COLOR_GREEN)
        #define COLOR_PRIMARY COLOR_GREEN
        #define COLOR_SECONDARY COLOR_GREEN
    #endif
#else
    // For FastLED (RGB), use full color scheme
    #define COLOR_PRIMARY COLOR_RED
    #define COLOR_SECONDARY COLOR_GOLD
#endif

#endif // CONFIG_H