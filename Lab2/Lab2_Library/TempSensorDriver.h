#ifndef TEMPSENSORDRIVER_H_
#define TEMPSENSORDRIVER_H_

class TempSensorDriver_ {
public:
    TempSensorDriver_();

    double temperature;

    void (*callback)();

    void setTemperature(int value);
};

#endif /* TEMPSENSORDRIVER_H_ */