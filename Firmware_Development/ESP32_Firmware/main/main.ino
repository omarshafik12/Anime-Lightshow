/*
This was orginally boiler plate from the examples provided by the ESP32 Dev Board in Arduino
*/

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h> 

const char *ssid = "";
const char *password = "";
const char *apiKey = ""

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //get avialiable memory
  Serial.printf("Total heap: %u bytes\n", ESP.getHeapSize());
  Serial.printf("Free heap: %u bytes\n", ESP.getFreeHeap());
  Serial.printf("Total PSRAM:%u bytes\n", ESP.getPsramSize());
  Serial.printf("Free PSRAM:%u bytes\n", ESP.getFreePsram());

  //freeing neccesary PSRAM
  byte* psdRamBuffer = (byte*)ps_malloc(882000);

  server.begin();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    HTTPClient http;

    http.begin(client, "https://api.openai.com/v1/audio/transcriptions");
    http.addHeader("Content-Type", "application/json");

    int httpResponse = http.POST(openai payload);

    if (httpResponse > 0) {
      String response = http.getString();
    } else {
      Serial.print("Error involving post request")
    }
  }
}