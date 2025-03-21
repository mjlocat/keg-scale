#include <Arduino.h>
#include <PubSubClient.h>
#include "mqtt.h"

ScaleMqtt::ScaleMqtt(Config *config) {
  this->config = config;
  this->mqtt_client = new PubSubClient(esp_client);
}

ScaleMqtt::~ScaleMqtt() {
  
}

bool ScaleMqtt::connect() {
  int i = 0;
  bool connected = this->mqtt_client->connected();
  this->mqtt_client->setServer(this->config->getMqttBroker(), this->config->getMqttPort());
  while (!connected && i < 10) {
    Serial.printf("Client %s connecting to %s:%i...\n", this->config->getMqttClientId(), this->config->getMqttBroker(), this->config->getMqttPort());
    if (this->mqtt_client->connect(this->config->getMqttClientId())) {
      Serial.println("Connected to broker");
      return true;
    } else {
      Serial.printf("Failed to connect to broker: ");
      switch (this->mqtt_client->state()) {
        case MQTT_CONNECTION_TIMEOUT:
          Serial.println("Connection Timeout");
          break;
        case MQTT_CONNECTION_LOST:
          Serial.println("Connection Lost");
          break;
        case MQTT_CONNECT_FAILED:
          Serial.println("Connect Failed");
          break;
        case MQTT_DISCONNECTED:
          Serial.println("Disconnected");
          break;
        case MQTT_CONNECT_BAD_PROTOCOL:
          Serial.println("Bad Protocol");
          return false;
        case MQTT_CONNECT_BAD_CLIENT_ID:
          Serial.println("Bad Client ID");
          return false;
        case MQTT_CONNECT_UNAVAILABLE:
          Serial.println("Connect Unavailable");
          return false;
        case MQTT_CONNECT_BAD_CREDENTIALS:
          Serial.println("Bad Credentials");
          return false;
        case MQTT_CONNECT_UNAUTHORIZED:
          Serial.println("Unauthorized");
          return false;
      }
      i++;
      delay(2000);
    }
  }
  Serial.println("Too many failed connection attempts to MQTT broker");
  return false;
}

bool ScaleMqtt::isConnected() {
  return (this->mqtt_client->connected());
}

void ScaleMqtt::publishWeight(float weight) {
  char str_weight[15];
  snprintf(str_weight, 15, "%f", weight);
  this->mqtt_client->publish(this->config->getMqttTopic(), str_weight);
}
