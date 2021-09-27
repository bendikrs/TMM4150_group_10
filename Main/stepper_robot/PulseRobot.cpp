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

typedef struct 
{
  char left, right;
} 
TMoveTable;

#define nMoves    9
#define timerTime 128
#define blinkDiv  16
#define cutPercentDefault 0.9

TMoveTable mvTable[nMoves] =
{// left rigth directions
  {   -1,    1  },  // 0: move ahead
  {    1,   -1  },  // 1: move back
  {    1,    1  },  // 2: spin left
  {   -1,   -1  },  // 3: spin right
  {    0,    1  },  // 4: turn left
  {   -1,    0  },  // 5: turn right
  {    0,   -1  },  // 6: back turn left
  {    1,    0  },  // 7: back turn right
  {    0,    0  },  // 8: brake (freio com as bobinas ativadas)
};


PulseRobot *prob;

boolean movingOn = false;
boolean blinkOn = false;
boolean cutOn = false;
unsigned long nextPulse, nextBlink, nextCut;


/// ISR Timer Routine
void timerEvent()
{
  if(!movingOn) return;
  unsigned long mcs= micros();

  if(mcs>=nextPulse)
	  prob->doNextPulse();
  else
	  if(blinkOn && (mcs>=nextBlink))
	    prob->doNextBlink();
    else
      if(cutOn && (mcs>=nextCut))
        prob->doNextCut();
}

void PulseRobot::doNextPulse()
{  
   if(!pulsesToGo)
   {
	   movingOn= false;
	   blinkOn= false;
	   mtLeft.motorPinsOut(0);
	   mtRight.motorPinsOut(0);

	   afterMove();
	   return;
   }

   unsigned long mcs= micros();

   mtLeft.nextStep(mvTable[moveType].left);
   mtRight.nextStep(mvTable[moveType].right);
   pulsesToGo--;
   cutOn= false;   

   if(pulsesToGo > acelUntil)
   {
	   pulseTime= max(timeOnCruise, pulseTime-timeStep*escaler);
	   escaler-=escalerStep;
     cutPercent+= percentStep;
     cutOn= true;
   }
   else
	   if(pulsesToGo <= decelAfter)
	   {
	     pulseTime= min(timeOnStart, pulseTime+timeStep*escaler);
	     escaler+=escalerStep;
       cutPercent-= percentStep;
       cutOn= true;
	   }

   // chopper control
   if(blinkOn)
   {
     blinkStatus= true;
     nextBlink= mcs + pulseTime/blinkDiv;
   }

   // cut controle 
   if(cutOn)
     nextCut= mcs + pulseTime*cutPercent;

   nextPulse= mcs+pulseTime;
}

void PulseRobot::doNextBlink()
{
  blinkStatus= !blinkStatus;

  if(!mvTable[moveType].left)
	  mtLeft.motorPinsOut(blinkStatus?leftPins:0);

  if(!mvTable[moveType].right)
	  mtRight.motorPinsOut(blinkStatus?rightPins:0);

  nextBlink= micros() + pulseTime/blinkDiv;
}

void PulseRobot::doNextCut()
{
  mtLeft.motorPinsOut(0);
  mtRight.motorPinsOut(0);
  cutOn= false;
}

/**
   startSpeed: pulses/second
   cruiseSpeed: pulses/second
   pulsesToCruise: how many pulses from start to cruise.
*/
void PulseRobot::init(int startSpeed, int cruiseSpeed, int pulsesToCruise)
{
  this->pulsesToCruise = pulsesToCruise;
  timeOnStart = 1000000L / startSpeed;    // tempo em microssegundos
  timeOnCruise= 1000000L / cruiseSpeed;

  Timer1.initialize(timerTime);
  Timer1.attachInterrupt( timerEvent ); 
}

void PulseRobot::setCutPercent(float initialCutPercent)
{
  this->initialCutPercent= initialCutPercent; 
}

boolean PulseRobot::move(char movType, int pulses)
{
   if (pulsesToGo) return false;
   if(movType>=nMoves) return false;
   moveType= movType;

   // calcula parametros para aceleração
   pulseTime= timeOnStart;
   timeStep = (pulseTime-timeOnCruise) / pulsesToCruise;

   decelAfter= min(pulses/2, pulsesToCruise);
   acelUntil= pulses-decelAfter;
   escaler= 1.5;
   escalerStep= (escaler-(2-escaler))/pulsesToCruise;
   cutPercent= initialCutPercent;
   percentStep= (1-cutPercent)/pulsesToCruise;

   // chopper
   blinkOn= (!mvTable[moveType].left) || (!mvTable[moveType].right);

   // start movement
   pulsesToGo= pulses;
   nextPulse= micros()+16;
   movingOn= true;

   return true;
}

void PulseRobot::stopNow()
{
   pulsesToGo= 0;
}

void PulseRobot::decelStop()
{
   pulsesToGo= min(pulsesToGo, pulsesToCruise);
   decelAfter= pulsesToGo;
}

boolean PulseRobot::isMoving()
{
  return movingOn;
}

void PulseRobot::initLeftMotor(int mPin_1, int mPin_2, int mPin_3, int mPin_4)
{
  mtLeft.init(mPin_1, mPin_2, mPin_3, mPin_4);
}

void PulseRobot::initRightMotor(int mPin_1, int mPin_2, int mPin_3, int mPin_4)
{
  mtRight.init(mPin_1, mPin_2, mPin_3, mPin_4);
}

void PulseRobot::turboOn()
{
  mtLeft.turboOn();
  mtRight.turboOn();
}

void PulseRobot::turboOff()
{
  mtLeft.turboOff();
  mtRight.turboOff();
}

void PulseRobot::halfStepOn()
{
  mtLeft.halfStepOn();
  mtRight.halfStepOn();
}

void PulseRobot::halfStepOff()
{
  mtLeft.halfStepOff();
  mtRight.halfStepOff();
}

PulseRobot::PulseRobot()
{
  initialCutPercent= cutPercentDefault;
}
