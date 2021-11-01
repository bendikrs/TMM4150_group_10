#include "Robot.h"

Robot::Robot(A4988 stepper_left, A4988 stepper_right, InfraredArray irArray, SyncDriver controller, Gripper gripper):
stepper_left(stepper_left), stepper_right(stepper_right), irArray(irArray), controller(controller), gripper(gripper){}

void Robot::beginRobot(){
    // Inits motors and calibrates IRs
    // pinMode(LED_BUILTIN, OUTPUT); //setups builtin led
    // this->irArray.calibrateIRs();

    this->controller.getMotor(0).begin(speed2rpm(this->speed), 1); //left motor
    this->controller.getMotor(1).begin(speed2rpm(this->speed), 1); //right motor
    this->controller.getMotor(0).setSpeedProfile(BasicStepperDriver::CONSTANT_SPEED);
    this->controller.getMotor(1).setSpeedProfile(BasicStepperDriver::CONSTANT_SPEED);

    this->gripper.initServos();
    this->gripper.initUltrasonic();
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
    // this->driveLog[driveLogIndex].leftSpeed = this->leftSpeed;
    // this->driveLog[driveLogIndex].rightSpeed = this->rightSpeed;
    // this->driveLog[driveLogIndex].leftSteps = this->leftSpeed/10;
    // this->driveLog[driveLogIndex].rightSteps = this->rightSpeed/10;
    // ++driveLogIndex; 
    this->moveRobot(this->leftSpeed/10, this->rightSpeed/10);
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
    determineState(); //sets state
    switch (state)
    {
        case NOLINE:
            if (this->hasFoundCup){
                this->gripper.letGo();
                this->celebrate();
            } else if (this->lookingForCup){
                if(this->noLineDoubleCheck){
                    this->rotateRobot(180);
                    this->lookingForCup = false;
                }
            }
            this->noLineDoubleCheck = true;
            moveRobotDist(5,5);
            break;

        case INTERSECTION:
            if (this->hasFoundCup){
                moveRobotDist(distAxelToSensorArray, distAxelToSensorArray);
                rotateRobot(90);
            }
            else{
                moveRobotDist(distAxelToSensorArray, distAxelToSensorArray);
                rotateRobot(-90);
            }
            break;

        case LEFTTURN:
            if(leftTurnDoubleCheck){
                this->leftTurnDoubleCheck = false;
                moveRobotDist(distAxelToSensorArray, distAxelToSensorArray);
                rotateRobot(-90);
                this->lookingForCup = true;
                this->checkCupIteration = 5;
            }
            this->leftTurnDoubleCheck = true;
            break;

        case RIGHTTURN:
            if(rightTurnDoubleCheck){
                this->rightTurnDoubleCheck = false;
                    if (!this->hasFoundCup){
                        moveRobotDist(distAxelToSensorArray, distAxelToSensorArray);
                        rotateRobot(90);
                    }
                    else{
                        moveRobotDist(15, 15); //move past line
                    }
            }
            this->rightTurnDoubleCheck = true;
            break;  


        case FOLLOWLINE:
            if (this->hasFoundCup){
                followLine();
            }

            else if(this->checkCupIteration == 0) {
                this->checkCupIteration = 2;
                bool sonarRead = this->gripper.checkCup(25, 50);
                if(sonarRead){
                    this->gripper.grab();
                    this->rotateRobot(180);
                    this->moveRobotDist(5,5);
                    this->hasFoundCup = true;
                    this->lookingForCup = false;
                    this->checkCupIteration = 10000;
                }}
            else{
                this->checkCupIteration -= 1;
                followLine();
            }
            break;

        default:
            break;
    }
}

void Robot::determineState(){
    readings ir = this->irArray.getDigitalReadings();
    if(!ir.r0 && !ir.r4){
        if (this->intersectionDoubleCheck){
            this->intersectionDoubleCheck = false;
        }
        this->intersectionDoubleCheck = true;
        this->state = INTERSECTION;
    }
    else if(!ir.r0){
        if (this->leftTurnDoubleCheck){
            this->leftTurnDoubleCheck = false;
        }
        this->leftTurnDoubleCheck = true;
        this->state = LEFTTURN;
    }
    else if(!ir.r4){
        if (this->rightTurnDoubleCheck){
            this->rightTurnDoubleCheck = false;
        }
        this->rightTurnDoubleCheck = true;
        this->state = RIGHTTURN;

    }
    else if (!ir.r1 || !ir.r2 || !ir.r3){
        this->state = FOLLOWLINE;
    }
    else if(ir.r1 && ir.r2 && ir.r3){
        this->state = NOLINE;
    }
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
    setLeftSpeed(this->speed);
    setRightSpeed(this->speed);
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

void Robot::reverseDrive(){
    // rotateRobot(180);
    for(int i = 0; i <= driveLogIndex; ++i){
        setLeftSpeed(this->driveLog[i].leftSpeed);
        setRightSpeed(this->driveLog[i].rightSpeed);
        this->moveRobot(this->driveLog[i].leftSteps, this->driveLog[i].rightSteps);
    }
}

void Robot::celebrate(){
}