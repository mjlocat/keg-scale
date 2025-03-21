/* Notes:
 *  10 lb kettle bell is actually 10.4 lbs or 166.4 oz
 *  Empty Keg is 9.4 pounds or 150.4 oz
 */


#include "config.h"
#include "scale.h"
#include "wifi.h"
#include "mqtt.h"
#include "webserver.h"

/* Globals */
Config *config = new Config();
Scale *scale = new Scale(config);
ScaleWifi *wifi = new ScaleWifi(config);
ScaleMqtt *mqtt = new ScaleMqtt(config);
ScaleWebServer *server = new ScaleWebServer(config);
bool runLoop = false;
unsigned long previousMillis = 0;
const unsigned long interval = 5000; // get weight every 5 seconds

void setup() {
  /* Set up serial port for debugging */
  Serial.begin(57600);
  delay(100);
  Serial.println("Starting up...");

  /* Uncomment this if you need to erase the config and start over */
//  Serial.println("Erasing configuration");
//  config->eraseConfig();
//  return;
  
  /* Get existing config values */
  Serial.println("Loading configuration");
  config->loadConfig();


  if (config->isConfigValid()) {
    Serial.println("Config is valid, connecting to wifi");
    /* Set up the WiFi connection */
    if (wifi->connectWifi()) {
       /* Connect to MQTT broker */
      Serial.println("Wifi connected, connecting to MQTT broker");
      if (mqtt->connect()) {
        /* Set up the HX711 sensor */
        Serial.println("Connected to MQTT broker");
        scale->begin();
        runLoop = true;
      }
    } 
  } else {
    Serial.println("Configuration is not valid, starting AP Config mode");
    wifi->setupAp();
  }
  Serial.println("Done with setup, starting webserver");
  server->begin();
  Serial.println("Webserver started");
}

void loop() {
  unsigned long currentMillis = millis();
  uint16_t weight;
  if (config->rebootRequest()) {
    delay(2000);
    ESP.reset();
  }
  if (config->tareRequest()) {
    Serial.println("Web requested tare being processed");
    scale->tare();
  }
  if ((weight = config->calibrateRequest()) != 0) {
    Serial.printf("Web requested calibration at %i\n", weight);
    scale->calibrate(weight);
  }
  if (runLoop) {
    if (!wifi->isConnected()) {
      if (!wifi->connectWifi()) {
        runLoop = false;
        return;
      }
    }
    if (!mqtt->isConnected()) {
      if (!mqtt->connect()) {
        runLoop = false;
        return;
      }
    }
  
    int bytes = Serial.available();
    if (bytes > 0) {
      Serial.printf("Received %i bytes\n", bytes);
      for (int i = 0; i < bytes; i++) {
        int cmd = Serial.read();
        switch (cmd) {
          /* T, t */
          case 0x54:
          case 0x74:
            Serial.printf("Tare\n");
            scale->tare();
            break;
          case 0x43:
          case 0x63:
            Serial.printf("Calibrate\n");
            scale->calibrate(160);
            break;
          case 0x44:
          case 0x64:
            scale->debug();
            break;
          case -1:
            Serial.printf("No data available\n");
            break;
          case 0x0a:
          case 0x0d:
            break;
          default:
            Serial.printf("Unknown command: %c\n", (char) cmd);
        }
      }
    }

    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      /* Read the weight from the HX711 sensor */
      float weight = scale->getWeight();
      Serial.printf("Weight = %f\n", weight);
  
      if (weight != 0) {
        /* Publish the weight to the MQTT topic */
        mqtt->publishWeight(weight);
      }
    }
  }
}
