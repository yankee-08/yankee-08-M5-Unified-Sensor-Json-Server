#ifndef HEADER_MAIN_H
#define HEADER_MAIN_H

#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <math.h>
#include <M5Unified.h>
#include <M5UnitENV.h>
#include <WiFi.h>

// network
const char* wifiSsid = "{{your_ssid}}";
const char* wifiKey = "{{your_key}}";
IPAddress ip(xx, xx, xx, xx);
IPAddress dns(xx, xx, xx, xx);
IPAddress gateway(xx, xx, xx, xx);
IPAddress subnet(xx, xx, xx, xx);
const uint32_t httpPort = xx;

// NTP
const char *ntpServer1st = "ntp.nict.jp";
const char *ntpServer2nd = "time.google.com";
const long gmtOffsetSec = 9 * 3600; // 時差（秒換算）
const int daylightOffsetSec = 0;    // 夏時間

// ループ間隔
const uint32_t loopTimeMs = 60 * 10 * 1000;

// リングバッファ
const uint32_t ringBufferSize = 10;
struct SensorData
{
    String datetime;
    double temperature;
    double humidity;
};

#endif // HEADER_MAIN_H
