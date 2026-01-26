#ifndef MOTION_DETECTOR_H
#define MOTION_DETECTOR_H

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

class MotionDetector {
public:
    MotionDetector();
    
    // Initialize the MPU6050 sensor
    bool begin();
    
    // Check if hand is raised (returns true when hand motion detected)
    bool isHandRaised();
    
    // Get current acceleration values
    void getAcceleration(float &x, float &y, float &z);
    
    // Check sensor status
    bool isConnected();
    
private:
    Adafruit_MPU6050 mpu;
    
    unsigned long lastTriggerTime;
    bool wasRaised;
    
    // Calculate pitch angle from accelerometer data
    float calculatePitch(float x, float y, float z);
    
    // Apply debouncing logic
    bool debounce();
};

#endif // MOTION_DETECTOR_H