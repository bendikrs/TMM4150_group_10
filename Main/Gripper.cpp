#include "Gripper.h"

Gripper::Gripper(int servoLiftPin, int servoGripPin)
:servoLiftPin(servoLiftPin), servoGripPin(servoGripPin)
{
    servoGrip.attach(servoGripPin);
    servoLift.attach(servoLiftPin);
}

void Gripper::grab()
{
    int i = gripOpened;
    for (i; i <= gripClosed; ++i){ // goes from gripOpened degrees to gripClosed degrees in steps of 1 degree
        servoGrip.write(i); // tell servo to go to position
        delay(15); 
    }
}

void Gripper::letGo()
{
    int i = gripClosed;
    for (i; i >= gripOpened; ++i){ // goes from gripClosed degrees to gripOpened degrees in steps of 1 degree
        servoGrip.write(i); // tell servo to go to position
        delay(15); 
    }
}