#include "Arduino.h"
#include "InfraredArray.h"

InfraredArray::InfraredArray(int infra0, int infra1, int infra2, int infra3, int infra4)
:infra0(infra0), infra1(infra1), infra2(infra2), infra3(infra3), infra4(infra4)
{
    // Assuming leds are arranged from left to right, seen from above, pointing forward
    pinMode(infra0, INPUT);
    pinMode(infra1, INPUT);
    pinMode(infra2, INPUT);
    pinMode(infra3, INPUT);
    pinMode(infra4, INPUT);

}

readings InfraredArray::getReadings(){
    /*Updates and returns a readings struct
    The numbers are in percent, where 100% is a reading larger or equal to this->upperLim
    and 0% is a reading lower or equal to this->lowerLim
    */

    readings returnReadings;
    // returnReadings.r0 = analogRead(this->infra0);
    // returnReadings.r1 = analogRead(this->infra1);
    // returnReadings.r2 = analogRead(this->infra2);
    // returnReadings.r3 = analogRead(this->infra3);
    // returnReadings.r4 = analogRead(this->infra4);

    returnReadings.r0 = digitalRead(this->infra0);
    returnReadings.r1 = digitalRead(this->infra1);
    returnReadings.r2 = digitalRead(this->infra2);
    returnReadings.r3 = digitalRead(this->infra3);
    returnReadings.r4 = digitalRead(this->infra4);


    
    this->irReadings = returnReadings;
    return returnReadings;
}

int InfraredArray::getAverage(){
    /*Updates readings and return average reading over all sensors, in percent
    */
    readings ir = getReadings();
    return (ir.r0+ir.r1+ir.r2+ir.r3+ir.r4)/5;
}

void InfraredArray::setLowerLim(int lowerLim, int sensorIndex){
    this->lowerLim[sensorIndex] = lowerLim;
}

void InfraredArray::setUpperLim(int upperLim, int sensorIndex){
    this->upperLim[sensorIndex] = upperLim;
}

void InfraredArray::calibrateIRs(){
    readings manyReadings[100];
    // for (int i=0; i<100; i++){
    //     manyReadings[i] = this->getReadings();
    //     if (){

    //     }
    // }
}

readings InfraredArray::getDigitalReading(){
    readings returnReadings = getReadings();

    returnReadings.r0 = map(returnReadings.r0, this->lowerLim[0], this->upperLim[0], 0, 100);
    returnReadings.r1 = map(returnReadings.r1, this->lowerLim[1], this->upperLim[1], 0, 100);
    returnReadings.r2 = map(returnReadings.r2, this->lowerLim[2], this->upperLim[2], 0, 100);
    returnReadings.r3 = map(returnReadings.r3, this->lowerLim[3], this->upperLim[3], 0, 100);
    returnReadings.r4 = map(returnReadings.r4, this->lowerLim[4], this->upperLim[4], 0, 100);

    return returnReadings;
}