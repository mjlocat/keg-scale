#ifndef __SCALEWIFI_H__
#define __SCALEWIFI_H__
#include "config.h"

class ScaleWifi {
  private:
    Config *config;
  public:
    ScaleWifi(Config *config);
    ~ScaleWifi();
    bool connectWifi();
    void setupAp();
    bool isConnected();
};

#endif /* __SCALEWIFI_H__ */
