#ifndef TEMPSENSOR_H
#define TEMPSENSOR_H

#include <DHT.h>

class TempSensor
{
public:
    TempSensor(uint8_t pin, uint8_t type);
    void initialize();
    void readSensor();
    float getHumidity() const;
    float getTemperature(bool isFahrenheit = false) const;
    float getHeatIndex(bool isFahrenheit = false);

private:
    DHT dht;
    float humidity;
    float temperatureC;
    float temperatureF;
};

#endif // TEMPSENSOR_H
