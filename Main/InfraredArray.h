#pragma once

struct readings
{
    int r1, r2, r3, r4, r5;
};
class InfraredArray
{
private:
    int infra1;
    int infra2;
    int infra3;
    int infra4;
    int infra5;
    int upperLim = 1023;
    int lowerLim = 1;
public:
    InfraredArray(int infra1, int infra2, int infra3, int infra4, int infra5);
    readings getReadings(); //Updates and returns a readings struct
    int getAverage(); //Updates readings and returns a int with the average reading fromm all sensors
    void setUpperLim(int upperLim);
    void setLowerLim(int lowerLim);

};