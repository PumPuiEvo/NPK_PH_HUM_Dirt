// #include "hydro.h"
// #define DEBUGMODE false
// //Constants 
// const int hygrometer = A0;	//Hygrometer sensor analog pin output at pin A0 of Arduino
// float soilHumidity;

// void setup(){
// 	Serial.begin(9600);
//   pinMode(hygrometer, INPUT);
// }

// void loop(){
//   if (DEBUGMODE) {
//     getHydorMeterDEBUG(hygrometer);
//   }

//   soilHumidity = getHydorMeter(hygrometer);
//   Serial.printf("Soil humidity: %.2f%%\n", soilHumidity);

//   delay(1000);
// }

#include <SoftwareSerial.h>
#include <Wire.h>
 
// #define RE 2
// #define DE 0
// new test
#define RE D4
#define DE D3
#define RO D5
#define DI D6
 
//const byte code[]= {0x01, 0x03, 0x00, 0x1e, 0x00, 0x03, 0x65, 0xCD};

const byte nitro[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[] = {0x01,0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[] = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};
// const byte phaddress[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
const byte ph[] = {0x01,0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0b};//0x0B64
// const byte phX[] = {0x01,0x03, 0x00, 0x06, 0x00, 0x01, 0x0B, 0x64};

// const byte test[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0x85, 0xc0}; //pota from web sumtec
// const byte test[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xb5, 0xcc}; // nitro from web sumtec
// const byte test[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c}; // phos from web sumtec
const byte test_phos[] = {0x01,0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte test_pota[] = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};
const byte test_nitro[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};

// const byte test_phos[] = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};
// const byte nitro[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xb5, 0xcc};
// const byte pota[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0x85, 0xc0};
const byte getall[] = {0x01,0x03, 0x00, 0x0e, 0x00, 0x03, 0x34, 0x04};

byte values[20];
SoftwareSerial mod(RO,DI);
 
void setup() {
  Serial.begin(9600);
  mod.begin(4800);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  
  delay(3000);
}
 
void loop() {

  Serial.println("---------------"); 
  // // get_all();
  // Serial.println("---------------"); 

  byte val1, val2, val3, val4;
  val1 = nitrogen();
  delay(250);
  val2 = phosphorous();
  delay(250);
  val3 = potassium();
  delay(250);
  val4 = phydrogen();
  delay(250);
  
  // Serial.print("Nitrogen: ");
  // Serial.print(val1);
  // Serial.println(" mg/kg");
  // Serial.print("Phosphorous: ");
  // Serial.print(val2);
  // Serial.println(" mg/kg");
  // Serial.print("Potassium: ");
  // Serial.print(val3);
  // Serial.println(" mg/kg");
  // Serial.print("PH: ");
  // Serial.print(val4);
  // Serial.println("");


  int delayget = 1000;

  byte valTest;
  valTest = test_get_phos();
  delay(delayget);
  Serial.print("test_phos: ");
  Serial.println(valTest); 

  val1 = nitrogen();
  delay(250);
  val2 = phosphorous();
  delay(250);
  val3 = potassium();
  delay(250);
  val4 = phydrogen();
  delay(250);

  byte valTest2;
  valTest2 = test_get_pota();
  delay(delayget);
  Serial.print("test_pota: ");
  Serial.println(valTest2); 

  val1 = nitrogen();
  delay(250);
  val2 = phosphorous();
  delay(250);
  val3 = potassium();
  delay(250);
  val4 = phydrogen();
  delay(250);

  byte valTest3;
  valTest3 = test_get_nitro();
  delay(delayget);
  Serial.print("test_nitro: ");
  Serial.println(valTest3); 

  // temp = getByteNPK(phos);
  // delay(250);
  // Serial.printf("phos %d\n", temp); 

  // temp = getByteNPK(pota);
  // delay(250);
  // Serial.printf("pota %d\n", temp); 

  // temp = getByteNPK(test);
  // delay(250);
  // Serial.printf("test %d\n", temp); 
  
  delay(2000);
 
}

void get_all(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(getall,sizeof(getall))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    // delay(1);
    for(byte i=0;i<20;i++){
    // Serial.print(mod.read(),HEX);
    values[i] = mod.read();
    Serial.printf("%XH ", values[i]);
    }
    Serial.println();
  }
}

// byte testByte(){
//   digitalWrite(DE,HIGH);
//   digitalWrite(RE,HIGH);
//   delay(10);
//   if(mod.write(test_n,sizeof(test_n))==8){
//     digitalWrite(DE,LOW);
//     digitalWrite(RE,LOW);
//     delay(1);
//     for(byte i=0;i<7;i++){
//     // Serial.print(mod.read(),HEX);
//     values[i] = mod.read();
//     Serial.printf("%XH ", values[i]);
//     }
//     Serial.println();
//   }
//   return values[4];
// }

byte test_get_phos(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);

  if(mod.write(test_phos,sizeof(test_phos))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    delay(1);
    for(byte i=0;i<7;i++){
    // Serial.print(mod.read(),HEX);
    values[i] = mod.read();
    Serial.printf("%XH ", values[i]);
    }
    Serial.println();
  }
  return values[4];
}

byte test_get_pota(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);

  if(mod.write(test_pota,sizeof(test_pota))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    delay(1);
    for(byte i=0;i<7;i++){
    // Serial.print(mod.read(),HEX);
    values[i] = mod.read();
    Serial.printf("%XH ", values[i]);
    }
    Serial.println();
  }
  return values[4];
}

byte test_get_nitro(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);

  if(mod.write(test_nitro,sizeof(test_nitro))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    delay(1);
    for(byte i=0;i<7;i++){
    // Serial.print(mod.read(),HEX);
    values[i] = mod.read();
    Serial.printf("%XH ", values[i]);
    }
    Serial.println();
  }
  return values[4];
}

 
byte nitrogen(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(nitro,sizeof(nitro))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);

    for(byte i=0;i<7;i++){
    //Serial.print(mod.read(),HEX);
    values[i] = mod.read();
    //Serial.print(values[i],HEX);
    }
    //Serial.println();
  }
  return values[4];
}
 
byte phosphorous(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(phos,sizeof(phos))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    // delay(10);
    for(byte i=0;i<7;i++){
    //Serial.print(mod.read(),HEX);
    values[i] = mod.read();
    //Serial.print(values[i],HEX);
    }
    //Serial.println();
  }
  return values[4];
}
 
byte potassium(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(pota,sizeof(pota))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    // delay(10);
    for(byte i=0;i<7;i++){
    //Serial.print(mod.read(),HEX);
    values[i] = mod.read();
    //Serial.print(values[i],HEX);
    }
    Serial.println();
  }
  return values[4];
}

byte phydrogen() {
  // switch RS-485 to transmit mode
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);

  if(mod.write(ph,sizeof(ph))==8){
  // write out the message
  // for (int i = 0; i < sizeof(ph); i++) mod.write(ph[i]);
  // switching RS485 to receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // delay to allow response bytes to be received!
  // delay(10);

  // read in the received bytes
    for (byte i = 0; i < 7; i++) {
      values[i] = mod.read();
      // Serial.print(values[i], HEX);
      // Serial.print(' ');
    }
  }
  return values[4];
}
