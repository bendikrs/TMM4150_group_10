#include "Robot.h"

Robot::Robot(A4988 stepper_left, A4988 stepper_right, InfraredArray irArray, SyncDriver controller, Gripper gripper):
stepper_left(stepper_left), stepper_right(stepper_right), irArray(irArray), controller(controller), gripper(gripper){}

void Robot::beginRobot(){
    // Initialize the robot
    this->controller.getMotor(0).begin(speed2rpm(this->speed), 1); //left motor
    this->controller.getMotor(1).begin(speed2rpm(this->speed), 1); //right motor
    this->controller.getMotor(0).setSpeedProfile(BasicStepperDriver::CONSTANT_SPEED);
    this->controller.getMotor(1).setSpeedProfile(BasicStepperDriver::CONSTANT_SPEED);

    this->gripper.initServos(); // Initialize the servos
    this->gripper.initUltrasonic(); // Initialize the ultrasonic sensor
}

int Robot::checkIfWorking(){
    // Check if the robot is working // Not used
    int avgIR;
    for (int i=1; i<3000; i++){
        avgIR = this->irArray.getAverage();
        delay(2);
    }
}

void Robot::followLine(){
    // Corrects the robot to follow the line
    // The robot will follow the line until it reaches the end of the line or the line is lost
    setLeftSpeed(this->speed);
    setRightSpeed(this->speed);

    if (!this->irArray.irReadings.r1){ // If on black line
        setLeftSpeed(this->speed - this->turnSpeedDiff);
        setRightSpeed(this->speed);
    }
    else if (!this->irArray.irReadings.r3){ // If on black line
        setLeftSpeed(this->speed);
        setRightSpeed(this->speed-this->turnSpeedDiff);
    }
    this->moveRobot(this->leftSpeed/10, this->rightSpeed/10); // Move the robot
}

void Robot::fancyFollowLine(){
    // Corrects the robot to follow the line // Not used
    /* returns
    0: No line found
    1: Detected left turn
    2: Detected right turn
    3: Detected intersection, need to choose to turn left or right
    4: Follows line
    */
    // readings ir;
    // ir = this->irArray.getMappedDigitalReadings();
    // float Kp = 1;
    // float Ki = 1;
    // float Kd = 1;
    // int error = this->irArray.calculatePosition();
    // int P,I,D;
    
    // P = error;
    // I = I + error;
    // D = error - this->lastError;
    // this->lastError = error;
    // int diffSpeed = P*Kp + I*Ki + D*Kd;

    // float leftSpeed = constrain(this->speed + diffSpeed, 0, this->maxSpeed);
    // float rightSpeed = constrain(this->speed - diffSpeed, 0, this->maxSpeed);


    // setLeftSpeed(leftSpeed);
    // setRightSpeed(rightSpeed);
}

bool Robot::autoDrive(){
    // Drives the robot automatically
    determineState(); // sets state
    switch (state)
    {
        case NOLINE: // No line found
            if (this->hasFoundCup){
                this->gripper.letGo();
                this->celebrate();
                delay(100000);
            } else if (this->lookingForCup){
                this->moveRobotDist(-80);
                this->rotateRobot(180);
            }
            break;

        case INTERSECTION: // Intersection found
            if (this->hasFoundCup){ // If cup found
                moveRobotDist(distAxelToSensorArray, distAxelToSensorArray);
                rotateRobot(90);
                this->moveRobotDist(50);
            }
            else{
                moveRobotDist(distAxelToSensorArray, distAxelToSensorArray);
                rotateRobot(-90);
                this->moveRobotDist(50);
            }
            break;

        case LEFTTURN: // Left turn found
            if (!this->hasFoundCup){
                moveRobotDist(distAxelToSensorArray, distAxelToSensorArray);
                rotateRobot(-90);
                this->moveRobotDist(30);
                this->lookingForCup = true; // Start to look for cup after turning left at the start of the track
            }else{ // rotates to the right
                moveRobotDist(distAxelToSensorArray, distAxelToSensorArray);
                rotateRobot(90);
            }
            
            break;

        case RIGHTTURN: // Right turn found
            if (!this->hasFoundCup){
                moveRobotDist(distAxelToSensorArray, distAxelToSensorArray);
                rotateRobot(-90); // always turn to left if not found cup
            }
            else{
                moveRobotDist(15, 15); // move past line
            }
            break;  


        case FOLLOWLINE: // Follows line
            if (this->hasFoundCup || this->checkCupIteration != 0){
                followLine();
            } 
            else 
            {
                this->checkCupIteration = 5;
                bool sonarRead = this->gripper.checkCup(20, 55);
                if(sonarRead){ // If cup found
                    this->moveRobotDist(30);
                    this->gripper.grab(); // Grab cup
                    this->moveRobotDist(-80); // To avoid going outside of the track
                    this->rotateRobot(180); // Turn around
                    this->moveRobotDist(5,5);
                    this->hasFoundCup = true;
                    this->lookingForCup = false; // Stop looking for cup
                }
            }

            if (this->lookingForCup){ // Iterates the variable If looking for cup
                this->checkCupIteration -= 1;
            }
            break;

        default:
            break;
    }
}

void Robot::determineState(){
    readings ir = this->irArray.getDigitalReadings();
    if(!ir.r0 && !ir.r4){ // both the left and right sensors are on black
        this->state = INTERSECTION;
    }
    else if(!ir.r0){ // if the left sensor is on black
        this->state = LEFTTURN;
    }
    else if(!ir.r4){// if the right sensor is on black
        this->state = RIGHTTURN;
    }
    else if (!ir.r1 || !ir.r2 || !ir.r3){ // one of the three middle sensors senses black
        this->state = FOLLOWLINE;
    }
    else if(ir.r1 && ir.r2 && ir.r3){ // Senses only white on the three middle sensors
        if (this->noLineDoubleCheck) // boolean variable to make sure the robot actually is not on a line
        { // after two no line readings, we run a additional check to see if the robot is on a line
            int8_t degs = 15;
            this->rotateRobot(15);
            for (int i = 0; i<6; i++) // rotates back and forth to check for line
            {
                readings irr = this->irArray.getDigitalReadings(); // reads the ir sensors
                if(!irr.r1 || !irr.r2 || !irr.r3) // if one of the middle sensors senses black
                {
                    this->rotateRobot(-degs);
                    degs = 0;
                    this->moveRobotDist(20);
                    this->state = FOLLOWLINE; // goes back to follow line
                    break;
                }
                else
                {
                    this->state = NOLINE;
                }
                this->rotateRobot(-5);
                degs -= 5;
            }
            this->rotateRobot(-degs);
            this->noLineDoubleCheck = false;
        }
        else
        {
            this->intersectionDoubleCheck = false;
            this->rightTurnDoubleCheck = false;
            this->leftTurnDoubleCheck = false;
            this->noLineDoubleCheck = true;
        }
    }
}

void Robot::moveRobot(int stepsLeft, int stepsRight){// stepsLeft is left motor, stepsRight is right motor
    this->controller.move(stepsLeft, -stepsRight);
    // adds steps to the log
    driveLog.leftSteps += stepsLeft; 
    driveLog.rightSteps -= stepsRight; 
}

void Robot::setSpeed(float _speed){ // sets the speed of the robot
    this->speed = _speed;
    this->controller.getMotor(0).setRPM(speed2rpm(_speed)); // left motor
    this->controller.getMotor(1).setRPM(speed2rpm(_speed)); // right motor
}

void Robot::setLeftSpeed(float _speed){ // sets the speed of the left motor
    // speed: int [mm/s]
    this->leftSpeed = _speed;
    this->controller.getMotor(0).setRPM(speed2rpm(_speed));
}

void Robot::setRightSpeed(float _speed){ // sets the speed of the right motor
    // speed: int [mm/s]
    this->rightSpeed = _speed;
    this->controller.getMotor(1).setRPM(speed2rpm(_speed));
}

float Robot::speed2rpm(float _speed){ // converts speed to rpm
    /* 
    speed [mm/s]
    output: RPM
    */
    return (_speed*60)/(3.141592*this->diameterDriveWheels);
}

void Robot::rotateRobot(float degrees){ // rotates the robot by a given amount of degrees around its center
    setLeftSpeed(this->speed);
    setRightSpeed(this->speed);
    float distance = (degrees/360)*this->wheelbase*3.141592;

    this->moveRobotDist(distance, -distance);
    setLeftSpeed(this->speed);
    setRightSpeed(this->speed);

}

void Robot::moveRobotDist(float distLeft, float distRight){ // moves the robot by a given amount of distance
    int stepsLeft = (distLeft/(this->diameterDriveWheels*3.141592)) * this->stepsPerRotation;
    int stepsRight = (distRight/(this->diameterDriveWheels*3.141592)) * this->stepsPerRotation;
    this->moveRobot(stepsLeft, stepsRight);
}

void Robot::moveRobotDist(float dist){ // moves the robot by a given amount of distance
    this->moveRobotDist(dist, dist);
}

void Robot::reverseDrive(){ // not used
    this->moveRobot(-this->driveLog.leftSteps, -this->driveLog.rightSteps);
}

void Robot::celebrate(){
    while (1){
        this->rotateRobot(360);
        this->gripper.letGo();
    }
}