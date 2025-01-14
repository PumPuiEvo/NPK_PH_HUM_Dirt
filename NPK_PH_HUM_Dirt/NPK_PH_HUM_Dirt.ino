#include "hydro.h"
#include "post.h"
#include "modbus485.h"

#define DEBUGMODE false

// Constants 
const int hygrometer = A0;	//Hygrometer sensor analog pin output at pin A0 of Arduino

float soilHumidity;
struct npk_ph theNPK_PH;

void setup() {
  // Serial.begin(115200);
  pinMode(hygrometer, INPUT);
  modbusSetup();

  // wifiSetup();
}

void loop(){
  if (DEBUGMODE) {
    getHydorMeterDEBUG(hygrometer);
  }

  delay(100); // must have to give time modbus clear
  theNPK_PH = getNPK_PH();
  Serial.printf("%d %d %d %.1f\n",theNPK_PH.n, theNPK_PH.p, theNPK_PH.k, theNPK_PH.ph);

  soilHumidity = getHydorMeter(hygrometer);
  Serial.printf("Soil humidity: %.2f%%\n", soilHumidity);
  
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

