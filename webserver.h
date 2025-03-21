#ifndef __SCALEWEBSERVER_H__
#define __SCALEWEBSERVER_H__
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "config.h"
#include "scale.h"
#define JSON_DATA_LEN 200

class ScaleWebServer {
  private:
    Config *config;
    AsyncWebServer *server;
    char tmp_json[JSON_DATA_LEN];
    void setupRoutes();
    String indexProcessor(const String &var);
    void onGetRoot(AsyncWebServerRequest *request);
    void receiveUpdateConfigData(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
    void processUpdateConfigData(AsyncWebServerRequest *request);
    void onReboot(AsyncWebServerRequest *request);
    void onTare(AsyncWebServerRequest *request);
    void receiveCalibrateData(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
    void processCalibrateData(AsyncWebServerRequest *request);
  public:
    ScaleWebServer(Config *config);
    ~ScaleWebServer();
    void begin();
};

#endif /* __SCALEWEBSERVER_H__ */
