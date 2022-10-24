#include <OneWire.h>
#include <TFT.h>  
#include <SPI.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

#define cs   10
#define dc   9
#define rst  8
OneWire  ds(7); 
TFT TFTscreen = TFT(cs, dc, rst);
float celsius;
float oldcelsius = 0;
int H;
int M;
int oldH = 0;
int oldM = 0;
char sensorPrintout[5];
char h[3];
char m[3];
unsigned long newTime;
unsigned long oldTime = 0;
int flag = 0;
bool heat = 1;
bool fitoLed = 0;
bool Led = 0;
bool eat = 0;
void setup() {
 
   TFTscreen.begin();
   TFTscreen.background(0, 0, 0);
   
   Serial.begin(9600);
   TFTscreen.setTextSize(2);
   TFTscreen.stroke(255, 255, 255);
   TFTscreen.text("Temp:", 3, 54);
   TFTscreen.text("Heating:", 3, 73);
   TFTscreen.text("FitoLed:", 3, 91);
   TFTscreen.text("Led:", 3, 106);
   TFTscreen.setTextSize(2);
      TFTscreen.stroke(0, 0, 0);
      TFTscreen.text("ON", 100, 93);     
      TFTscreen.setTextSize(2);
      TFTscreen.stroke(255, 0, 0);
      TFTscreen.text("OFF", 100, 93);

      TFTscreen.setTextSize(2);
      TFTscreen.stroke(0, 0, 0);
      TFTscreen.text("ON", 100, 108);     
      TFTscreen.setTextSize(2);
      TFTscreen.stroke(255, 0, 0);
      TFTscreen.text("OFF", 100, 108);
   pinMode(3, OUTPUT);
   pinMode(4, OUTPUT);
   pinMode(5, OUTPUT);
   pinMode(6, OUTPUT);
   digitalWrite(3, 1);
   digitalWrite(4, 1);
}

void loop() {
  Temperature();
  timeNow();
  disp();
  digitalWrite(3, heat);
  digitalWrite(5, Led);
  digitalWrite(6, fitoLed);

  if (eat == 1){
    digitalWrite(4, 0);
    delay(9500);
    digitalWrite(4, 1);
    eat = 0;
  }
}



void disp(){
  newTime = millis();

  
  if(H != oldH){
    TFTscreen.setTextSize(5);
    TFTscreen.stroke(0, 0, 0);
    TFTscreen.text(h, 1, 3);
    TFTscreen.stroke(181, 51, 138);
    String HH = String(H);
    HH.toCharArray(h, 3);
    TFTscreen.text(h, 1, 3);
    
    switch (H) {
    case 8:
      fitoLed = 1;
      Led = 1;
      eat = 1;
      TFTscreen.setTextSize(2);
      TFTscreen.stroke(0, 0, 0);
      TFTscreen.text("OFF", 100, 93);     
      TFTscreen.setTextSize(2);
      TFTscreen.stroke(0, 255, 0);
      TFTscreen.text("ON", 100, 93);

      TFTscreen.setTextSize(2);
      TFTscreen.stroke(0, 0, 0);
      TFTscreen.text("OFF", 100, 108);     
      TFTscreen.setTextSize(2);
      TFTscreen.stroke(0, 255, 0);
      TFTscreen.text("ON", 100, 108);
      break;
    case 12:
      fitoLed = 0;
      Led = 0;
      TFTscreen.setTextSize(2);
      TFTscreen.stroke(0, 0, 0);
      TFTscreen.text("ON", 100, 93);     
      TFTscreen.setTextSize(2);
      TFTscreen.stroke(255, 0, 0);
      TFTscreen.text("OFF", 100, 93);

      TFTscreen.setTextSize(2);
      TFTscreen.stroke(0, 0, 0);
      TFTscreen.text("ON", 100, 108);     
      TFTscreen.setTextSize(2);
      TFTscreen.stroke(255, 0, 0);
      TFTscreen.text("OFF", 100, 108);
      break;
    case 15:
      fitoLed = 1;
      Led = 1;
      TFTscreen.setTextSize(2);
      TFTscreen.stroke(0, 0, 0);
      TFTscreen.text("OFF", 100, 93);     
      TFTscreen.setTextSize(2);
      TFTscreen.stroke(0, 255, 0);
      TFTscreen.text("ON", 100, 93);

      TFTscreen.setTextSize(2);
      TFTscreen.stroke(0, 0, 0);
      TFTscreen.text("OFF", 100, 108);     
      TFTscreen.setTextSize(2);
      TFTscreen.stroke(0, 255, 0);
      TFTscreen.text("ON", 100, 108);
      break;
    case 18:
      fitoLed = 0;
      TFTscreen.setTextSize(2);
      TFTscreen.stroke(0, 0, 0);
      TFTscreen.text("ON", 100, 93);     
      TFTscreen.setTextSize(2);
      TFTscreen.stroke(255, 0, 0);
      TFTscreen.text("OFF", 100, 93);
      break;
    case 20:
      Led = 0;
      eat = 1;
      TFTscreen.setTextSize(2);
      TFTscreen.stroke(0, 0, 0);
      TFTscreen.text("ON", 100, 108);     
      TFTscreen.setTextSize(2);
      TFTscreen.stroke(255, 0, 0);
      TFTscreen.text("OFF", 100, 108);
      break;
    }
    oldH = H;
   }
   
  if(M != oldM){
    TFTscreen.setTextSize(5);
    TFTscreen.stroke(0, 0, 0);
    TFTscreen.text(m, 75, 3);
    TFTscreen.stroke(181, 51, 138);
    String MM = String(M);
    MM.toCharArray(m, 3);
    TFTscreen.text(m, 75, 3);
    oldM = M;
   }
  if (newTime - oldTime >= 500){
    if(flag == 0){
      TFTscreen.setTextSize(5);
      TFTscreen.stroke(0, 0, 0);
      TFTscreen.text(":", 55, 3);
      flag = 1;
    }
    else{
      TFTscreen.setTextSize(5);
      TFTscreen.stroke(181, 51, 138);
      TFTscreen.text(":", 55, 3);
      flag = 0;
    }
    oldTime = newTime;
  }
  //Serial.println(celsius);
  if(celsius != oldcelsius){
    TFTscreen.setTextSize(3);
    TFTscreen.stroke(0, 0, 0);
    TFTscreen.text(sensorPrintout, 60, 50);
    Serial.println(sensorPrintout);
    String sensorVal = String(celsius);
    sensorVal.toCharArray(sensorPrintout, 5);
    if(celsius < 25){
      TFTscreen.setTextSize(2);
      TFTscreen.stroke(0, 0, 0);
      TFTscreen.text("OFF", 100, 75);     
      TFTscreen.setTextSize(2);
      TFTscreen.stroke(0, 255, 0);
      TFTscreen.text("ON", 100, 75);
      
      TFTscreen.stroke(0, 0, 255); 
      heat = 0;
    }
    else if (celsius >= 25 && celsius <= 27){
      TFTscreen.stroke(0, 255, 0);
    }
    else {
      TFTscreen.setTextSize(2);
      TFTscreen.stroke(0, 0, 0);
      TFTscreen.text("ON", 100, 75);
      TFTscreen.setTextSize(2);
      TFTscreen.stroke(255, 0, 0);
      TFTscreen.text("OFF", 100, 75);
      
      TFTscreen.stroke(255, 0, 0);
      heat = 1;
    }
    TFTscreen.setTextSize(3);
    TFTscreen.text(sensorPrintout, 60, 50);
    oldcelsius = celsius;
    
  }

  
}


void Temperature(){
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
    if ( !ds.search(addr)) {
    ds.reset_search();
    //delay(250);
    return;
  }
  if (OneWire::crc8(addr, 7) != addr[7]) {
      return;
  }
   switch (addr[0]) {
    case 0x10:
      type_s = 1;
      break;
    case 0x28:
      type_s = 0;
      break;
    case 0x22:
      type_s = 0;
      break;
    default:
      return;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);       
  //delay(1000);     
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         
  for ( i = 0; i < 9; i++) {         
    data[i] = ds.read();
  }
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3;
    if (data[7] == 0x10) {
     
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
  
    if (cfg == 0x00) raw = raw & ~7;
    else if (cfg == 0x20) raw = raw & ~3;
    else if (cfg == 0x40) raw = raw & ~1;
  }
  celsius = (float)raw / 16.0;
 }

 void timeNow(){
  tmElements_t tm;

  if (RTC.read(tm)) {
    H = tm.Hour;
    M = tm.Minute;
    
    Serial.print("Time = ");
    Serial.print(H);
    Serial.write(':');
    Serial.print(M);
    Serial.println();
  }
 }
