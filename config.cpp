#include <Arduino.h>
#include <ESP_EEPROM.h>
#include "config.h"

Config::Config() {
  size_t flash_size = sizeof(this->magic) + WIFI_SSID_LEN + WIFI_PASSWORD_LEN +
    MQTT_BROKER_LEN + MQTT_TOPIC_LEN + MQTT_CLIENT_ID_LEN + sizeof(this->mqtt_port) +
    sizeof(this->scale_scale) + sizeof(this->scale_offset);
  this->reboot = false;
  this->tare = false;
  this->calibrate_weight = 0;
  EEPROM.begin(flash_size);
}

Config::~Config() {
  
}

bool Config::isConfigValid() {
  return (this->magic == MAGIC);
}

bool Config::eraseConfig() {
  Serial.printf("Wiping all EEPROM Data\n");
  return EEPROM.wipe();
}

bool Config::loadConfig() {
  int addr = 0;
  EEPROM.get(addr, this->magic);
  Serial.printf("magic: 0x%X\n", this->magic);
  if (this->magic == MAGIC) {
    addr += sizeof(this->magic);
    EEPROM.get(addr, this->wifi_ssid);
    Serial.printf("wifi_ssid: %s\n", this->wifi_ssid);
    addr += sizeof(this->wifi_ssid);
    EEPROM.get(addr, this->wifi_password);
    Serial.printf("wifi_password: %s\n", this->wifi_password);
    addr += sizeof(addr, this->wifi_password);
    EEPROM.get(addr, this->mqtt_broker);
    Serial.printf("mqtt_broker: %s\n", this->mqtt_broker);
    addr += sizeof(this->mqtt_broker);
    EEPROM.get(addr, this->mqtt_topic);
    Serial.printf("mqtt_topic: %s\n", this->mqtt_topic);
    addr += sizeof(this->mqtt_topic);
    EEPROM.get(addr, this->mqtt_client_id);
    Serial.printf("mqtt_client_id: %s\n", this->mqtt_client_id);
    addr += sizeof(this->mqtt_client_id);
    EEPROM.get(addr, this->mqtt_port);
    Serial.printf("mqtt_port: %i\n", this->mqtt_port);
    addr += sizeof(this->mqtt_port);
    EEPROM.get(addr, this->scale_scale);
    Serial.printf("scale_scale: %f\n", this->scale_scale);
    addr += sizeof(this->scale_scale);
    EEPROM.get(addr, this->scale_offset);
    Serial.printf("scale_offset: %i\n", this->scale_offset);
    addr += sizeof(this->scale_offset);
  
    return true;
  }
  return false;
}

void Config::saveConfig() {
  int addr = 0;
  Serial.printf("Saving magic to Address 0x%X\n", addr);
  Serial.printf("0x%X\n", EEPROM.put(addr, MAGIC));
  addr += sizeof(MAGIC);
  Serial.printf("Saving wifi_ssid to Address 0x%X\n", addr);
  Serial.printf("%s\n", EEPROM.put(addr, this->wifi_ssid));
  addr += sizeof(this->wifi_ssid);
  Serial.printf("Saving wifi_password to Address 0x%X\n", addr);
  Serial.printf("%s\n", EEPROM.put(addr, this->wifi_password));
  addr += sizeof(addr, this->wifi_password);
  Serial.printf("Saving mqtt_broker to Address 0x%X\n", addr);
  Serial.printf("%s\n", EEPROM.put(addr, this->mqtt_broker));
  addr += sizeof(this->mqtt_broker);
  Serial.printf("Saving mqtt_topic to Address 0x%X\n", addr);
  Serial.printf("%s\n", EEPROM.put(addr, this->mqtt_topic));
  addr += sizeof(this->mqtt_topic);
  Serial.printf("Saving mqtt_client_id to Address 0x%X\n", addr);
  Serial.printf("%s\n", EEPROM.put(addr, this->mqtt_client_id));
  addr += sizeof(this->mqtt_client_id);
  Serial.printf("Saving mqtt_port to Address 0x%X\n", addr);
  Serial.printf("%i\n", EEPROM.put(addr, this->mqtt_port));
  addr += sizeof(this->mqtt_port);
  Serial.printf("Saving scale_scale to Address 0x%X\n", addr);
  Serial.printf("%f\n", EEPROM.put(addr, this->scale_scale));
  addr += sizeof(this->scale_scale);
  Serial.printf("Saving scale_offset to Address 0x%X\n", addr);
  Serial.printf("%i\n", EEPROM.put(addr, this->scale_offset));
  addr += sizeof(this->scale_offset);
  Serial.printf("Committing EEPROM\n");
  EEPROM.commit();
}

char *Config::getWifiSsid() {
  return this->wifi_ssid;
}

char *Config::getWifiPassword() {
  return this->wifi_password;
}

char *Config::getMqttBroker() {
  return this->mqtt_broker;
}

char *Config::getMqttTopic() {
  return this->mqtt_topic;
}

char *Config::getMqttClientId() {
  return this->mqtt_client_id;
}

uint16_t Config::getMqttPort() {
  return this->mqtt_port;
}

float Config::getScaleScale() {
  return this->scale_scale;
}

int32_t Config::getScaleOffset() {
  return this->scale_offset;
}

bool Config::rebootRequest() {
  return this->reboot;
}

bool Config::tareRequest() {
  return this->tare;
}

uint16_t Config::calibrateRequest() {
  return this->calibrate_weight;
}


void Config::setWifiSsid(const char *wifi_ssid) {
  strncpy(this->wifi_ssid, wifi_ssid, WIFI_SSID_LEN);
}

void Config::setWifiPassword(const char *wifi_password) {
  strncpy(this->wifi_password, wifi_password, WIFI_PASSWORD_LEN);
}

void Config::setMqttBroker(const char *mqtt_broker) {
  strncpy(this->mqtt_broker, mqtt_broker, MQTT_BROKER_LEN);
}

void Config::setMqttTopic(const char *mqtt_topic) {
  strncpy(this->mqtt_topic, mqtt_topic, MQTT_TOPIC_LEN);
}

void Config::setMqttClientId(const char *mqtt_client_id) {
  strncpy(this->mqtt_client_id, mqtt_client_id, MQTT_CLIENT_ID_LEN);
}

void Config::setMqttPort(const uint16_t mqtt_port) {
  this->mqtt_port = mqtt_port;
}

void Config::setScaleScale(float scale_scale) {
  this->scale_scale = scale_scale;
}

void Config::setScaleOffset(int32_t scale_offset) {
  this->scale_offset = scale_offset;
}

void Config::setRebootRequest(const bool reboot) {
  this->reboot = reboot;
}

void Config::setTareRequest(const bool tare) {
  this->tare = tare;
}

void Config::setRequestCalibrate(const uint16_t weight) {
  this->calibrate_weight = weight;
}
