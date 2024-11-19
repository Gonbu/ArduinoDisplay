#include "tempSensor.h"
#include <Arduino.h>

TempSensor::TempSensor(uint8_t pin, uint8_t type) : dht(pin, type), humidity(0), temperatureC(0), temperatureF(0) {}

void TempSensor::initialize()
{
  dht.begin();
}

void TempSensor::readSensor()
{
  humidity = dht.readHumidity();
  temperatureC = dht.readTemperature(false);
  temperatureF = dht.readTemperature(true);

  if (isnan(humidity) || isnan(temperatureC) || isnan(temperatureF))
  {
    Serial.println("Failed to read from DHT sensor!");
    humidity = temperatureC = temperatureF = NAN;
  }
}

float TempSensor::getHumidity() const
{
  return humidity;
}

float TempSensor::getTemperature(bool isFahrenheit) const
{
  return isFahrenheit ? temperatureF : temperatureC;
}

float TempSensor::getHeatIndex(bool isFahrenheit)
{
  return dht.computeHeatIndex(isFahrenheit ? temperatureF : temperatureC, humidity, isFahrenheit);
}
