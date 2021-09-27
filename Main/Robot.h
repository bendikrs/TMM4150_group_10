#include "A4988.h"
#include "InfraredArray.h"
#include "SyncDriver.h"
#pragma once

class Robot
{
private:
    int diameterDriveWheels = 65; // [mm]
    int speed = 400; // [mm/s]


public:
    A4988 stepper_left;
    A4988 stepper_right;
    InfraredArray irArray;
    SyncDriver controller;
    Robot(A4988 stepper_left, A4988 stepper_right, InfraredArray irArray, SyncDriver controller);
    int checkIfWorking();
    void setSpeed(int speed); // [mm/s]
    float speed2rpm(int speed);
    bool lookForLine(); // False until line is found
    bool followLine(); // Assumes a line is present, and executes one correction iteration to follow the line
    bool autoDrive(); // Drives until line is found, follows line when it's found. tries to grab cup, if found.
    void moveRobot(int steps1, int steps2);
    void beginRobot();
};


