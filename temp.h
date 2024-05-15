#ifndef TEMP_H
#define TEMP_H
  #include <OneWire.h>
  #include <DallasTemperature.h>

  #define ONE_WIRE_BUS 2

  class Temp{
    public:
      Temp(int pin);
      void begin();
      float getTemperatureCelsius();
      float getTemperatureFahrenheit();
    private:
      OneWire oneWire;
      DallasTemperature sensors;
  };
#endif
