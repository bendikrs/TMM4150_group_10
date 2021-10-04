// #include "Gripper.h"
// #include "Driver.h"
#include "Robot.h"


Robot::Robot(A4988 stepper_left, A4988 stepper_right, InfraredArray irArray, SyncDriver controller):
stepper_left(stepper_left), stepper_right(stepper_right), irArray(irArray), controller(controller)
{
    
    // stepper_right.begin(speed2rpm(this->speed), 1);
    // stepper_left.begin(speed2rpm(this->speed), 1);

    // stepper_right.enable(); // energize coils
    // stepper_left.enable();

    // stepper_left.setSpeedProfile(BasicStepperDriver::LINEAR_SPEED, 500, 500);
    // stepper_right.setSpeedProfile(BasicStepperDriver::LINEAR_SPEED, 500, 500);

    //stepper_left.nextAction
}

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

bool Robot::followLine(){
    readings ir;
    ir = this->irArray.getReadings();

    setLeftSpeed(this->speed);
    setRightSpeed(this->speed);
    
    if (!ir.r2){ // viss svart
        setLeftSpeed(this->speed - this->turnSpeedDiff);
        setRightSpeed(this->speed);
    }
    else if (!ir.r4){ //viss svart 
        setLeftSpeed(this->speed);
        setRightSpeed(this->speed-this->turnSpeedDiff);
    }
    
}


bool Robot::autoDrive(){
    followLine(); // oppdaterer leftSpeed og rightSpeed
    // this->controller.startMove(this->leftSpeed/40, -this->rightSpeed/40);
    // this->controller.nextAction();
    // ca. 1cm per iterasjon, ved speed=400 mm/s
    this->controller.move(this->leftSpeed/5, -this->rightSpeed/5);  //rotate(2*1080, -2*1080);
    
    // this->controller.startMove(this->leftSpeed/5, -this->rightSpeed/5);
}

void Robot::moveRobot(int steps1, int steps2){
    this->controller.move(steps1, -steps2);
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
    /* speed [mm/s]
    output: RPM
    */
    return (_speed*60)/(3.141592*this->diameterDriveWheels);
}