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

    readings returnReadings;
    returnReadings.r1 = analogRead(this->infra1);
    returnReadings.r2 = analogRead(this->infra2);
    returnReadings.r3 = analogRead(this->infra3);
    returnReadings.r4 = analogRead(this->infra4);
    returnReadings.r5 = analogRead(this->infra5);

    return returnReadings;
}