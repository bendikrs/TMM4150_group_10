#include "Arduino.h"
#include "InfraredArray.h"

InfraredArray::InfraredArray(int infra1, int infra2, int infra3)
:infra1(infra1), infra2(infra2), infra3(infra3)
{
    pinMode(infra1, INPUT);
    pinMode(infra2, INPUT);
    pinMode(infra3, INPUT);
}

readings InfraredArray::getReadings(){

    readings returnReadings;
    returnReadings.r1 = 1;
    returnReadings.r2 = 1;
    returnReadings.r3 = 1;

    return returnReadings;
}