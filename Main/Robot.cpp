#include "Robot.h"

Robot::Robot(A4988 stepper_left, A4988 stepper_right, InfraredArray irArray, SyncDriver controller, Gripper gripper):
stepper_left(stepper_left), stepper_right(stepper_right), irArray(irArray), controller(controller), gripper(gripper){}

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
    setLeftSpeed(this->speed);
    setRightSpeed(this->speed);

    if (!this->irArray.irReadings.r1){ // viss svart
        setLeftSpeed(this->speed - this->turnSpeedDiff);
        setRightSpeed(this->speed);
    }
    else if (!this->irArray.irReadings.r3){ //viss svart 
        setLeftSpeed(this->speed);
        setRightSpeed(this->speed-this->turnSpeedDiff);
    }
}

void Robot::fancyFollowLine(){
    /* returns
    0: No line found
    1: Detected left turn
    2: Detected right turn
    3: Detected intersection, need to choose to turn left or right
    4: Follows line
    */
    readings ir;
    ir = this->irArray.getMappedDigitalReadings();
    float Kp = 1;
    float Ki = 1;
    float Kd = 1;
    int error = this->irArray.calculatePosition();
    int P,I,D;
    
    P = error;
    I = I + error;
    D = error - this->lastError;
    this->lastError = error;
    int diffSpeed = P*Kp + I*Ki + D*Kd;

    float leftSpeed = constrain(this->speed + diffSpeed, 0, this->maxSpeed);
    float rightSpeed = constrain(this->speed - diffSpeed, 0, this->maxSpeed);


    setLeftSpeed(leftSpeed);
    setRightSpeed(rightSpeed);
}

bool Robot::autoDrive(){
    
    followLine(); // updates leftSpeed, rightSpeed and state
    // fancyFollowLine();

    // this->controller.startMove(this->leftSpeed/5, -this->rightSpeed/5);
    // for (int i = 0; i<5;i++){
    //     this->controller.nextAction();
    // }

    this->moveRobot(this->leftSpeed/10, this->rightSpeed/10);
    determineState(); //sets state
    switch (state)//this->state)
    {
    case NOLINE: // No line found
        // testing stuff
        rotateRobot(180);
        delay(1000);
        rotateRobot(120);
        break;

    case LEFTTURN:
        rotateRobot(-90);
        break;

    case RIGHTTURN:
        rotateRobot(90);
        break;  

    case INTERSECTION:
        rotateRobot(90); // må endres
        break;

    case FOLLOWLINE:
        followLine();
        break;

    default:
        break;
    }
    this->moveRobot(this->leftSpeed/5, this->rightSpeed/5);
    
    // ca. 1cm per iterasjon, ved speed=400 mm/s
    // this->controller.startMove(this->leftSpeed/5, -this->rightSpeed/5); // nextAction()
}

void Robot::moveRobot(int stepsLeft, int stepsRight){// stepsLeft is left motor, stepsRight is right motor
    this->controller.move(stepsLeft, -stepsRight);
}

void Robot::setSpeed(float _speed){
    this->speed = _speed;
    this->controller.getMotor(0).setRPM(speed2rpm(_speed));
    this->controller.getMotor(1).setRPM(speed2rpm(_speed));
}

void Robot::setLeftSpeed(float _speed){
    // speed: int [mm/s]
    this->leftSpeed = _speed;
    this->controller.getMotor(0).setRPM(speed2rpm(_speed));
}

void Robot::setRightSpeed(float _speed){
    // speed: int [mm/s]
    this->rightSpeed = _speed;
    this->controller.getMotor(1).setRPM(speed2rpm(_speed));
}

float Robot::speed2rpm(float _speed){
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

void Robot::determineState(){
    readings ir = this->irArray.getDigitalReadings();
    if (ir.r1 && ir.r2 && ir.r3){
        this->state = NOLINE;
    }
    else if(!ir.r0){
        this->state = LEFTTURN;
    }
    else if(!ir.r4){
        this->state = RIGHTTURN;
    }
    else if(!ir.r0 && !ir.r4){
        this->state = INTERSECTION;
    }
    else if(!ir.r2){
        this->state = FOLLOWLINE;
    }
}