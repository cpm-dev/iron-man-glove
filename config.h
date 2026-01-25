#ifndef CONFIG_H
#define CONFIG_H

// ===== Pin Configuration =====
#define LED_PIN 6              // PWM pin for LEDs (WS2812B or similar)
#define NUM_LEDS 4            // Number of LEDs in your strip/ring

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

// ===== LED Color Scheme (Iron Man Red/Gold) =====
#define COLOR_RED 255, 0, 0
#define COLOR_GOLD 255, 180, 0
#define COLOR_OFF 0, 0, 0

#endif // CONFIG_H
