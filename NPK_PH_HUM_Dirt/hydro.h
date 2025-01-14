float getHydorMeterDEBUG(int port) {
  /*	Arduino Tutorial - How to use a Soil Hygrometer Module
	Dev: Michalis Vasilakis // Date: 18/05/2016 // www.ardumotive.com	*/

  //Constants 
  const int hygrometerIn = port;	//Hygrometer sensor analog pin output at pin A0 of Arduino
  //Variables 
  int value;

  value = analogRead(hygrometerIn);		//Read analog value 
  Serial.print(value);
  Serial.print("::");
	value = constrain(value,320,560);	//Keep the ranges!
	value = map(value,320,560,100,0);	//Map value : 400 will be 100 and 1023 will be 0
	Serial.print("Soil humidity: ");
	Serial.print(value);
	Serial.println("%");

  return value;
}

float getHydorMeter(int port) {
  //Constants 
  const int hygrometerIn = port;	//Hygrometer sensor analog pin output at pin A0 of Arduino
  //Variables 
  int value;

  value = analogRead(hygrometerIn);		//Read analog value 
	value = constrain(value,320,560);	//Keep the ranges!
	value = map(value,320,560,100,0);	//Map value : 400 will be 100 and 1023 will be 0

  return value;
}