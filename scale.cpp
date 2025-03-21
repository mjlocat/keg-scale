#include "scale.h"

Scale::Scale(Config *config) {
    this->scale = new HX711();
    this->config = config;
}

Scale::~Scale() {
  delete this->scale;
}

void Scale::begin() {
  this->scale->begin(SCALE_DATA_PIN, SCALE_CLOCK_PIN);
  this->scale->set_scale(this->config->getScaleScale());
  this->scale->set_offset(this->config->getScaleOffset());
  this->scale->power_down();
}

void Scale::tare() {
  Serial.println("In Scale.tare()");
  this->scale->power_up();
  if (this->scale->wait_ready_timeout(1000, 0)) {
    this->scale->tare(10);
    this->scale->power_down();
    Serial.println("Scale tared, storing offset to config");
    this->config->setScaleOffset(this->scale->get_offset());
    Serial.println("Offset stored, saving config to EEPROM");
    this->config->saveConfig();
    Serial.println("Config saved to EEPROM");
    this->config->setTareRequest(false);
  }
}

void Scale::calibrate(uint16_t weight) {
  this->scale->power_up();
  if (this->scale->wait_ready_timeout(1000, 0)) {
    this->scale->calibrate_scale(weight);
    this->scale->power_down();
    this->config->setScaleScale(this->scale->get_scale());
    this->config->saveConfig();
    this->config->setRequestCalibrate(0);
  }
}

float Scale::getWeight() {
  float weight = 0;
  this->scale->power_up();
  if (this->scale->wait_ready_timeout(1000, 0)) {
    weight = this->scale->get_units(10);
    this->scale->power_down();
  }
  return weight;
}

void Scale::debug() {
  Serial.printf("Gain: %i\n", this->scale->get_gain());
  Serial.printf("Tare: %f\n", this->scale->get_tare());
  Serial.printf("Scale: %f\n", this->scale->get_scale());
  Serial.printf("Offset: %i\n", this->scale->get_offset());
}
