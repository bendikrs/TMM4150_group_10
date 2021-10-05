#include "Robot.h"

Robot::Robot(A4988 stepper_left, A4988 stepper_right, InfraredArray irArray, SyncDriver controller):
stepper_left(stepper_left), stepper_right(stepper_right), irArray(irArray), controller(controller){}

void Robot::beginRobot(){
    // Inits motors and calibrates IRs
    pinMode(LED_BUILTIN, OUTPUT); //setups builtin led
    this->controller.getMotor(0).begin(speed2rpm(this->speed), 1); //left motor
    this->controller.getMotor(1).begin(speed2rpm(this->speed), 1); //right motor
    this->controller.getMotor(0).setSpeedProfile(BasicStepperDriver::CONSTANT_SPEED, 10, 10);
    this->controller.getMotor(1).setSpeedProfile(BasicStepperDriver::CONSTANT_SPEED, 10, 10);

    // this->irArray.calibrateIRs();
}

int Robot::checkIfWorking(){
    int avgIR;
    for (int i=1; i<3000; i++){
        avgIR = this->irArray.getAverage();
        delay(2);
    }
}

void Robot::followLine(){
    /* returns
    0: No line found
    1: Detected left turn
    2: Detected right turn
    3: Detected intersection, need to choose to turn left or right
    4: Follows line
    */
    readings ir;
    ir = this->irArray.getReadings();

    if(!ir.r2 && !ir.r4){ 
        setLeftSpeed(this->speed);
        setRightSpeed(this->speed);
    }
    else if (!ir.r2){ // viss svart
        setLeftSpeed(this->speed - this->turnSpeedDiff);
        setRightSpeed(this->speed);
    }
    else if (!ir.r4){ //viss svart 
        setLeftSpeed(this->speed);
        setRightSpeed(this->speed-this->turnSpeedDiff);
    }
}

bool Robot::autoDrive(){
    
    followLine(); // updates leftSpeed, rightSpeed and state
    
    switch (this->state)
    {
    case 0: // No line found
        // testing stuff
        this->rotateRobot(180);
        delay(1000);
        this->rotateRobot(180);
        break;
        
    default:
        this->moveRobot(this->leftSpeed/5, this->rightSpeed/5);
        break;
    }

    // ca. 1cm per iterasjon, ved speed=400 mm/s
    // this->controller.startMove(this->leftSpeed/5, -this->rightSpeed/5); // nextAction()
}

void Robot::moveRobot(int stepsLeft, int stepsRight){// stepsLeft is left motor, stepsRight is right motor
    this->controller.move(stepsLeft, -stepsRight);
}

void Robot::setSpeed(int _speed){
    this->speed = _speed;
    this->controller.getMotor(0).setRPM(speed2rpm(_speed));
    this->controller.getMotor(1).setRPM(speed2rpm(_speed));
}

void Robot::setLeftSpeed(int _speed){
    // speed: int [mm/s]
    this->leftSpeed = _speed;
    this->controller.getMotor(0).setRPM(speed2rpm(_speed));
}

void Robot::setRightSpeed(int _speed){
    // speed: int [mm/s]
    this->rightSpeed = _speed;
    this->controller.getMotor(1).setRPM(speed2rpm(_speed));
}

float Robot::speed2rpm(int _speed){
    /* 
    speed [mm/s]
    output: RPM
    */
    return (_speed*60)/(3.141592*this->diameterDriveWheels);
}

void Robot::rotateRobot(float degrees){
    setLeftSpeed(this->speed/5);
    setRightSpeed(this->speed/5);
    float distance = (degrees/360)*this->wheelbase*3.141592;

    this->moveRobotDist(distance, -distance);
    setLeftSpeed(this->speed);
    setRightSpeed(this->speed);

}

void Robot::moveRobotDist(float distLeft, float distRight){
    int stepsLeft = (distLeft/(this->diameterDriveWheels*3.141592)) * this->stepsPerRotation;
    int stepsRight = (distRight/(this->diameterDriveWheels*3.141592)) * this->stepsPerRotation;
    this->moveRobot(stepsLeft, stepsRight);
}