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
    
public:
    InfraredArray(int infra1, int infra2, int infra3, int infra4, int infra5);
    readings getReadings(); //Updates and returns a readings struct
};