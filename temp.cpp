#include "temp.h"

Temp::Temp(int pin) : oneWire(pin), sensors(&oneWire) {}


void Temp::begin() {
  sensors.begin();
}


float Temp::getTemperatureCelsius() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

float Temp::getTemperatureFahrenheit() {
  return Temp::getTemperatureCelsius() * 9.0 / 5.0 + 32.0;
}
