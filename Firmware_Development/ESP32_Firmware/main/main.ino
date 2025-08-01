/*
This was orginally boiler plate from the examples provided by the ESP32 Dev Board in Arduino
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <Arduino.h>
#include "main.hpp"

WiFiClient client;
const char *ssid = "";

int status = WL_IDLE_STATUS;
IPAddress server(1,1,1,1); //dummy IP

void setup() {
  Serial.begin(9600);

  Serial.println("Attempting to connect to WPA network...");

  Serial.print("SSID: ");

  Serial.println(ssid);


  status = WiFi.begin(ssid, pass);

  if ( status != WL_CONNECTED) {

    Serial.println("Couldn't get a wifi connection");

    // don't do anything else:

    while(true);

  }

  else {

    Serial.println("Connected to wifi");

    Serial.println("\nStarting connection...");

    // if you get a connection, report back via serial:

    if (client.connect(server, 8000)) {

      Serial.println("connected");

      // Make a HTTP request:

      client.println("GET /search?q=arduino HTTP/1.0");

      client.println();

    }

  }
  setup_i2s();

  //get avialiable memory
  Serial.printf("Total heap: %u bytes\n", ESP.getHeapSize());
  Serial.printf("Free heap: %u bytes\n", ESP.getFreeHeap());
  Serial.printf("Total PSRAM:%u bytes\n", ESP.getPsramSize());
  Serial.printf("Free PSRAM:%u bytes\n", ESP.getFreePsram());

  server.begin();
}

void loop() {

  while(client1.connected()){
    if (client.available()) {
    // https://docs.arduino.cc/language-reference/en/functions/wifi/client/?utm_source=chatgpt.com#returns-2
    // https://forum.arduino.cc/t/esp-32-wifi-client-write-buffer-size-unexpected-disconnetion-of-a-client/639193?utm_source=chatgpt.com

    client.write(sBuffer)//work on this more 

    char c = client.read();

    Serial.print(c);

    }
  }


  if (!client.connected()) {

    Serial.println();

    Serial.println("disconnecting.");

    client.stop();

    for(;;)

      ;

  }
}