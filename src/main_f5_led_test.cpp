#include <Arduino.h>

// ===== LED Pin Configuration =====
// Using PWM-capable pins for brightness control
#define LED1_PIN 3
#define LED2_PIN 5
#define LED3_PIN 6
#define LED4_PIN 9

// ===== Settings =====
#define BRIGHTNESS 255      // 0-255 (255 = full brightness)
#define BLINK_DELAY 500    // milliseconds

void setup() {
    // Initialize serial for debugging
    Serial.begin(115200);
    Serial.println("\n=== Iron Man Glove - F5 LED Test ===");
    Serial.println("Testing 4 blue F5 LEDs\n");
    
    // Set LED pins as outputs
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    pinMode(LED3_PIN, OUTPUT);
    pinMode(LED4_PIN, OUTPUT);
    
    // Start with all LEDs off
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
    digitalWrite(LED4_PIN, LOW);
    
    Serial.println("LED pins initialized!");
    Serial.println("Starting test sequence...\n");
    
    delay(1000);
}

void loop() {
    // Test 1: All on
    Serial.println("Test 1: All LEDs ON");
    analogWrite(LED1_PIN, BRIGHTNESS);
    analogWrite(LED2_PIN, BRIGHTNESS);
    analogWrite(LED3_PIN, BRIGHTNESS);
    analogWrite(LED4_PIN, BRIGHTNESS);
    delay(2000);
    
    // Test 2: All off
    Serial.println("Test 2: All LEDs OFF");
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
    digitalWrite(LED4_PIN, LOW);
    delay(2000);
    
    // Test 3: One at a time
    Serial.println("Test 3: One at a time");
    digitalWrite(LED1_PIN, HIGH);
    delay(BLINK_DELAY);
    digitalWrite(LED1_PIN, LOW);
    
    digitalWrite(LED2_PIN, HIGH);
    delay(BLINK_DELAY);
    digitalWrite(LED2_PIN, LOW);
    
    digitalWrite(LED3_PIN, HIGH);
    delay(BLINK_DELAY);
    digitalWrite(LED3_PIN, LOW);
    
    digitalWrite(LED4_PIN, HIGH);
    delay(BLINK_DELAY);
    digitalWrite(LED4_PIN, LOW);
    delay(1000);
    
    // Test 4: Power-up sequence
    Serial.println("Test 4: Power-up sequence");
    analogWrite(LED1_PIN, BRIGHTNESS);
    delay(150);
    analogWrite(LED2_PIN, BRIGHTNESS);
    delay(150);
    analogWrite(LED3_PIN, BRIGHTNESS);
    delay(150);
    analogWrite(LED4_PIN, BRIGHTNESS);
    delay(2000);
    
    // Turn all off
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
    digitalWrite(LED4_PIN, LOW);
    delay(1000);
    
    // Test 5: Breathing effect (fade in/out)
    Serial.println("Test 5: Breathing effect");
    for (int cycle = 0; cycle < 3; cycle++) {
        // Fade in
        for (int brightness = 0; brightness <= BRIGHTNESS; brightness += 5) {
            analogWrite(LED1_PIN, brightness);
            analogWrite(LED2_PIN, brightness);
            analogWrite(LED3_PIN, brightness);
            analogWrite(LED4_PIN, brightness);
            delay(20);
        }
        
        // Fade out
        for (int brightness = BRIGHTNESS; brightness >= 0; brightness -= 5) {
            analogWrite(LED1_PIN, brightness);
            analogWrite(LED2_PIN, brightness);
            analogWrite(LED3_PIN, brightness);
            analogWrite(LED4_PIN, brightness);
            delay(20);
        }
    }
    delay(1000);
    
    // Test 6: Alternating
    Serial.println("Test 6: Alternating");
    for (int i = 0; i < 5; i++) {
        digitalWrite(LED1_PIN, HIGH);
        digitalWrite(LED3_PIN, HIGH);
        digitalWrite(LED2_PIN, LOW);
        digitalWrite(LED4_PIN, LOW);
        delay(300);
        
        digitalWrite(LED1_PIN, LOW);
        digitalWrite(LED3_PIN, LOW);
        digitalWrite(LED2_PIN, HIGH);
        digitalWrite(LED4_PIN, HIGH);
        delay(300);
    }
    
    // All off
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
    digitalWrite(LED4_PIN, LOW);
    delay(2000);
    
    Serial.println("\n--- Test sequence complete! Restarting... ---\n");
    delay(1000);
}

