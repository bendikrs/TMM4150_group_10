#include "Gripper.h"

Gripper::Gripper(int servoLiftPin, int servoGripPin, int trigpPin, int echoPin)
    : servoLiftPin(servoLiftPin), servoGripPin(servoGripPin), trigPin(trigPin), echoPin(echoPin)
{
}

void Gripper::grab()
{
    servoLift.write(liftLower);
    delay(500);
    servoGrip.write(gripOpened);
    delay(500);
    for (int i = gripOpened; i <= gripClosed; ++i){ // goes from gripOpened degrees to gripClosed degrees in steps of 1 degree
        servoGrip.write(i); // tell servo to go to position
        delay(10);
    }
    delay(500);
    for (int i = liftLower; i >= liftUpper; --i){ // lifts object up
        servoLift.write(i);
        delay(10);
    }
    delay(500);
}

void Gripper::letGo()
{
    for (int i = liftUpper; i <= liftLower; ++i)
    { // placing object down
        servoLift.write(i);
        delay(10);
    }
    for (int i = gripClosed; i >= gripOpened; --i)
    {                       // goes from gripClosed degrees to gripOpened degrees in steps of 1 degree
        servoGrip.write(i); // tell servo to go to position in degrees
        delay(10);
    }
}

void Gripper::initServos()
{
    servoLift.attach(servoLiftPin);
    servoGrip.attach(servoGripPin);
    servoLift.write(liftLower);
    servoGrip.write(gripOpened);
}

CupPos Gripper::checkForCup()
{
    CupPos position;
    // placeholder values
    position.direction = 0;
    position.distance = -1;
    return position;
}

void Gripper::initUltrasonic(){ //setup for sonar
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

int Gripper::calDist(){ //returnerer avstanden
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2; // Range : 2cm to 400 cm
    return distance/10;
}

bool Gripper::checkCup(int a, int b){
    return (calDist() < a && calDist() > b);
}