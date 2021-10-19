#include "Gripper.h"

Gripper::Gripper(int servoLiftPin, int servoGripPin)
:servoLiftPin(servoLiftPin), servoGripPin(servoGripPin)
{}

void Gripper::grab()
{
    servoLift.write(liftLower);
    delay(500);
    servoGrip.write(gripOpened);
    delay(1000);
    for (int i = gripOpened; i <= gripClosed; ++i){ // goes from gripOpened degrees to gripClosed degrees in steps of 1 degree
        servoGrip.write(i); // tell servo to go to position
        delay(10); 
    }
    delay(1000);
    for (int i = liftLower; i >= liftUpper; --i){ // lifts object up
        servoLift.write(i);
        delay(10);
    }
    delay(1000);
}

void Gripper::letGo()
{
    for (int i = liftUpper; i <= liftLower; ++i){ // placing object down
        servoLift.write(i);
        delay(10);
    }
    for (int i = gripClosed; i >= gripOpened; --i){ // goes from gripClosed degrees to gripOpened degrees in steps of 1 degree
        servoGrip.write(i); // tell servo to go to position in degrees
        delay(10); 
    }

}

void Gripper::initServos(){
    servoLift.attach(servoLiftPin);
    servoGrip.attach(servoGripPin);
}