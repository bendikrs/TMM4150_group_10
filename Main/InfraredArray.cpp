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
    readings v; // values 1
    // readings v2; // values 2
    
    v.r0 = digitalRead(this->infra0);
    v.r1 = digitalRead(this->infra1);
    v.r2 = digitalRead(this->infra2);
    v.r3 = digitalRead(this->infra3);
    v.r4 = digitalRead(this->infra4);

    // v2.r0 = digitalRead(this->infra0)*v.r0;
    // v2.r1 = digitalRead(this->infra1)*v.r1;
    // v2.r2 = digitalRead(this->infra2)*v.r2;
    // v2.r3 = digitalRead(this->infra3)*v.r3;
    // v2.r4 = digitalRead(this->infra4)*v.r4;

    this->irReadings = v;
    return v;
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

int InfraredArray::calculatePosition(){
    readings ir = getMappedDigitalReadings(); //kan kanskje funke like bra me getAnalogReadings()
    // Use a weighted sum to find the approximated position
    int position;
    position = (100-ir.r0)*sensorPositions[0] + (100-ir.r1)*sensorPositions[1] 
               + (100-ir.r3)*sensorPositions[3] + (100-ir.r4)*sensorPositions[4];
    
    position = map(position,-100*64 - 100*32, 100*64 + 100*32, -5000, 5000);

    return position;

}
/*
100 80 30 80 100
30 80 90 80 100,   4480
100 80 30 80 100
*/

void InfraredArray::printAnalogReadings(){
    readings returnReadings = getAnalogReadings();
    Serial.println();
    Serial.print(returnReadings.r0);
    Serial.print(" ");
    Serial.print(returnReadings.r1);
    Serial.print(" ");
    Serial.print(returnReadings.r2);
    Serial.print(" ");
    Serial.print(returnReadings.r3);
    Serial.print(" ");
    Serial.print(returnReadings.r4); 
}