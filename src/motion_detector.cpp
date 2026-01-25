#include "motion_detector.h"
#include "config.h"
#include <math.h>

MotionDetector::MotionDetector() 
    : lastTriggerTime(0), wasRaised(false) {
}

bool MotionDetector::begin() {
    // Initialize I2C
    Wire.begin();
    
    // Try to initialize MPU6050
    if (!mpu.begin()) {
        DEBUG_PRINTLN("Failed to find MPU6050 chip");
        return false;
    }
    
    DEBUG_PRINTLN("MPU6050 Found!");
    
    // Configure sensor ranges
    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    
    // Small delay for sensor stabilization
    delay(100);
    
    return true;
}

bool MotionDetector::isConnected() {
    sensors_event_t a, g, temp;
    return mpu.getEvent(&a, &g, &temp);
}

void MotionDetector::getAcceleration(float &x, float &y, float &z) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    x = a.acceleration.x;
    y = a.acceleration.y;
    z = a.acceleration.z;
}

float MotionDetector::calculatePitch(float x, float y, float z) {
    // Calculate pitch (rotation around Y axis)
    // Pitch is the angle of the hand relative to horizontal
    float pitch = atan2(-x, sqrt(y * y + z * z)) * 180.0 / PI;
    return pitch;
}

bool MotionDetector::debounce() {
    unsigned long currentTime = millis();
    
    if (currentTime - lastTriggerTime < DEBOUNCE_TIME) {
        return false; // Still in debounce period
    }
    
    return true;
}

bool MotionDetector::isHandRaised() {
    float x, y, z;
    getAcceleration(x, y, z);
    
    // Calculate the pitch angle
    float pitch = calculatePitch(x, y, z);
    
    DEBUG_PRINT("Pitch: ");
    DEBUG_PRINTLN(pitch);
    
    // Check if hand is raised above threshold angle
    bool isRaised = (pitch > ACTIVATION_ANGLE);
    
    // Detect rising edge (transition from not raised to raised)
    if (isRaised && !wasRaised && debounce()) {
        wasRaised = true;
        lastTriggerTime = millis();
        DEBUG_PRINTLN("HAND RAISED - ACTIVATING!");
        return true;
    }
    
    // Update state for next check
    if (!isRaised) {
        wasRaised = false;
    }
    
    return false;
}
