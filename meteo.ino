#include <Wire.h>

#include <SimpleDHT.h>
#include <SFE_BMP180.h>
#include <BH1750.h>

//#include <ArduinoJson.h>
#include <Http.h>
#include <Sim800.h>

#include "error.h"
#include "settings.h"
#include "time.h"

SimpleDHT22 dht22;
SFE_BMP180 bmp180;
BH1750 bh1750;

HTTP http(9600, RX_PIN, TX_PIN, RST_PIN, true);
GSMNTP gsmntp(9600, RX_PIN, TX_PIN, RST_PIN, true);

struct Data
{
  byte error = 0;
  time_t ts = 0;
  byte temperature = 0;
  byte humidity = 0;
  word pressure = 0;
  word luminosity = 0;
  word rain = 0;
};

Data collectData()
{
  Data result;
  
  byte temperature = 0;
  byte humidity = 0;
  char status;
  double T, P;
  int err = SimpleDHTErrSuccess;
  time_t timestamp = 0;
  
  if (!(timestamp = gsmntp.getTimestamp()))
    result.error = TIME_SYNC_ERROR;
  
  result.ts = timestamp;
  
  if ((err = dht22.read(DHT22_PIN, &result.temperature, &result.humidity, NULL)) != SimpleDHTErrSuccess)
    result.error = err;

  status = bmp180.startTemperature();
  if (!status)
    result.error = 1;
  delay(status);
  status = bmp180.getTemperature(T);

  status = bmp180.startPressure(PRESSURE_RESOLUTION);
  if (!status)
    result.error = 1;
  delay(status);
  status = bmp180.getPressure(P, T);

  result.pressure = P;

  result.luminosity = bh1750.readLightLevel();

  result.rain = analogRead(RAIN_PIN);

  return result;
}

String dataToJsonString(const Data& data)
{
  return "{\"e\": " + String(data.error) + ", \"id\": " + String(ID) + ", \"ts\": " + String(data.ts) + ", \"t\": " + String(data.temperature) + ", \"h\": " + String(data.humidity) + ", \"p\": " + String(data.pressure) + ", \"l\": " + String(data.luminosity) + ", \"r\": " + String(data.rain) + "}";
}

String dataToString(const Data& data)
{
  return String(data.error) + "," + String(ID) + "," + String(data.ts) + ", " + String(data.temperature) + "," + String(data.humidity) + "," + String(data.pressure) + "," + String(data.luminosity) + "," + String(data.rain);
}

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  bh1750.begin();

  if (!bmp180.begin())
    return;

  Serial.println(http.configureBearer(APN));
  
  analogReference(EXTERNAL);
  
  Serial.println("Started!");
}

void loop()
{
  Data data;

  char response[8];
  String body;

  data = collectData();

  Serial.println("-----");
  Serial.println(data.error);
  Serial.println(data.ts);
  Serial.println(data.temperature);
  Serial.println(data.humidity);
  Serial.println(data.pressure);
  Serial.println(data.luminosity);
  Serial.println(data.rain);
  Serial.println("-----");
  
//  body = dataToString(data);

  body = dataToJsonString(data);

//  StaticJsonBuffer<100> jsonBuffer;
//  JsonObject& json = jsonBuffer.createObject();
//  
//  json["error"] = data.error;
//  json["id"] = 1;
//  json["time"] = data.time;
//  json["temperature"] = data.temperature;
//  json["humidity"] = data.humidity;
//  json["pressure"] = data.pressure;
//  json["luminosity"] = data.luminosity;
//  json["rain"] = data.rain;
//  
//  json.printTo(body);
  
//  Serial.println(body);
//  Serial.println(body.c_str());
  
  http.connect();
  
  Result result = http.post(URL, body.c_str(), response);

  if (result == SUCCESS)
  {
    Serial.print("Response: ");
    Serial.println(response);
  }
  else
  {
    Serial.print("Error:");
    Serial.println(result);
  }
  
  http.disconnect();

  delay(DELAY);
}
