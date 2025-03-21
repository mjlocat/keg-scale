#ifndef __CONFIG_H__
#define __CONFIG_H__
#define MAGIC 0xDEADBEEF
#define WIFI_SSID_LEN 32
#define WIFI_PASSWORD_LEN 64
#define MQTT_BROKER_LEN 15
#define MQTT_TOPIC_LEN 30
#define MQTT_CLIENT_ID_LEN 20

class Config {
  private:
    uint32_t magic;
    char wifi_ssid[WIFI_SSID_LEN];
    char wifi_password[WIFI_PASSWORD_LEN];
    char mqtt_broker[MQTT_BROKER_LEN];
    char mqtt_topic[MQTT_TOPIC_LEN];
    char mqtt_client_id[MQTT_CLIENT_ID_LEN];
    uint16_t mqtt_port;
    float scale_scale;
    int32_t scale_offset;
    bool reboot;
    bool tare;
    uint16_t calibrate_weight;
  public:
    // default constructor
    Config();
    // destructor
    ~Config();
    // returns true if the config is valid
    bool isConfigValid();
    // erase the configuration
    bool eraseConfig();
    // loads config from eeprom, returns true if config is valid
    bool loadConfig();
    // saves config to eeprom
    void saveConfig();

    // getters
    char *getWifiSsid();
    char *getWifiPassword();
    char *getMqttBroker();
    char *getMqttTopic();
    char *getMqttClientId();
    uint16_t getMqttPort();
    float getScaleScale();
    int32_t getScaleOffset();
    bool rebootRequest();
    bool tareRequest();
    uint16_t calibrateRequest();

    // setters
    void setWifiSsid(const char *wifi_ssid);
    void setWifiPassword(const char *wifi_password);
    void setMqttBroker(const char *mqtt_broker);
    void setMqttTopic(const char *mqtt_topic);
    void setMqttClientId(const char *mqtt_client_id);
    void setMqttPort(const uint16_t mqtt_port);
    void setScaleScale(float scale_scale);
    void setScaleOffset(int32_t scale_offset);
    void setRebootRequest(const bool reboot);
    void setTareRequest(const bool tare);
    void setRequestCalibrate(const uint16_t weight);
};

#endif /* __CONFIG_H__ */
