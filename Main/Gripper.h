#include <Servo.h>
#include <Arduino.h>
#pragma once

class Gripper
{
private:
    int servoLiftPin;
    int servoGripPin;
    int gripClosed = 165; // Angle configuration in closed posistion
    int gripOpened = 100; // Angle configuration in open posistion
    int liftLower = 150; // Angle configuration in lower posistion
    int liftUpper = 110; // Angle configuration in upper posisiton
    Servo servoLift;
    Servo servoGrip;
public:
    Gripper(int servoLiftPin, int servoGripPin);
    void grab(); // grabs object and lifts it up
    void letGo(); // places object down and loosens gripper
    void initServos();
};