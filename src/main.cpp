#include "main.h"

AsyncWebServer server(httpPort);
SHT3X sht3x;
BMP280 bmp;


// sensor data
static double temperature = 0.0;
static double humidity = 0.0;
static String datetime = "";
SensorData ringBuffer[ringBufferSize];
static int32_t bufferHead = 0; // 次に書き込む位置
static int32_t bufferCount = 0; // 現在のデータ数

String getCurrentDatetimeString()
{
  struct tm time_info;
  if(getLocalTime(&time_info))
  {
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &time_info);
    return String(buffer);
  }
  M5_LOGE("Failed to get local time");
  return "N/A";
}

void addSensorData(String date, double temp, double humi)
{
  ringBuffer[bufferHead].datetime = date;
  ringBuffer[bufferHead].temperature = temp;
  ringBuffer[bufferHead].humidity = humi;
  bufferHead = (bufferHead + 1) % ringBufferSize;
  if (bufferCount < ringBufferSize)
  {
    ++bufferCount;
  }
}

String createJsonData(void)
{
  DynamicJsonDocument doc(1024);

  doc["datetime"] = datetime;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;

  String responseData;
  serializeJson(doc, responseData);

  M5_LOGI("reponse: %s", responseData.c_str());

  return responseData;
}

String createJsonDataArray(void)
{
  DynamicJsonDocument doc(1024);
  JsonArray dataArray = doc.createNestedArray("data");

  for (int32_t i = 0; i < bufferCount; ++i)
  {
    int32_t index = (bufferHead - bufferCount + i + ringBufferSize) % ringBufferSize;
    JsonObject data = dataArray.createNestedObject();
    data["datetime"] = ringBuffer[index].datetime;
    data["temperature"] = ringBuffer[index].temperature;
    data["humidity"] = ringBuffer[index].humidity;
  }

  String responseData;
  serializeJson(doc, responseData);

  M5_LOGI("response: %s", responseData.c_str());

  return responseData;
}

void setup()
{
  auto cfg = M5.config();
  // M5デバイスの初期化
  M5.begin(cfg);

  // シリアル出力のログレベルを VERBOSEに設定
  M5.Log.setLogLevel(m5::log_target_serial, ESP_LOG_VERBOSE);
  // ログカラー表示を有効化
  M5.Log.setEnableColor(m5::log_target_serial, true);

  M5.Display.setTextSize(3);         // テキストサイズを変更
  M5.Display.print("Hello World!!"); // 画面にHello World!!と1行表示
  M5.delay(3000);
  M5_LOGI("Hello M5Stack");

  // Wi-Fi
  WiFi.config(ip, gateway, subnet, dns);
  WiFi.begin(wifiSsid, wifiKey);
  while(WiFi.status() != WL_CONNECTED)
  {
    M5.Log.print(".");
    M5.delay(1000); // wait
  }
  M5.delay(2000);
  M5_LOGI("IP ADDR:%s", WiFi.localIP().toString().c_str());
  M5_LOGI("WIFI STATE:%d", WiFi.status());
  M5_LOGI("Wi-Fi RSSI: %d dBm", WiFi.RSSI());

  // NTP
  configTime(gmtOffsetSec, daylightOffsetSec, ntpServer1st, ntpServer2nd);
  struct tm time_info;
  if (!getLocalTime(&time_info))
  {
    M5_LOGE("NTP init Err!");
    delay(3000);
  }

  // sensor
  if(!sht3x.begin(&Wire, SHT3X_I2C_ADDR, 2, 1, 400000U))
  {
    M5_LOGE("Couldn't find SHT3X");
    M5.Display.print("SHT3X Err!!");
    while (1) delay(1);
  }
  else
  {
    M5_LOGI("Find SHT3X");
  }

  if(!bmp.begin(&Wire, BMP280_I2C_ADDR, 2, 1, 400000U))
  {
    M5_LOGE("Couldn't find BMP280");
    M5.Display.print("BMP280 Err!!");
    while (1) delay(1);
  }
  else
  {
    M5_LOGI("Find BMP280");
  }

  // Default settings from datasheet.
  bmp.setSampling(
    BMP280::MODE_NORMAL,     /* Operating Mode.       */
    BMP280::SAMPLING_X2,     /* Temp. oversampling    */
    BMP280::SAMPLING_X16,    /* Pressure oversampling */
    BMP280::FILTER_X16,      /* Filtering.            */
    BMP280::STANDBY_MS_500
  );

  // WebServer
  server.on("/hello-world", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", "Hello world from AtomS3");
  });

  server.on("/get-sensor-latest-value", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "application/json", createJsonData());
  });

  server.on("/get-sensor-values", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "application/json", createJsonDataArray());
  });
  server.begin();
}

void loop()
{
  M5.update();
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(0, 0);

  datetime = getCurrentDatetimeString();

  if(sht3x.update())
  {
    temperature = sht3x.cTemp;
    humidity = sht3x.humidity;
    temperature = round(temperature * 100) / 100.0;
    humidity = round(humidity * 100) / 100.0;

    addSensorData(datetime, temperature, humidity);

    M5.Display.setTextSize(1);
    M5.Display.printf("%s\n", datetime.c_str());
    M5.Display.setTextSize(2);
    M5.Display.printf("temp.\n");
    M5.Display.printf("  %.2f\n", temperature);
    M5.Display.printf("humi.\n");
    M5.Display.printf("  %.2f\n", humidity);

    M5_LOGI("-----SHT3X-----");
    M5_LOGI("%s", datetime.c_str());
    M5_LOGI("Temperature: %.2f degrees C", temperature);
    M5_LOGI("Humidity: %.2f % rH", humidity);
    M5_LOGI("---------------");
  }

  if (bmp.update())
  {
    M5_LOGI("-----BMP280-----");
    M5_LOGI("Temperature: %.2f degrees C", bmp.cTemp);
    M5_LOGI("Pressure: %.2f Pa", bmp.pressure);
    M5_LOGI("Approx altitude: %.2f m", bmp.altitude);
    M5_LOGI("----------------");
  }

  M5.delay(loopTimeMs);
}

