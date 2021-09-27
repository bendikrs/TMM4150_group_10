// ---------------------------------------------------------------------------
// Stepper Robot Library - v1.03 - 06/04/2014
//
// AUTHOR/LICENSE:
// Created by Francesco A. Perrotti - faperrotti@hotmail.com
// Copyright 2014 License: GNU General Public License v3
// http://www.gnu.org/licenses/
//
// LINKS:
// Project home: https://code.google.com/p/stepper-robot-arduino-lib/
// Wiki reference: https://code.google.com/p/stepper-robot-arduino-lib/wiki/Reference
// Playground page: http://playground.arduino.cc/Main/StepperRobot
//
// Portuguese: https://fperrotti.wikispaces.com/Stepper+Lib+Arduino
//
// DISCLAIMER:
// This software is furnished "as is", without technical support, and with no 
// warranty, express or implied, as to its usefulness for any purpose.
// ---------------------------------------------------------------------------
//
// Esta biblioteca � parte do projeto de desenvolvimento de
// rob�s m�veis desenvolvido por Francesco A. Perrotti na
// Fatec Americana.
// Pode ser usada para fins comerciais ou pessoais livremente,
// apenas deixe citado o autor.
//
//
// BACKGROUND:
// I am developing a robot that uses two stepper motors in a differential configuration. 
// I could not find a library that would meet my needs, so I decided to make one. This 
// library implements a class that takes care to synchronize the two motors to produce 
// programmed motions. All methods are non-blocking and library works with unipolar or 
// bipolar stepper motors. Advanced features were implemented for vibration reduction, 
// energy saving and protection of control circuits. 
//
// FEATURES:
// * Extremely precise movements.
// * Non-blocking methods.
// * Works with bipolar or unipolar motors.
// * Automatic acceleration and deceleration.
// * Allows storing movement commands in a buffer so they run in sequence.
// * Advanced capabilities for vibration control at low speeds.
// * Supports half-step and full-step mode with one or two coils.
// * Uses Timer 1 to control the pulse's timing.
// * Do not need a service function that is often called from the main loop.
//
// NOTE: This version uses TimerOne library to control the timer. This library 
//       is not included on package but you can download from Timer1 page at
//       http://playground.arduino.cc/Code/Timer1
//
// SETUP 
// rob.init(startSpeed(*), cruiseSpeed(*), pulsesToCruise);
// rob.initRightMotor(4,5,6,7); (**)
// rob.initLeftMotor(9,10,11,12); (**)
//
// (*) in pulses per second
// (**) all pins can be used, including analog pins
//
// SYNTAX:
// rob.move(movType, pulses) - immediate movement command
// rob.isMoving() - check if is moving
// rob.decelStop() - force deceleration and stop
// rob.stopNow() - immediate stop 
// rob.addMove(movType, pulses) - add movement in buffer
// rob.goNow() - execute movements on buffer
// rob.stopAndClear() - cancel movement and clear the buffer
// rob.halfStepOn() - half-step mode on
// rob.halfStepOff() - half-step mode off (full-step)
// rob.turboOn() - full step with two coils mode
// rob.turboOff() - full step with one coil mode
// rob.setCutPercent(initialCutPercent) - set the initial cut percent value (vibration control)
//
// HISTORY:
// 06/04/2014 v1.03 
//   * Some english errors corrected on code and comments.
//   * Licence adjust to fit to Google Code
//   * File headers comments updated with new links.
//      
// 05/04/2014 v1.02 
//   * File headers comments review.
//
// 03/04/2014 v1.01 
//   * Initial release.
// ---------------------------------------------------------------------------


#ifndef StepperRobot_h
#define StepperRobot_h

#include <Arduino.h>
#include "TimerOne-r11/TimerOne.h"
#include "StepperMotor.h"
#include "stpCircBuff.h"

// movements
#define mvAhead     0
#define mvBack      1
#define mvSpinL  	2
#define mvSpinR 	3
#define mvTurnL 	4
#define mvTurnR 	5
#define mvBkTurnL 	6
#define mvBkTurnR 	7
#define mvBrake 	8

// pulse control
class PulseRobot {
public:
	PulseRobot();
	void init(int startSpeed, int cruiseSpeed, int pulsesToCruise);
	void initLeftMotor(int mPin_1, int mPin_2, int mPin_3, int mPin_4);
	void initRightMotor(int mPin_1, int mPin_2, int mPin_3, int mPin_4);

	boolean move(char movType, int pulses);
	boolean isMoving();
	void stopNow();
	void decelStop();
	void turboOn();
	void turboOff();
    void halfStepOn();
    void halfStepOff();
    void setCutPercent(float initialCutPercent);

	// that should not be public
	void doNextPulse();
	void doNextBlink();
    void doNextCut();

protected:
	virtual void afterMove()=0;

private:
	StepperMotor mtLeft;
	StepperMotor mtRight;
	int pulsesToCruise;
	unsigned long timeOnStart;
	unsigned long timeOnCruise;
	unsigned long timeStep;
	unsigned long pulseTime;
	int acelUntil, decelAfter;
	float escaler, escalerStep;
	float cutPercent, percentStep;
	char moveType;
	int pulsesToGo;
	boolean blinkStatus;
	byte leftPins, rightPins;
	float initialCutPercent;
};

// buffer control
class MoveRobot: public PulseRobot {
public:
	MoveRobot();
	boolean addMove(int movType, int pulses);
	void goNow();
	void stopAndClear();
protected:
	void afterMove();
private:
	CircularBuff cBuf;
	boolean moveOn;
	void moveNext();
};

// publish class
class StepperRobot: public MoveRobot{
};

// instance
extern StepperRobot rob;

#endif

