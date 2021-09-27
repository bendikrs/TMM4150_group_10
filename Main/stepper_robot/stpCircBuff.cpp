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

#ifndef STPCIRCBUFF_cpp
#define STPCIRCBUFF_cpp

#include "Arduino.h"
#include "stpCircBuff.h"

TBuffItem buf[BufLen];

CircularBuff::CircularBuff()
{
  clear();
}

void CircularBuff::clear()
{
   firstItem= 0;
   nextPos= 0;
   items= 0;
}

boolean CircularBuff::add(char mvType, int pulses)
{
  if(items>=BufLen) return false;

  buf[nextPos].mvType= mvType;
  buf[nextPos].pulses= pulses;
  nextPos= (nextPos+1) % BufLen;
  items++;
  return true;
}

void CircularBuff::getNext()
{
  if(items)
  {
	  current.mvType = buf[firstItem].mvType;
	  current.pulses = buf[firstItem].pulses;
	  firstItem= (firstItem+1) % BufLen;
	  items--;
  }
}

boolean CircularBuff::itemWaiting()
{
  return items > 0;
}

/*
int CircularBuff::freePos()
{
  return BufLen-items;
}
*/

#endif

