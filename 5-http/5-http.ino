#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const char* ssid = "Galaxy A135454";
const char* password = "aaaaaaaa";

unsigned long lastTime = 0;
unsigned long timerDelay = 15000;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting..");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if((millis() - lastTime) > timerDelay){
    if(WiFi.status() == WL_CONNECTED){
      HTTPClient http;
      String serverPath = "https://api.restful-api.dev/objects?id=3";
      http.begin(serverPath.c_str());
      int httpResponseCode = http.GET();
      if(httpResponseCode > 0){

        String payload = http.getString();
        Serial.println(payload);
      } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    } else {
      Serial.println("WiFi disconnected");
    }
    lastTime = millis();
  }

}
