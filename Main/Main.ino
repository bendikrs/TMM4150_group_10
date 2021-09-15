// #include <Servo.h>
#include "BasicStepperDriver.h"
#include "MultiDriver.h"
#include "SyncDriver.h"

#include <Arduino.h>
#include "A4988.h"

// using a 200-step motor (most common)
#define MOTOR_STEPS 200
// configure the pins connected
#define DIR_L 2
#define STEP_L 3
#define DIR_R 8
#define STEP_R 9

#define MS1 10
#define MS2 11
#define MS3 12

#define INFRA1 A1
#define INFRA2 A2
#define INFRA3 A3

A4988 stepper_left(MOTOR_STEPS, DIR_L, STEP_L); //, MS1, MS2, MS3);
// A4988 stepper_right(MOTOR_STEPS, DIR_R, STEP_R); //, MS1, MS2, MS3);

void setup() {
    // Set target motor RPM to 1RPM and microstepping to 1 (full step mode)
    stepper_left.begin(60, 1);
    // stepper_right.begin(30, 1);
}

void loop() {
    // Tell motor to rotate 360 degrees. That's it.
    stepper_left.rotate(360);
    // stepper_right.rotate(360);
}