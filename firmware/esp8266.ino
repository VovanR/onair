#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Your Domain name with URL path or IP address with path
String serverAddress = "http://192.168.1.1:3000";

WiFiClient wifiClient;

// The following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

#define LED_BUILTIN D4

uint8_t CameraLedPin = D1;
uint8_t MicrophoneLedPin = D2;

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(CameraLedPin, OUTPUT);
  pinMode(MicrophoneLedPin, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  // Send an HTTP POST request every 5 seconds
  if ((millis() - lastTime) > timerDelay) {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      boolean isLedOn = false;

      // API path to get status
      String apiPath = serverAddress + "/api/status";
      http.begin(wifiClient, apiPath.c_str());
      // http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

      // Send HTTP GET request
      int httpResponseCode = http.GET();
      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
        // TODO: Use JSON parser to read "status" value
        if (payload == "{\"status\":\"ok\",\"responseData\":{\"camera\":true,\"microphone\":true}}") {
          setLedStatus(HIGH, HIGH);
        } else if (payload == "{\"status\":\"ok\",\"responseData\":{\"camera\":true,\"microphone\":false}}") {
          setLedStatus(HIGH, LOW);
        } else if (payload == "{\"status\":\"ok\",\"responseData\":{\"camera\":false,\"microphone\":true}}") {
          setLedStatus(LOW, HIGH);
        } else {
          setLedStatus(LOW, LOW);
        }
        toggleBuiltinLed(false);
      } else {
        toggleBuiltinLed(true);
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

void setLedStatus(bool camera, bool microphone) {
  digitalWrite(CameraLedPin, camera);
  digitalWrite(MicrophoneLedPin, microphone);
}

void toggleBuiltinLed(bool value) {
  // Builtin LED level is inverted: LOW - switch on, HIGH - switch off
  if (value == true) {
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
