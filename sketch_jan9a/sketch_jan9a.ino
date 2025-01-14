#include <SoftwareSerial.h>
#include <Wire.h>
#define RE D4
#define DE D3
#define RO D5
#define DI D6

#define led_on 23
#define led_capture 27
#define led_Soilsensor 25

const byte H[8] = {0x01,0x03,0x00,0x12,0x00,0x01,0x24,0x0F};   //Soil Humidity
const byte T[8] = {0x01,0x03,0x00,0x13,0x00,0x01,0x75,0xCF};   //Soil Temperature
const byte CE[8] = {0x01,0x03,0x00,0x15,0x00,0x01,0x95,0xCE};   //Soil Conductivity
const byte PH[8] = {0x01,0x03,0x00,0x06,0x00,0x01,0x64,0x0B};   //Soil PH
const byte N[8] = {0x01,0x03,0x00,0x1E,0x00,0x01,0xE4,0x0C};  //Soil Nitrogen
const byte P[8] = {0x01,0x03,0x00,0x1F,0x00,0x01,0xB5,0xCC};  //Soil Phosphorus
const byte K[8] = {0x01,0x03,0x00,0x20,0x00,0x01,0x85,0xC0};  //Soil Phosphorus

byte values[9];

float temperature_val = 0.00;
float humidity_val = 0.00;
float PH_val = 0.00;
int CE_val;
byte N_val, P_val, K_val;

SoftwareSerial mod(RO,DI);

void setup() {
  Serial.begin(9600);
  mod.begin(4800);

  pinMode(DE, OUTPUT);
  pinMode(RE, OUTPUT);
  pinMode(led_on, OUTPUT);
  pinMode(led_capture, OUTPUT);
  pinMode(led_Soilsensor, OUTPUT);
  digitalWrite(led_on, HIGH);
}

void loop() {
  humidity_val = Collect_Data_H();
  temperature_val = Collect_Data_T();
  CE_val = Collect_Data_CE();
  N_val = Collect_Data_N();
  P_val = Collect_Data_P();
  K_val = Collect_Data_K();
  PH_val = Collect_Data_PH();
  
  digitalWrite(led_capture, HIGH);
  delay(100);
  digitalWrite(led_capture, LOW);
  delay(100);

  Serial.print("Humidity: "); Serial.println(humidity_val);
  Serial.print("Temperature: "); Serial.println(temperature_val);
  Serial.print("CE: "); Serial.println(CE_val);
  Serial.print("PH: "); Serial.println(PH_val);
  Serial.print("N: "); Serial.println(N_val);
  Serial.print("P: "); Serial.println(P_val);
  Serial.print("K: "); Serial.println(K_val);
  Serial.println("----------------------");
  delay(500);
}

float Collect_Data_H(){
  float humidity;
  uint32_t startTime;
  uint8_t i;
  while(mod.available())mod.read();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  for(i=0;i<sizeof(H);i++)mod.write(H[i]);
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    i = 0;
    startTime = millis();
    while(millis() - startTime <= 500UL){  //2000UL
      if(mod.available() && i<sizeof(values)){
        values[i++] = mod.read();
      }   
    }
    humidity = (values[3]<<8 | values[4])* 0.1;   
    return humidity;
}

float Collect_Data_T(){
  float temperature;
  uint32_t startTime;
  uint8_t i;
  while(mod.available())mod.read();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  for(i=0;i<sizeof(T);i++)mod.write(T[i]);
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    i = 0;
    startTime = millis();
    while(millis() - startTime <= 500UL){
      if(mod.available() && i<sizeof(values)){
        values[i++] = mod.read();
      }
    }
 temperature = (values[3]<<8 | values[4])* 0.1;  
 return temperature;
}

byte Collect_Data_CE(){
  float ce;
  uint32_t startTime;
  uint8_t i;
  while(mod.available())mod.read();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  for(i=0;i<sizeof(CE);i++)mod.write(CE[i]);
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    i = 0;
    startTime = millis();
    while(millis() - startTime <= 750UL){
      if(mod.available() && i<sizeof(values)){
        values[i++] = mod.read();
      }  
  } 
 ce = (values[3]<<8 | values[4]);
 return ce; 
}

float Collect_Data_PH(){
  float ph;
  uint32_t startTime;
  uint8_t i;
  while(mod.available())mod.read();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  for(i=0;i<sizeof(PH);i++)mod.write(PH[i]);
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    i = 0;
    startTime = millis();
    while(millis() - startTime <= 500UL){
      if(mod.available() && i<sizeof(values)){
        values[i++] = mod.read();
      } 
  }
 ph = (values[3]<<8 | values[4])*0.01;  
 return ph;
}

byte Collect_Data_N(){
  float nitrogen;
  uint32_t startTime;
  uint8_t i;
  while(mod.available())mod.read();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  for(i=0;i<sizeof(N);i++)mod.write(N[i]);
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    i = 0;
    startTime = millis();
    while(millis() - startTime <= 500UL){
      if(mod.available() && i<sizeof(values)){
        values[i++] = mod.read();
      }  
 } 
 return values[4];
}

byte Collect_Data_P(){
  float phosphorus;
  uint32_t startTime;
  uint8_t i;
  while(mod.available())mod.read();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  for(i=0;i<sizeof(P);i++)mod.write(P[i]);
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    i = 0;
    startTime = millis();
    while(millis() - startTime <= 500UL){
      if(mod.available() && i<sizeof(values)){
        values[i++] = mod.read();
      }   
 }
 return values[4]; 
}

byte Collect_Data_K(){
  float potassium;
  uint32_t startTime;
  uint8_t i;
  while(mod.available())mod.read();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  for(i=0;i<sizeof(K);i++)mod.write(K[i]);
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    i = 0;
    startTime = millis();
    while(millis() - startTime <= 500UL){
      if(mod.available() && i<sizeof(values)){
        values[i++] = mod.read();
      }   
  } 
 return values[4]; 
}