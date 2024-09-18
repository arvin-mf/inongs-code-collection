#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const char* ssid = "Galaxy A135454";
const char* password = "aaaaaaaa";

String apiUrl = "https://api.restful-api.dev/objects";

unsigned long lastTime = 0;
unsigned long timerDelay = 15000;

void objRec(JSONVar myObject) {
  Serial.println("{");
  for (int x = 0; x < myObject.keys().length(); x++) {
    if ((JSON.typeof(myObject[myObject.keys()[x]])).equals("object")) {
      Serial.print(myObject.keys()[x]);
      Serial.println(" : ");
      objRec(myObject[myObject.keys()[x]]);
    }
    else {
      Serial.print(myObject.keys()[x]);
      Serial.print(" : ");
      Serial.println(myObject[myObject.keys()[x]]);
    }
  }
  Serial.println("}");
}

void parsing(JSONVar myObject){
  if (JSON.typeof(myObject) == "undefine"){
    Serial.println("parsing gagal!!");
    return;
  }
  if (myObject.hasOwnProperty("data")){
    Serial.print("\"Capacity\": ");
    Serial.println((const char*) myObject["data"]["Capacity"]);
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Sedang melakukan Koneksi dengan WIFi ...");
  while(WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.println("...");
  }
  Serial.print("Berhasil terkoneksi ke WiFi dengan IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if ((millis() - lastTime) > timerDelay){
    if (WiFi.status() == WL_CONNECTED){
      HTTPClient http;
      String apiPath = apiUrl + "?id=10";
      http.begin(apiPath.c_str());
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0){
        Serial.print("HTTP response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);

        payload.remove(payload.length()-1, 1);
        payload.remove(0, 1);
        Serial.println(payload);

        JSONVar myObject = JSON.parse(payload);

        parsing(myObject);
        // objRec(myObject);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    }
    else{
      Serial.println("Sedang tidak terhubung dengan WiFi!!!");
    }
    lastTime = millis();
  }
}
