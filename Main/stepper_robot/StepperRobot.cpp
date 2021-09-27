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
// Esta biblioteca é parte do projeto de desenvolvimento de
// robôs móveis desenvolvido por Francesco A. Perrotti na
// Fatec Americana.
// Pode ser usada para fins comerciais ou pessoais livremente,
// apenas deixe citado o autor.
//

#include "StepperRobot.h"

extern PulseRobot *prob;

StepperRobot rob;

MoveRobot::MoveRobot()
{
  prob= this;
  moveOn= false;
}

boolean MoveRobot::addMove(int movType, int pulses)
{
  return cBuf.add(movType, pulses);
}

void MoveRobot::afterMove()
{
  if(!moveOn) return;

  if(!cBuf.itemWaiting())
	  moveOn= false;
  else
    moveNext();
}

void MoveRobot::moveNext()
{
  cBuf.getNext();
  move(cBuf.current.mvType, cBuf.current.pulses);
}

void MoveRobot::goNow()
{
  if(!cBuf.itemWaiting())
	  return;

  moveOn= true;
  moveNext();
}

void MoveRobot::stopAndClear()
{
  moveOn= false;
  decelStop();
  cBuf.clear();
}


