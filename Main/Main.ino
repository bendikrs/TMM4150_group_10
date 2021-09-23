#include "BasicStepperDriver.h"
#include "MultiDriver.h"
#include "SyncDriver.h"
#include "InfraredArray.h"
#include "Robot.h"
#include "Gripper.h"

#include <Arduino.h>
#include "A4988.h"


// Setting the pins for left and right stepper motor, we don't use microstepping
#define DIR_L 2
#define STEP_L 3
#define DIR_R 4
#define STEP_R 5

// Setting the pins for the IR array
#define IR1 A1
#define IR2 A2
#define IR3 A3
#define IR4 A4
#define IR5 A5

// Setting the pins for the servos
#define grabServo 10
#define liftServo 11

// using a 200-step motor
#define MOTOR_STEPS 200

A4988 stepper_left(MOTOR_STEPS, DIR_L, STEP_L);
A4988 stepper_right(MOTOR_STEPS, DIR_R, STEP_R);

InfraredArray irArray(IR1, IR2, IR3, IR4, IR5);

Robot robot(stepper_left, stepper_right, irArray);

void setup() {
    Serial.begin(9600);
    // Set target motor RPM to 1RPM and microstepping to 1 (full step mode)
    // stepper_left.begin(60, 1);
    
    // stepper_right.begin(30, 1);
}

void loop() {
    //stepper_left.rotate(360);
    // stepper_right.rotate(360);
    // delay(20);
    // Serial.println(irArray.getReadings().r1);


    
}
