#ifndef MQ_INPUT_H
  #define MQ_INPUT_H
  #include<Arduino.h>

  #define CH4_PIN A0 //MQ4
  #define CO2_PIN A1 //MQ135
  #define RZERO 54.56

  float mqCH4Input();
  float mqCO2Input();
  float rawValue();
#endif
