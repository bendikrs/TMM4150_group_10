// #include "Gripper.h"
// #include "Driver.h"
#include "Robot.h"


Robot::Robot(A4988 stepper_left, A4988 stepper_right, InfraredArray irArray):
stepper_left(stepper_left), stepper_right(stepper_right), irArray(irArray)
{
    stepper_right.begin(speed2rpm(this->speed), 1);
    stepper_left.begin(speed2rpm(this->speed), 1);
}

int Robot::checkIfWorking(){
    int avgIR;
    for (int i=1; i<3000; i++){
        avgIR = this->irArray.getAverage();
        
        
        delay(2);
    }
}

int Robot::speed2rpm(int speed){
    return speed/(3.141592*this->diameterDriveWheels);
}

void Robot::setSpeed(int speed){
    this->speed = speed;
}