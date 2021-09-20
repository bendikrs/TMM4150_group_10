// #include <Servo.h>
#include "BasicStepperDriver.h"
#include "MultiDriver.h"
#include "SyncDriver.h"
#include "InfraredArray.h"

#include <Arduino.h>
#include "A4988.h"

// using a 200-step motor
#define MOTOR_STEPS 200
// Setting the pins for left and right stepper motor
#define DIR_L 2
#define STEP_L 3
#define DIR_R 4
#define STEP_R 5

// Setting the pins for the IR array
#define INFRA1 A1
#define INFRA2 A2
#define INFRA3 A3
#define INFRA4 A4
#define INFRA5 A5

// Setting the pins for the servos
#define grabServo 10
#define liftServo 11

A4988 stepper_left(MOTOR_STEPS, DIR_L, STEP_L);
A4988 stepper_right(MOTOR_STEPS, DIR_R, STEP_R);

InfraredArray infraArray(INFRA1, INFRA2, INFRA3, INFRA4, INFRA5);

void setup() {
    Serial.begin(9600);
    // Set target motor RPM to 1RPM and microstepping to 1 (full step mode)
    // stepper_left.begin(60, 1);
    
    // stepper_right.begin(30, 1);
    Serial.println("hei");
}

void loop() {
    //stepper_left.rotate(360);
    // stepper_right.rotate(360);
    Serial.println(infraArray.getReadings().r1);
    // delay(20);
    
}
