#include "A4988.h"
#include "InfraredArray.h"
#include "SyncDriver.h"
#include "Gripper.h"
#pragma once

enum State{NOLINE, LEFTTURN, RIGHTTURN, INTERSECTION, FOLLOWLINE};

struct driveLog
{
    int leftSpeed;
    int rightSpeed;
    int leftSteps;
    int rightSteps;
};

class Robot
{
private:
    // Robot constants
    int stepsPerRotation = 200; // [steps]
    int diameterDriveWheels = 65; // [mm]
    int distAxelToSensorArray = 45; // [mm] used in left and right turns
    float wheelbase = 132; // [mm]
    int maxSpeed = 400; // [mm/s]
    
    // Speeds
    float speed = 150; // [mm/s]
    float leftSpeed; // [mm/s] positive value is forward
    float rightSpeed; // [mm/s]  negative value is forward
    int turnSpeedDiff = speed - 60; // [mm/s]
    
    // Logging variables
    driveLog driveLog[1];
    int driveLogIndex = 0;
    
    // States and special cases
    enum State state;
    bool hasFoundCup = false;
    bool lookingForCup = false;
    bool rightTurnDoubleCheck = false;
    bool leftTurnDoubleCheck = false;
    bool intersectionDoubleCheck = false;
    bool noLineDoubleCheck = false;
    int checkCupIteration = 5; // checks for cup each n iterations

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
    void followLine(); // Assumes a line is present, and executes one correction iteration to follow the line
    void fancyFollowLine();
    bool autoDrive(); // Drives until line is found, follows line when it's found. tries to grab cup, if found.
    void moveRobot(int steps1, int steps2); // drives the robot, positive is forward, negative is backwards
    void beginRobot(); // init motors and calibrate IRs
    void setRightSpeed(float _speed); // sets the speed for the right stepper, takes in [mm/s]
    void setLeftSpeed(float _speed); // sets the speed for the left stepper, takes in [mm/s]
    void rotateRobot(float degrees); // rotates robot, positive is clockwise, negative is counterclockwise
    void moveRobotDist(float distLeft, float distRight); // moves robot a given distance in [mm]
    void moveRobotDist(float dist); // moves robot a given distance in [mm]
    void determineState();
    void reverseDrive();
    void celebrate();

};