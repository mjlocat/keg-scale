#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "wifi.h"

ScaleWifi::ScaleWifi(Config *config) {
  this->config = config;
}
    
ScaleWifi::~ScaleWifi() {
  
}

bool ScaleWifi::connectWifi() {
  int i = 0;
  bool connected = false;
  bool connectFailed = false;
  WiFi.begin(config->getWifiSsid(), config->getWifiPassword());
  Serial.printf("Connecting to %s ...\n", config->getWifiSsid());
  while(!connected) {
    switch (WiFi.status()) {
      case WL_CONNECTED:
        connected = true;
        break;
      case WL_NO_SSID_AVAIL:
        Serial.printf("SSID %s cannot be reached\n", config->getWifiSsid());
        connectFailed = true;
        break;
      case WL_CONNECT_FAILED:
        Serial.printf("Connection to %s failed\n", config->getWifiSsid());
        connectFailed = true;
        break;
      case WL_WRONG_PASSWORD:
        Serial.printf("Password for %s is incorrect\n", config->getWifiSsid());
        connectFailed = true;
        break;
      case -1:
        Serial.printf("Connection to %s timed out\n", config->getWifiSsid());
        connectFailed = true;
        break;
    }
    if (i > 30 || connectFailed) {
      WiFi.disconnect(true); // Turn off station mode
      this->setupAp();
      return false;
    }
    delay(1000);
    Serial.println(++i);
  }
  Serial.println("Connecton Established");
  Serial.print("IP Address:\t");
  Serial.println(WiFi.localIP());
  Serial.printf("Signal strength: %d dBm\n", WiFi.RSSI());
  return true;
}

void ScaleWifi::setupAp() {
  Serial.printf("Invalid configuration, setting up Access Point...\n");
  WiFi.softAP("keg-scale-config");
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
}

bool ScaleWifi::isConnected() {
  return (WiFi.status() == WL_CONNECTED);
}
