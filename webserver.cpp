#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "webserver.h"
#include "html.h"

ScaleWebServer::ScaleWebServer(Config *config) {
  this->config = config;
  this->server = new AsyncWebServer(80);
  this->setupRoutes();
}

ScaleWebServer::~ScaleWebServer() {
  
}

void ScaleWebServer::onGetRoot(AsyncWebServerRequest *request) {
  request->send_P(200, "text/html", index_html, [this](const String &var) {
    return this->indexProcessor(var);
  });
}

void ScaleWebServer::receiveUpdateConfigData(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  Serial.println("in /updateconfig data handler");
  Serial.printf("len = %i\n", len);
  Serial.printf("index = %i\n", index);
  Serial.printf("total = %i\n", total);
  for (int i = 0; i < len; i++) {
    Serial.print((char)data[i]);
  }
  Serial.println("");
  if (total > JSON_DATA_LEN) {
    request->send(413, "text/plain", "ERROR: Request too large. Increase JSON_DATA_LEN");
  } else {
    strncpy(((char*)this->tmp_json)+index, (char*)data, len);
  }
}

void ScaleWebServer::processUpdateConfigData(AsyncWebServerRequest *request) {
  Serial.println("in /updateconfig request handler");
  Serial.println(this->tmp_json);
  StaticJsonDocument<JSON_DATA_LEN> jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, this->tmp_json, strlen(this->tmp_json));
  if (error) {
    Serial.println(error.c_str());
    request->send(400, "text/plain", "ERROR: Invalid Request");
    return;
  }
  const char *wifi_ssid = jsonDoc["wifi_ssid"];
  const char *wifi_password = jsonDoc["wifi_password"];
  const char *mqtt_broker = jsonDoc["mqtt_broker"];
  const char *mqtt_topic = jsonDoc["mqtt_topic"];
  const char *mqtt_client_id = jsonDoc["mqtt_client_id"];
  const char *mqtt_port_str = jsonDoc["mqtt_port"];
  uint16_t mqtt_port = atoi(mqtt_port_str);
  this->config->setWifiSsid(wifi_ssid);
  this->config->setWifiPassword(wifi_password);
  this->config->setMqttBroker(mqtt_broker);
  this->config->setMqttTopic(mqtt_topic);
  this->config->setMqttClientId(mqtt_client_id);
  this->config->setMqttPort(mqtt_port);
  this->config->saveConfig();
  request->send(200, "text/plain", "Updated config... please reboot");
}

void ScaleWebServer::onReboot(AsyncWebServerRequest *request) {
  Serial.println("in /reboot");
  request->send(200, "text/plain", "Rebooting in 2 seconds...");
  this->config->setRebootRequest(true);
}

void ScaleWebServer::onTare(AsyncWebServerRequest *request) {
  Serial.println("in /tare");
  this->config->setTareRequest(true);
  Serial.println("Tare requested");
  request->send(200, "text/plain", "Zero weight set");
}

void ScaleWebServer::receiveCalibrateData(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  Serial.println("in /calibrate data handler");
  Serial.printf("len = %i\n", len);
  Serial.printf("index = %i\n", index);
  Serial.printf("total = %i\n", total);
  for (int i = 0; i < len; i++) {
    Serial.print((char)data[i]);
  }
  Serial.println("");
  if (total > JSON_DATA_LEN) {
    request->send(413, "text/plain", "ERROR: Request too large. Increase JSON_DATA_LEN");
  } else {
    strncpy(((char*)this->tmp_json)+index, (char*)data, len);
  }
}

void ScaleWebServer::processCalibrateData(AsyncWebServerRequest *request) {
  Serial.println("in /calibrate request handler");
  Serial.println(this->tmp_json);
  StaticJsonDocument<JSON_DATA_LEN> jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, this->tmp_json, strlen(this->tmp_json));
  if (error) {
    Serial.println(error.c_str());
    request->send(400, "text/plain", "ERROR: Invalid Request");
    return;
  }
  const uint16_t weight = atoi(jsonDoc["calibration"]);
  this->config->setRequestCalibrate(weight);
  request->send(200, "text/plain", "Scale calibrated");
}

void ScaleWebServer::setupRoutes() {
  this->server->on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
    this->onGetRoot(request);
  });
  this->server->on("/updateconfig", HTTP_POST, [this] (AsyncWebServerRequest *request) {
   this->processUpdateConfigData(request); 
  }, NULL, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    this->receiveUpdateConfigData(request, data, len, index, total);
  });
  this->server->on("/reboot", HTTP_POST, [this](AsyncWebServerRequest *request) {
    this->onReboot(request);
  });
  this->server->on("/tare", HTTP_POST, [this](AsyncWebServerRequest *request) {
    this->onTare(request);
  });
  this->server->on("/calibrate", HTTP_POST, [this] (AsyncWebServerRequest *request) {
   this->processCalibrateData(request); 
  }, NULL, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    this->receiveCalibrateData(request, data, len, index, total);
  });
}

String ScaleWebServer::indexProcessor(const String &var) {
  if (config->isConfigValid()) {
    if (var == "WIFI_SSID") {
      return String(config->getWifiSsid());
    }
    if (var == "WIFI_PASSWORD") {
      return String(config->getWifiPassword());
    }
    if (var == "MQTT_BROKER") {
      return String(config->getMqttBroker());
    }
    if (var == "MQTT_TOPIC") {
      return String(config->getMqttTopic());
    }
    if (var == "MQTT_CLIENT_ID") {
      return String(config->getMqttClientId());
    }
    if (var == "MQTT_PORT") {
      return String(config->getMqttPort());
    }
  }
  return String();
}

void ScaleWebServer::begin() {
  this->server->begin();
}
