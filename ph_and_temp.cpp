#include "ph_and_temp.h"


float ph_reading(){
  int arrr[10], temp;
  for(int i=0; i<10; i++){
    arrr[i] = analogRead(PH_SENSOR_PIN);
    Serial.println(arrr[i]);
    Serial.println("raw value");
    delay(30);
  }

  for(int i=0; i<9; i++){
    for(int j=i+1;j<10; j++){
      if(arrr[i]>arrr[j]){
        temp = arrr[i];
        arrr[i] = arrr[j];
        arrr[j] = temp;
      }
    }
  }
  unsigned long int avgval = 0;
  for(int i=2;i<8;i++)
  avgval+=arrr[i];
  float volt=(float)avgval*5.0/1024/6;
  float ph_act = -5.70 * volt + CALIBRATION_VALUE;

  return ph_act;
}
