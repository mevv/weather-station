#include <Wire.h>

#include <SimpleDHT.h>
#include <SFE_BMP180.h>
#include <BH1750.h>

//#include <ArduinoJson.h>
#include <Http.h>

#include "settings.h"

SimpleDHT22 dht22;
SFE_BMP180 bmp180;
BH1750 bh1750;

HTTP http(9600, RX_PIN, TX_PIN, RST_PIN, true);

struct Data
{
  byte error = 0;
  int time = 0;
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
  int err = SimpleDHTErrSuccess;

  if ((err = dht22.read(DHT22_PIN, &result.temperature, &result.humidity, NULL)) != SimpleDHTErrSuccess)
    result.error = err;

  char status;
  double T, P;

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
  return "{\"error\": " + String(data.error) + ", \"id\": " + String(ID) + ", \"timestamp\": " + String(data.time) + ", \"temperature\": " + String(data.temperature) + ", \"humidity\": " + String(data.humidity) + ", \"pressure\": " + String(data.pressure) + ", \"luminosity\": " + String(data.luminosity) + ", \"rain\": " + String(data.rain) + "}";
}

String dataToString(const Data& data)
{
  return String(data.error) + "," + String(ID) + "," + String(data.time) + ", " + String(data.temperature) + "," + String(data.humidity) + "," + String(data.pressure) + "," + String(data.luminosity) + "," + String(data.rain);
}

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  bh1750.begin();

  if (!bmp180.begin())
    return;

  Serial.println(http.configureBearer("internet"));
  
  analogReference(EXTERNAL);
  
  Serial.println("Started!");
}

void loop()
{
  Data data;

  char response[128];
  String body;

  data = collectData();

  Serial.println("-----");
  Serial.println(data.error);
  Serial.println(data.temperature);
  Serial.println(data.humidity);
  Serial.println(data.pressure);
  Serial.println(data.luminosity);
  Serial.println(data.rain);
  Serial.println("-----");

  body = dataToString(data);

//  body = dataToJsonString(data);
  
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
  
  Serial.println(body);
  Serial.println(body.c_str());
  
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
