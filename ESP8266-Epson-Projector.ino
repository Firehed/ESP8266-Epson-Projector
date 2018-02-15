#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "wifi.h"

ESP8266WebServer server(80);

void setup() {
  Serial.begin(9600);
  delay(100);

  connectToWifi();

  server.on("/", [](){
    Serial.println("");
    Serial.readString(); // Discard any buffer
    String command = String("PWR?");
    Serial.println(command);

    // This will output "OKa"; a=0/1 (off/on)

    server.send(200, "text/plain",  Serial.readString());
  });

  server.on("/power", HTTP_POST, [](){
    String state = server.arg("state");
    String mode;
    if (state == "on") {
        mode = "ON";
    } else if (state == "off") {
        mode = "OFF";
    } else {
      server.send(400, "text/plain", "invalid state");
      return;
    }
    Serial.println("");
    Serial.readString();
    String command = String("PWR " + mode);

    Serial.println(command);
    server.send(200, "text/plain", Serial.readString());
    // switch response
    // case "00": // standby
    // case "03": // cooling down
    // case "04": // standbny
    //   return off
    // case "01": // on
    // case "02": // warm up
    //   return on
    // case "05":
    //   return error
  });

  server.begin();
}

void connectToWifi() {
  WiFi.hostname(deviceName);
  WiFi.mode(WIFI_STA); // Client only
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("WiFi connected: ");
  Serial.println(WiFi.localIP());

  // Make the device discoverable at `epson-projector.local`
  if (!MDNS.begin(deviceName)) {
    Serial.println("mDNS responder setup failed");
  }
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWifi();
  }
  server.handleClient();
}
