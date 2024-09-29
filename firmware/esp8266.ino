#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Your Domain name with URL path or IP address with path
String serverName = "http://192.168.1.1:3000";

WiFiClient wifiClient;

// The following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

#define LED_BUILTINN D4

void setup() {
  Serial.begin(115200); 

  // Initialize the LED_BUILTIN pin as an output
  pinMode(LED_BUILTINN, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  // Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverName + "/api/status";
      
      // Your Domain name with URL path or IP address with path
      http.begin(wifiClient, serverPath.c_str());
      
      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
        // TODO: Use JSON parser to read "status" value
        if (payload == "{\"status\":\"ok\",\"responseData\":{\"status\":true}}") {
          // Turn the LED on (Note that LOW is the voltage level
          digitalWrite(LED_BUILTINN, LOW);
        } else {
          // Turn the LED off by making the voltage HIGH
          digitalWrite(LED_BUILTINN, HIGH);
        }
      } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        // Turn the LED off by making the voltage HIGH
        digitalWrite(LED_BUILTINN, HIGH);
      }
      // Free resources
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
