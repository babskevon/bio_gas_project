#include "mq_input.h"
#include "temp.h"
//#include "MQ135.h"
#include "LiquidCrystal_I2C.h"
#include "ph_and_temp.h"
#include <SoftwareSerial.h>
#include <ArduinoJson.h>


SoftwareSerial gsm(11, 10);//(rxPin, txPin, inverse_logic) //SIM800L Tx & Rx is connected to Arduino #11 & #10
String APN = "internet";
String URL = "http://bio-gas-monitor.kesilo.tech/send-data/";
#define rst 12


Temp dallas(ONE_WIRE_BUS);

LiquidCrystal_I2C lcd(0x27, 16, 2);

void load_data(){
  Serial.println();
  lcd.clear();
  Serial.print("Initializing");
  lcd.print("Initializing");
  Serial.print(".");
  lcd.print(".");
  delay(700);
  Serial.print(".");
  lcd.print(".");
  delay(700);
  Serial.print(".");
  lcd.print(".");
  delay(700);
  Serial.print(".");
  lcd.print(".");
  delay(700);
}
void initialise(){
  lcd.begin();
  lcd.backlight();
  lcd.print("Welcome To");
  lcd.setCursor(0,1);
  lcd.print("Bio-Gas Monitor");
  delay(2500);
  if(isNetWorkConnected()){
    Serial.println("looking for network");
    load_data();
  }
  load_data();
  load_data();
  load_data();
  
  
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dallas.begin();
  gsm.begin(9600);
  gsm_config_gprs();
  initialise();

}

void display(int co2, int ch4, float temp, float ph =0.0){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CO2:");
  lcd.print(co2);
  lcd.print("% CH4:");
  lcd.print(ch4);
  lcd.setCursor(0,1);
  lcd.print("TP:");
  lcd.print(temp,1);
  lcd.print("C ");
  lcd.print("pH:");
  lcd.print(ph);
}




void loop() {
  // put your main code here, to run repeatedly:
  // float value = mqCH4Input();
  // MQ135 gasSensor = MQ135(A1);
  // float rzero = gasSensor.getRZero();
  
  int co2 = mqCO2Input(); //returning a percentage co2
  float ch4 = mqCH4Input(); //returning raw sensor value of methane sensor
  float tempC = dallas.getTemperatureCelsius();
  float tempF = dallas.getTemperatureFahrenheit();
  float ph = ph_reading();
  display(co2, ch4, tempC, ph);
  Serial.println();
  Serial.print("PH: ");
  Serial.print(ph);
  Serial.println();
  Serial.print("TempC: ");
  Serial.print(tempC);
  Serial.println();
  
  JsonDocument object;

  object["co2"] = co2;
  object["ch4"] = ch4;
  object["tempC"] = tempC;
  object["ph"] = ph;
  String data;
  serializeJson(object, data);
  gsm_post(data);
  delay(3000);

}

bool gsm_send(String command){
  gsm.println(command);
  long wtimer = millis();
  bool result = false;
  while(wtimer + 3000 > millis()){
    while( gsm.available() ){
      Serial.println("some response");
      Serial.print(gsm.read());
      result = true;
    }
  }
  Serial.println("no response");
  return result;
}
void gsm_config_gprs(){
  Serial.println(" --- CONFIG GPRS --- "); 
  gsm_send("AT+SAPBR=3,1,Contype,GPRS");
  gsm_send("AT+SAPBR=3,1,APN," + APN);
}

void gsm_post(String postdata){
  Serial.println(" --- Start GPRS & HTTP --- ");
  gsm_send("AT+SAPBR=1,1");
  gsm_send("AT+SAPBR=2,1");
  gsm_send("AT+HTTPINIT");
  gsm_send("AT+HTTPPARA=CID,1");
  gsm_send("AT+HTTPPARA=URL," + URL);
  gsm_send("AT+HTTPPARA=CONTENT,application/x-www-form-urlencoded");
  gsm_send("AT+HTTPDATA=192,5000");
  Serial.println(postdata);
  gsm_send(postdata);
  gsm_send("AT+HTTPACTION=1");
  gsm_send("AT+HTTPREAD");
  gsm_send("AT+HTTPTERM");
  gsm_send("AT+SAPBR=0,1");
}

bool isNetWorkConnected(){
  if(gsm_send("AT+CREG?")){
    String response = gsm.readStringUntil('\n');
    if (response.indexOf("+CREG: 0,1") != -1 || response.indexOf("+CREG: 0,5") != -1) {
      return false;
    }
  }
  return true;
}
