#include <Arduino.h>
#include "A4988.h"
#include "SyncDriver.h"
#include "InfraredArray.h"
#include "Robot.h"
#include "Gripper.h"

// Setting the pins for left and right stepper motor, we don't use microstepping
#define MOTORINTERFACE = 1
#define DIR_L 2
#define STEP_L 3
#define DIR_R 4
#define STEP_R 5

// Setting the pins for the IR array
#define IR1 A0
#define IR2 A1
#define IR3 A2
#define IR4 A3
#define IR5 A4

// Setting the pins for the servos
#define grabServo 10
#define liftServo 11

// using a 200-step motor
#define MOTOR_STEPS 200

A4988 stepper_left(MOTOR_STEPS, DIR_L, STEP_L);
A4988 stepper_right(MOTOR_STEPS, DIR_R, STEP_R);
SyncDriver controller(stepper_left, stepper_right);
InfraredArray irArray(IR1, IR2, IR3, IR4, IR5);
Gripper gripper(liftServo, grabServo);
Robot robot(stepper_left, stepper_right, irArray, controller, gripper);

void setup() {
    Serial.begin(115200);
    robot.beginRobot();
}

void loop() {
    // robot.autoDrive();
    // Serial.println();
    // Serial.print(robot.irArray.getReadings().r0);
    // Serial.print(" ");
    // Serial.print(robot.irArray.getReadings().r1);
    // Serial.print(" ");
    // Serial.print(robot.irArray.getReadings().r2);
    // Serial.print(" ");
    // Serial.print(robot.irArray.getReadings().r3);
    // Serial.print(" ");
    // Serial.print(robot.irArray.getReadings().r4);  
}