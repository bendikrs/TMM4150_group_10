#include <Servo.h>
#include <Arduino.h>
#pragma once

class Gripper
{
private:
    int servoLiftPin;
    int servoGripPin;
    int gripClosed = 160;
    int gripOpened = 150;
public:
    Servo servoLift;
    Servo servoGrip;
    Gripper(int servoLiftPin, int servoGripPin);
    void grab(); // grabs object and lifts it up
    void letGo(); // places object down and loosens gripper
};