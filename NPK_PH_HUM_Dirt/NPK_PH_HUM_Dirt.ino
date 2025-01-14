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

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "hydro.h"
#define DEBUGMODE false
// Constants 
const int hygrometer = A0;	//Hygrometer sensor analog pin output at pin A0 of Arduino
float soilHumidity;

// WiFi Credentials
const char* ssid = "pumpui";
const char* password = "14325678";

// Server Details
const char* server = "43.228.86.203"; // Server IP
const int httpsPort = 443;            // HTTPS Port
const char* endpoint = "/api/v1/soil/post"; // API endpoint

WiFiClientSecure client;

void setup() {
  Serial.begin(115200);
  pinMode(hygrometer, INPUT);
  
  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".\n");
  }
  Serial.println("\nConnected to WiFi");

  // Optional: Skip SSL certificate verification (not recommended for production)
  client.setInsecure();

  // Connect to the server
  Serial.print("Connecting to server...");
  if (!client.connect(server, httpsPort)) {
    Serial.println("Connection failed!");
    return;
  }
  Serial.println("Connected to server!");

  // Construct JSON Payload
  String jsonPayload = "{\"N\": 10, \"P\": 20, \"K\": 21, \"Humidity\": 20}";

  // Construct HTTP POST Request
  String request = String("POST ") + endpoint + " HTTP/1.1\r\n" +
                   "Host: " + server + "\r\n" +
                   "Content-Type: application/json\r\n" +
                   "Content-Length: " + jsonPayload.length() + "\r\n" +
                   "Connection: close\r\n\r\n" +
                   jsonPayload;

  // Send the request
  Serial.println("Sending request:");
  Serial.println(request);
  client.print(request);

  // Wait for the response
  Serial.println("Response:");
  while (client.connected() || client.available()) {
    if (client.available()) {
      String response = client.readStringUntil('\n');
      Serial.println(response);
    }
  }
  
  client.stop();
  Serial.println("Disconnected from server.");
}

void loop(){
  if (DEBUGMODE) {
    getHydorMeterDEBUG(hygrometer);
  }

  soilHumidity = getHydorMeter(hygrometer);
  Serial.printf("Soil humidity: %.2f%%\n", soilHumidity);

  delay(1000);
}

