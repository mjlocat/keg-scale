#ifndef __SCALEMQTT_H__
#define __SCALEMQTT_H__
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include "config.h"

class ScaleMqtt {
  private:
    Config *config;
    WiFiClient esp_client;
    PubSubClient *mqtt_client;
  public:
    ScaleMqtt(Config *config);
    ~ScaleMqtt();
    bool connect();
    bool isConnected();
    void publishWeight(float weight);
};

#endif /* __SCALEMQTT_H__ */
