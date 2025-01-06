#include "hydro.h"
#define DEBUGMODE false
//Constants 
const int hygrometer = A0;	//Hygrometer sensor analog pin output at pin A0 of Arduino
float soilHumidity;

void setup(){
	Serial.begin(9600);
  pinMode(hygrometer, INPUT);
}

void loop(){
  if (DEBUGMODE) {
    getHydorMeterDEBUG(hygrometer);
  }

  soilHumidity = getHydorMeter(hygrometer);
  Serial.printf("Soil humidity: %.2f%%\n", soilHumidity);

  delay(1000);
}
