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
// Em portugues: https://fperrotti.wikispaces.com/Stepper+Lib+Arduino
//
// DISCLAIMER:
// This software is furnished "as is", without technical support, and with no 
// warranty, express or implied, as to its usefulness for any purpose.
// ---------------------------------------------------------------------------
//
// Esta biblioteca é parte do projeto de desenvolvimento de
// robôs móveis desenvolvido por Francesco A. Perrotti na
// Fatec Americana.
// Pode ser usada para fins comerciais ou pessoais livremente,
// apenas deixe citado o autor.
//

#ifndef StepperMotor_h
#define StepperMotor_h

#include <Arduino.h>

class StepperMotor 
{
  public:
	StepperMotor();
	void init(int mPin_1, int mPin_2, int mPin_3, int mPin_4);

	byte nextStep(int direction);
	void motorPinsOut(byte pins);
	void turboOn();
	void turboOff();
    void halfStepOn();
    void halfStepOff();
  private:
	byte stepNumber;
	byte stepInc;
	int mPin_1, mPin_2, mPin_3, mPin_4;
};

#endif
