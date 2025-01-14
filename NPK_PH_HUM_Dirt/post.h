#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

// WiFi Credentials
const char* ssid = "pumpui";
const char* password = "14325678";

// Server Details
const char* server = "43.228.86.203"; // Server IP
const int httpsPort = 443;            // HTTPS Port
const char* endpoint = "/api/v1/soil/post"; // API endpoint

WiFiClientSecure client;

void wifiSetup() {
  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".\n");
  }
  Serial.println("\nConnected to WiFi");
}

void post_NPK_PH_Humidity(int N, int P, int K, float PH, float Humidity) {
  // Optional: Skip SSL certificate verification (not recommended for production)
  client.setInsecure();

  // Connect to the server
  Serial.print("\nConnecting to server...");
  if (!client.connect(server, httpsPort)) {
    Serial.println("Connection failed!");
    return;
  }
  Serial.println("Connected to server!");

  // Construct JSON Payload
  String n = String("{\"N\": ") + String(N) + String(", ");
  String p = String("\"P\": ") + String(P) + String(", ");
  String k = String("\"K\": ") + String(K) + String(", ");
  String ph = String("\"PH\": ") + String(PH) + String(", ");
  String humidity = String("\"Humidity\": ") + String(Humidity) + String("}");
  String jsonPayload = n + p + k + ph + humidity;

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
  Serial.println("Disconnected from server.\n");
}