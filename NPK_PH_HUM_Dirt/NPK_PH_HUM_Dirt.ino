#include "hydro.h"
#include "post.h"
#include <ModbusMaster.h>

#define DEBUGMODE false

// Constants 
const int hygrometer = A0;	//Hygrometer sensor analog pin output at pin A0 of Arduino

float soilHumidity;

// Pin definitions for MAX485 control
#define MAX485_DE      D3  // Driver Enable pin
#define MAX485_RE_NEG  D2  // Receiver Enable pin

// Instantiate ModbusMaster object
ModbusMaster node;

// Function to control the MAX485 in transmit mode
void preTransmission()
{
  digitalWrite(MAX485_DE, HIGH);
  digitalWrite(MAX485_RE_NEG, HIGH);
}

// Function to control the MAX485 in receive mode
void postTransmission()
{
  digitalWrite(MAX485_DE, LOW);
  digitalWrite(MAX485_RE_NEG, LOW);
}

void setup() {
  // Set up MAX485 control pins
  pinMode(MAX485_DE, OUTPUT);
  pinMode(MAX485_RE_NEG, OUTPUT);

  // Initially set to receive mode
  digitalWrite(MAX485_DE, LOW);
  digitalWrite(MAX485_RE_NEG, LOW);

  // Initialize Serial communication for Modbus
  Serial.begin(4800, SERIAL_8N1);  // Use hardware Serial (TX=GPIO1, RX=GPIO3)

  // Initialize Modbus communication
  node.begin(1, Serial);  // Slave ID = 1
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  // Debugging
  Serial.println("Modbus Master Initialized");
  // Serial.begin(115200);
  pinMode(hygrometer, INPUT);
  // modbusSetup();

  // wifiSetup();
}

void loop(){
  if (DEBUGMODE) {
    getHydorMeterDEBUG(hygrometer);
  }

  uint8_t result;
  uint16_t startAddr = 0x0000;
  uint16_t offsetAddr = 50;
  uint16_t data[offsetAddr];

  // Request 10 holding registers starting at address 0x0000
  result = node.readHoldingRegisters(startAddr, offsetAddr);

  // delay(200);
  // Check the result
  if (result == node.ku8MBSuccess) {
    Serial.println("Data received:");
    for (int i = 0; i < offsetAddr; i++) {
      data[i] = node.getResponseBuffer(i);
      // Serial.print("Register ");
      // Serial.print(i);
      // Serial.print(": ");
      // Serial.println(data[i]);
    }
    Serial.println("___________________");
    Serial.printf("PH: %.1f\n", float(data[3])/10);
    Serial.printf("Nitrogen: %d mg/kg\n", data[4]);
    Serial.printf("Phosphorus: %d mg/kg\n", data[5]);
    Serial.printf("Potassium: %d mg/kg\n", data[6]);
    Serial.println("-------------------");

    Serial.print("maybe nitro gen : ");
    Serial.println(node.getResponseBuffer(30));

  } else {
    // Display error message if the request fails
    Serial.print("Modbus Error: ");
    Serial.println(result);
  }

  soilHumidity = getHydorMeter(hygrometer);
  Serial.printf("Soil humidity: %.2f%%\n", soilHumidity);
  // getNPK_PH();

  // int N, P, K;
  // float PH, Humidity;
  // N = 0;
  // P = 0;
  // K = 0;
  // PH = 0;
  // Humidity = 0;
  // post_NPK_PH_Humidity(N, P, K, PH, Humidity);

  delay(5000);
}

