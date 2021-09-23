#include "A4988.h"
#include "InfraredArray.h"

class Robot
{
private:
    A4988 stepper_left;
    A4988 stepper_right;
    InfraredArray irArray;
    int diameterDriveWheels = 65; // [mm]
    int speed = 200; // [mm/s]


public:
    Robot(A4988 stepper_left, A4988 stepper_right, InfraredArray irArray);
    int checkIfWorking();
    void setSpeed(int speed); // [mm/s]
    int speed2rpm(int speed);
    bool lookForLine(); // False until line is found
    bool followLine(); // Assumes a line is present, and executes one correction iteration to follow the line
    bool autoDrive(); // Drives until line is found, follows line when it's found. tries to grab cup, if found.
    
};


