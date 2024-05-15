#include "mq_input.h"

float mqCH4Input(){
  unsigned int sensorValue = analogRead(CH4_PIN); 
  unsigned int outputValue = map(sensorValue, 0, 1023, 0, 1023);
  return outputValue;
}

float mqCO2Input(){
  unsigned int sensorValue = analogRead(CO2_PIN); 
  unsigned int outputValue = map(sensorValue - RZERO, 0, 1023, 0, 100);
  return outputValue;
}


float rawValue(){
  return analogRead(CO2_PIN);
}
