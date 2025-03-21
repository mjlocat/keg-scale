#ifndef __SCALE_H__
#define __SCALE_H__

#include "HX711.h"
#include "config.h"

#define SCALE_DATA_PIN 4
#define SCALE_CLOCK_PIN 5

class Scale {
  private:
    Config *config;
    HX711 *scale;
  public:
    Scale(Config *config);
    ~Scale();
    void begin();
    void tare();
    void calibrate(uint16_t weight);
    float getWeight();
    void debug();
};

#endif __SCALE_H__
