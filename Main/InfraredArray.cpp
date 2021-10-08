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

readings InfraredArray::getAnalogReadings(){
    /*Updates and returns a readings struct, with analog values

    */

    readings returnReadings;
    returnReadings.r0 = analogRead(this->infra0);
    returnReadings.r1 = analogRead(this->infra1);
    returnReadings.r2 = analogRead(this->infra2);
    returnReadings.r3 = analogRead(this->infra3);
    returnReadings.r4 = analogRead(this->infra4);
    
    this->irReadings = returnReadings;
    return returnReadings;
}

readings InfraredArray::getDigitalReadings(){
    readings returnReadings;

    returnReadings.r0 = digitalRead(this->infra0);
    returnReadings.r1 = digitalRead(this->infra1);
    returnReadings.r2 = digitalRead(this->infra2);
    returnReadings.r3 = digitalRead(this->infra3);
    returnReadings.r4 = digitalRead(this->infra4);

    return returnReadings;
}

readings InfraredArray::getMappedDigitalReadings(){
    /*
    The numbers are in percent, where 100% is a reading larger or equal to this->upperLim
    and 0% is a reading lower or equal to this->lowerLim
    */
    readings returnReadings = getAnalogReadings();

    returnReadings.r0 = map(returnReadings.r0, this->lowerLim[0], this->upperLim[0], 0, 100);
    returnReadings.r1 = map(returnReadings.r1, this->lowerLim[1], this->upperLim[1], 0, 100);
    returnReadings.r2 = map(returnReadings.r2, this->lowerLim[2], this->upperLim[2], 0, 100);
    returnReadings.r3 = map(returnReadings.r3, this->lowerLim[3], this->upperLim[3], 0, 100);
    returnReadings.r4 = map(returnReadings.r4, this->lowerLim[4], this->upperLim[4], 0, 100);

    return returnReadings;
}

readings InfraredArray::getMappedBinaryReadings(){
    /*
    1: reading over 50%
    0: reading under 50%
    */
    readings returnReadings = getMappedDigitalReadings();

    returnReadings.r0 = returnReadings.r0 > 50 ? 1:0;
    returnReadings.r1 = returnReadings.r1 > 50 ? 1:0;
    returnReadings.r2 = returnReadings.r2 > 50 ? 1:0;
    returnReadings.r3 = returnReadings.r3 > 50 ? 1:0;
    returnReadings.r4 = returnReadings.r4 > 50 ? 1:0;

    return returnReadings;
}

int InfraredArray::getAverage(){
    /*Updates readings and return average reading over all sensors, in percent
    */
    readings ir = getAnalogReadings();
    return (ir.r0+ir.r1+ir.r2+ir.r3+ir.r4)/5;
}

void InfraredArray::setLowerLim(int lowerLim, int sensorIndex){
    this->lowerLim[sensorIndex] = lowerLim;
}

void InfraredArray::setUpperLim(int upperLim, int sensorIndex){
    this->upperLim[sensorIndex] = upperLim;
}

void InfraredArray::calibrateIRs(){
    /*Calibrates IR sensors lowerLim and upperLim
    Builtin LED blinks rapidly while calibrating
    */
    readings reading_i;
    for (int i=0; i<1000; i++){  
        reading_i = this->getAnalogReadings();
        updateUpperLowerLim(reading_i.r0, 0);
        updateUpperLowerLim(reading_i.r1, 1);
        updateUpperLowerLim(reading_i.r2, 2);
        updateUpperLowerLim(reading_i.r3, 3);
        updateUpperLowerLim(reading_i.r4, 4);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(10);
        digitalWrite(LED_BUILTIN, LOW);
        delay(10);

    }
}



void InfraredArray::updateUpperLowerLim(int sensorReading, int sensorIndex){
    if (sensorReading < this->lowerLim[sensorIndex]){
        setLowerLim(sensorReading, sensorIndex);
    }
    if (sensorReading > this->upperLim[sensorIndex]){
        setUpperLim(sensorReading, sensorIndex);
    }
}