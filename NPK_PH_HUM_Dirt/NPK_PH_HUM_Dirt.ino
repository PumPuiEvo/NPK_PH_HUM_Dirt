#include "hydro.h"
#include "post.h"
#include "modbus485.h"

#define DEBUGMODE false
#define pinLEDdebug D4

// Constants 
const int hygrometer = A0;	//Hygrometer sensor analog pin output at pin A0 of Arduino
const unsigned long interval = 60000; // Interval in milliseconds (60 seconds)

unsigned long previousMillis = 0; // Store the last time data was sent
float soilHumidity;
struct npk_ph theNPK_PH;

void setup() {
  // Serial.begin(115200);
  pinMode(hygrometer, INPUT);
  modbusSetup();

  pinMode(pinLEDdebug, OUTPUT);
  wifiSetup(pinLEDdebug);
}

void loop(){
  if (DEBUGMODE) {
    getHydorMeterDEBUG(hygrometer);
  }

  delay(100); // must have to give time modbus clear
  theNPK_PH = getNPK_PH(pinLEDdebug);
  Serial.printf("%d %d %d %.1f\n",theNPK_PH.n, theNPK_PH.p, theNPK_PH.k, theNPK_PH.ph);
  delay(200);

  soilHumidity = getHydorMeter(hygrometer);
  Serial.printf("Soil humidity: %.2f%%\n", soilHumidity);

  unsigned long currentMillis = millis();
  // Check if 60 seconds have passed
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Update the last send time
    post_NPK_PH_Humidity(theNPK_PH.n, theNPK_PH.p, theNPK_PH.k, theNPK_PH.ph, soilHumidity, pinLEDdebug);
  }
  delay(5000);
}

