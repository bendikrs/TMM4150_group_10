#include "A4988.h"
#include "InfraredArray.h"
#include "SyncDriver.h"
#include "Gripper.h"
#pragma once

class Robot
{
private:
    int stepsPerRotation = 200; // steps
    int diameterDriveWheels = 65; // [mm]
    float speed = 180; // [mm/s]
    float leftSpeed; // [mm/s] positiv verdi er framover
    float rightSpeed; // [mm/s] negativ verdi er framover
    int turnSpeedDiff = speed - 40; // [mm/s]
    int cycleTime = 50; // [millisecond]
    bool hasFoundCup = false;
    float wheelbase = 124; // [mm]
    float lastError = 0;
    int state = 0;
    int maxSpeed = 400; // [mm/s]
    /* States:
    0: No line found
    1: Detected left turn
    2: Detected right turn
    3: Detected intersection, need to choose to turn left or right
    4: Follows line
    */

public:
    A4988 stepper_left;
    A4988 stepper_right;
    InfraredArray irArray;
    SyncDriver controller;
    Gripper gripper;
    Robot(A4988 stepper_left, A4988 stepper_right, InfraredArray irArray, SyncDriver controller, Gripper gripper);
    int checkIfWorking();
    void setSpeed(float speed); // [mm/s]
    float speed2rpm(float speed);
    bool lookForLine(); // False until line is found
    void followLine(); // Assumes a line is present, and executes one correction iteration to follow the line
    void fancyFollowLine();
    bool autoDrive(); // Drives until line is found, follows line when it's found. tries to grab cup, if found.
    void moveRobot(int steps1, int steps2); // drives the robot, positive is forward, negative is backwards
    void beginRobot(); // init motors and calibrate IRs
    void setRightSpeed(float _speed); // sets the speed for the right stepper, takes in [mm/s]
    void setLeftSpeed(float _speed); // sets the speed for the left stepper, takes in [mm/s]
    void rotateRobot(float degrees); // rotates robot, positive is clockwise, negative is counterclockwise
    void moveRobotDist(float distLeft, float distRight); // moves robot a given distance in [mm]
};