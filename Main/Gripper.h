#include <Servo.h>
#include <Arduino.h>
#pragma once

struct CupPos // not currently used
{   
    int direction; //direction 0 to 360 degrees. 0 degrees is straight ahead
    int distance; // [mm]
};

class Gripper
{
private:
    int servoLiftPin;
    int servoGripPin;
    int trigPin;
    int echoPin;
    int gripClosed = 166; // Angle configuration in closed posistion
    int gripOpened = 100; // Angle configuration in open posistion
    int liftLower = 160; // Angle configuration in lower posistion
    int liftUpper = 110; // Angle configuration in upper posisiton
    Servo servoLift;
    Servo servoGrip;
public:
    Gripper(int servoLiftPin, int servoGripPin, int trigpPin, int echoPin);
    void grab(); // grabs object and lifts it up
    void letGo(); // places object down and loosens gripper
    void initServos();
    void initUltrasonic(); // setup     
    int readDistance(); // returns distance measured by Ultrasonic sensor   
    bool checkCup(int a, int b); // Checks if Ultrasonic reading is between a [mm] and b [mm]
};