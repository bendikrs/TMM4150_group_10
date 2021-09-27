#include "Arduino.h"
#include "InfraredArray.h"

InfraredArray::InfraredArray(int infra1, int infra2, int infra3, int infra4, int infra5)
:infra1(infra1), infra2(infra2), infra3(infra3), infra4(infra4), infra5(infra5)
{
    // Assuming leds are arranged from left to right, seen from above, pointing forward
    pinMode(infra1, INPUT);
    pinMode(infra2, INPUT);
    pinMode(infra3, INPUT);
    pinMode(infra4, INPUT);
    pinMode(infra5, INPUT);

}

readings InfraredArray::getReadings(){
    /*Updates and returns a readings struct
    The numbers are in percent, where 100% is a reading larger or equal to this->upperLim
    and 0% is a reading lower or equal to this->lowerLim
    */

    readings returnReadings;
    // returnReadings.r1 = analogRead(this->infra1);
    // returnReadings.r2 = analogRead(this->infra2);
    // returnReadings.r3 = analogRead(this->infra3);
    // returnReadings.r4 = analogRead(this->infra4);
    // returnReadings.r5 = analogRead(this->infra5);

    returnReadings.r1 = digitalRead(this->infra1);
    returnReadings.r2 = digitalRead(this->infra2);
    returnReadings.r3 = digitalRead(this->infra3);
    returnReadings.r4 = digitalRead(this->infra4);
    returnReadings.r5 = digitalRead(this->infra5);

    // returnReadings.r1 = map(returnReadings.r1, this->lowerLim, this->upperLim, 0, 100);
    // returnReadings.r2 = map(returnReadings.r2, this->lowerLim, this->upperLim, 0, 100);
    // returnReadings.r3 = map(returnReadings.r3, this->lowerLim, this->upperLim, 0, 100);
    // returnReadings.r4 = map(returnReadings.r4, this->lowerLim, this->upperLim, 0, 100);
    // returnReadings.r5 = map(returnReadings.r5, this->lowerLim, this->upperLim, 0, 100);
    
    return returnReadings;
}

int InfraredArray::getAverage(){
    /*Updates readings and return average reading over all sensors, in percent
    */
    readings ir = getReadings();
    return (ir.r1+ir.r2+ir.r3+ir.r4+ir.r5)/5;
}

void InfraredArray::setLowerLim(int lowerLim){
    this->lowerLim = lowerLim;
}

void InfraredArray::setUpperLim(int upperLim){
    this->upperLim = upperLim;
}