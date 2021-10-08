#include "Gripper.h"

Gripper::Gripper(int servoLiftPin, int servoGripPin)
:servoLiftPin(servoLiftPin), servoGripPin(servoGripPin)
{
    servoLift.attach(servoLiftPin);
    servoGrip.attach(servoGripPin);
}

void Gripper::grab()
{
    for (int i = gripOpened; i <= gripClosed; ++i){ // goes from gripOpened degrees to gripClosed degrees in steps of 1 degree
        servoGrip.write(i); // tell servo to go to position
        delay(15); 
    }

    for (int i = liftLower; i <= liftUpper; ++i){ // lifts object up
        servoLift.write(i);
        delay(15);
    }
}

void Gripper::letGo()
{
    for (int i = gripClosed; i >= gripOpened; --i){ // goes from gripClosed degrees to gripOpened degrees in steps of 1 degree
        servoGrip.write(i); // tell servo to go to position in degrees
        delay(15); 
    }

    for (int i = liftUpper; i >= liftLower; --i){ // placing object down
        servoLift.write(i);
        delay(15);
    }
}