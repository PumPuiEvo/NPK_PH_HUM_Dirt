#include <ModbusMaster.h>

#define RXD2 D5 //RO
#define TXD2 D6 //DI

#define MAX485_DE      D3
#define MAX485_RE_NEG  D4

uint8_t i = 0;
unsigned long last_time = 0;

// instantiate ModbusMaster object
ModbusMaster node;

void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
  delay(1);
}

void postTransmission()
{
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
  delay(1);
}

void setup()
{
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  
  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);

  // Modbus communication runs at 115200 baud
  Serial.begin(115200);
  Serial1.pins(TXD2, RXD2);
  Serial1.begin(4800,SERIAL_8N1);
  
  // Modbus slave ID 2
  node.begin(1, Serial1);
  // Callbacks allow us to configure the RS485 transceiver correctly
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}


void loop()
{
  uint8_t result = 0;
  uint8_t respons = 0;
  uint16_t data[10];

 if(millis()- last_time > 1000)
 {
 
  i++;
  if (i > 100)
  {
    i = 0; 
  }

  node.writeSingleRegister(0x40000,i);
  //node.writeSingleRegister(1,456);

  //node.setTransmitBuffer(0x40002, i);
  //node.setTransmitBuffer(0x40003, 888);
  //respons = node.writeMultipleRegisters(0x40000, 4);
  
  result = node.readHoldingRegisters(0x0000, 10);
  if (getResultMsg(&node, result)) {
    Serial.println("Read data from Slave");
    for (int j = 0; j < 10; j++){
      data[j] = node.getResponseBuffer(j);
      Serial.println(data[j]);
    }
  }
    
   last_time = millis();
 }

}

bool getResultMsg(ModbusMaster *node, uint8_t result) 
{
  String tmpstr2 = "\r\n";
  switch (result) 
  {
  case node->ku8MBSuccess:
    return true;
    break;
  case node->ku8MBIllegalFunction:
    tmpstr2 += "Illegal Function";
    break;
  case node->ku8MBIllegalDataAddress:
    tmpstr2 += "Illegal Data Address";
    break;
  case node->ku8MBIllegalDataValue:
    tmpstr2 += "Illegal Data Value";
    break;
  case node->ku8MBSlaveDeviceFailure:
    tmpstr2 += "Slave Device Failure";
    break;
  case node->ku8MBInvalidSlaveID:
    tmpstr2 += "Invalid Slave ID";
    break;
  case node->ku8MBInvalidFunction:
    tmpstr2 += "Invalid Function";
    break;
  case node->ku8MBResponseTimedOut:
    tmpstr2 += "Response Timed Out";
    break;
  case node->ku8MBInvalidCRC:
    tmpstr2 += "Invalid CRC";
    break;
  default:
    tmpstr2 += "Unknown error: " + String(result);
    break;
  }
  Serial.println(tmpstr2);
  return false;
}