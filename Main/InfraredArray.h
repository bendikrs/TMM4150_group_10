


class InfraredArray
{
private:
    int infra1;
    int infra2;
    int infra3;
    struct readings
    {
        int r1, r2, r3;
    };
    
public:
    InfraredArray(int infra1, int infra2, int infra3);
    readings getReadings(); //Updates and returns readings
    ~InfraredArray();
};
