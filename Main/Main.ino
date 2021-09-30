#include "BasicStepperDriver.h"
#include "MultiDriver.h"
#include "SyncDriver.h"
#include "InfraredArray.h"
#include "Robot.h"
#include "Gripper.h"
#include "stepper_robot/StepperRobot.h"

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
SyncDriver controller(stepper_left, stepper_right);
InfraredArray irArray(IR1, IR2, IR3, IR4, IR5);

Robot robot(stepper_left, stepper_right, irArray, controller);
// StepperRobot roboto();

void setup() {
    Serial.begin(115200);
    // Set target motor RPM to 1RPM and microstepping to 1 (full step mode)
    // stepper_left.begin(120, 1);
    // stepper_right.begin(120, 1);
    // controller.getMotor(0).begin(60,1);
    // controller.getMotor(1).begin(60,1);
    // controller.enable();
    robot.beginRobot();
}

void loop() {
    //stepper_left.rotate(360);
    // stepper_right.rotate(360);
    // delay(20);
    // Serial.println(irArray.getReadings().r1);
    //robot.moveRobot(200, 100);

    // Serial.println("drit");

    // robot.moveRobot(1000,1000);
    // robot.autoDrive();
    // robot.setSpeed(100);

    readings rd = robot.irArray.getDigitalReadings();
    Serial.println();
    Serial.print(rd.r0);
    Serial.print(" ");
    Serial.print(rd.r1);
    Serial.print(" ");
    Serial.print(rd.r2);
    Serial.print(" ");
    Serial.print(rd.r3);
    Serial.print(" ");
    Serial.print(rd.r4);
    
}


