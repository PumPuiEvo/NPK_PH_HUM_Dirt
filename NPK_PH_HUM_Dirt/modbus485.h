#include <ModbusMaster.h>

// Pin definitions for MAX485 control
#define MAX485_DE      D3  // Driver Enable pin
#define MAX485_RE_NEG  D2  // Receiver Enable pin

struct npk_ph {
  int n = 0;
  int p = 0;
  int k = 0;
  float ph = 0;
};

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

void modbusSetup()
{
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
}

struct npk_ph getNPK_PH(int debugpinled) {
  struct npk_ph npk_ph_result;
  uint8_t result;
  uint16_t startAddr = 0x0000;
  uint16_t offsetAddr = 50;
  uint16_t data[offsetAddr];

  // Request 10 holding registers starting at address 0x0000
  result = node.readHoldingRegisters(startAddr, offsetAddr);

  delay(100);
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

    int predictiveNitogen = int(( 1.2 *data[5] + 0.8*data[6])/2);
    npk_ph_result.n = predictiveNitogen > 1 ? predictiveNitogen : 0;
    npk_ph_result.p = data[5];
    npk_ph_result.k = data[6];
    npk_ph_result.ph = float(data[3])/10;
    // Serial.print("maybe nitro gen : ");
    // Serial.println(node.getResponseBuffer(30));

    digitalWrite(debugpinled, LOW);
    delay(500);
    digitalWrite(debugpinled, HIGH);
    delay(500);
    digitalWrite(debugpinled, LOW);
    delay(500);
    digitalWrite(debugpinled, HIGH);

  } else {
    // Display error message if the request fails
    Serial.print("Modbus Error: ");
    Serial.println(result);

    digitalWrite(debugpinled, LOW);
    delay(500);
    digitalWrite(debugpinled, HIGH);
    delay(500);
    digitalWrite(debugpinled, LOW);
    delay(500);
    digitalWrite(debugpinled, HIGH);
    delay(500);
    digitalWrite(debugpinled, LOW);
    delay(500);
    digitalWrite(debugpinled, HIGH);
  }

  return npk_ph_result;
}
