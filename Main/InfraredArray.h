#pragma once


struct readings{
    int r0, r1, r2, r3, r4;
};

class InfraredArray
{
private:
    // The input pins for the ir sensors
    int infra0;
    int infra1;
    int infra2;
    int infra3;
    int infra4;
     // used to calibrate the ir sensors
    int upperLim[5] = {512};
    int lowerLim[5] = {512};
    // contains the most recent readings from the sensors
    readings irReadings;
public:
    InfraredArray(int infra0, int infra1, int infra2, int infra3, int infra4);
    readings getReadings(); //Updates private variable "irReadings" and returns a readings struct
    readings getDigitalReadings(); //fetches the analog reading from sensors and based on the upperLim and LowerLim determines the binary outcome
    int getAverage(); //Updates readings and returns a int with the average reading fromm all sensors
    void setUpperLim(int upperLim, int sensorIndex);
    void setLowerLim(int lowerLim, int sensorIndex);
    void calibrateIRs();
    void updateUpperLowerLim(int sensorReading, int sensorIndex);

};