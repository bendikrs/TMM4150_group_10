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
    int upperLim[5] = {1023};
    int lowerLim[5] = {1};
    int sensorPositions[5] = {-64, -32, 0, 32, 64}; // positions of sensors measured from middle [mm]
    // contains the most recent readings from the sensors
public:
    readings irReadings; // measurements from sensors, is either in raw analog format (from 1 to 1023) or digital (1,0) based on last run function
    InfraredArray(int infra0, int infra1, int infra2, int infra3, int infra4);
    readings getAnalogReadings(); //Updates private variable "irReadings" and returns a readings struct by using analogread()
    readings getDigitalReadings(); //Updates private variable "irReadings" and returns a readings struct by using digitalread()
    readings getMappedDigitalReadings(); //Updates private variable "irReadings" and based on the upperLim and LowerLim map the signal from 0 to 100%
    readings getMappedBinaryReadings(); //Updates private variable "irReadings" and  based on the upperLim and LowerLim determines the binary outcome
    int getAverage(); //Updates readings and returns a int with the average reading fromm all sensors
    void setUpperLim(int upperLim, int sensorIndex); // used to calibrate the ir sensors
    void setLowerLim(int lowerLim, int sensorIndex); // used to calibrate the ir sensors
    void calibrateIRs(); // calibrates the ir sensors
    void updateUpperLowerLim(int sensorReading, int sensorIndex); // used to calibrate the ir sensors
    int calculatePosition(); // return number from -5000 to 5000, which is the position of the line from left to right seen from above
    void printAnalogReadings(); // prints the analog readings from the sensors

};