#include <Wire.h>
#include <SoftwareSerial.h>

#include <SimpleDHT.h>
#include <SFE_BMP180.h>
#include <BH1750.h>
#include <Http.h>
#include <Sim800.h>

#include "error.h"
#include "settings.h"
#include "time.h"
#include "ram_cache.h"

#ifdef MEMORY
#include <MemoryFree.h>
#endif

SimpleDHT dht22;
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

RamCache<Data> cache;

Data collectData()
{
    Data result;

    byte status;
    byte err = 0;
    double T = 0.0;
    double P = 0.0;

    if (!(result.ts = gsmntp.getTimestamp()))
        result.error = TIME_SYNC_ERROR;

    if ((err = dht22.read(DHT22_PIN, &result.temperature, &result.humidity, NULL)) != SimpleDHTErrSuccess)
        if (!result.error)
            result.error = err;

    status = bmp180.startTemperature();
    if (!status)
        if (!result.error)
            result.error = bmp180.getError();
    delay(status);
    status = bmp180.getTemperature(T);

    status = bmp180.startPressure(PRESSURE_RESOLUTION);
    if (!status)
        if (!result.error)
            result.error = bmp180.getError();
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

#ifdef MEMORY
void printFreeMem()
{
    Serial.println(String("Memory:" + String(freeMemory())));
}
#endif

#ifdef VERBOSE
void printData(const Data& data)
{
    Serial.println("-----");
    Serial.println(data.error);
    Serial.println(data.ts);
    Serial.println(data.temperature);
    Serial.println(data.humidity);
    Serial.println(data.pressure);
    Serial.println(data.luminosity);
    Serial.println(data.rain);
    Serial.println("-----");
}
#endif

void setup()
{
    Serial.begin(9600);

    if (!bmp180.begin())
        return;

    if (!bh1750.begin())
        return;

    #ifdef VERBOSE
    Serial.println(http.configureBearer(APN));
    #else
    http.configureBearer(APN);
    #endif

    analogReference(EXTERNAL);

    #ifdef MEMORY
    printFreeMem();
    #endif
}

void loop()
{
    Data data;
    char response[8];

    #ifdef MEMORY
    printFreeMem();
    #endif

    data = collectData();
    cache.push(data);

    #ifdef MEMORY
    printFreeMem();
    #endif

    if (http.connect())
        return;

    bool flag = true;
    while (flag)
    {
        flag = false;
        data = cache.pop();

        #ifdef VERBOSE
        Serial.println("Cache size: ");
        Serial.println(cache.size());
        printData(data);
        #endif

        #ifdef MEMORY
        printFreeMem();
        #endif

        if (http.post(URL, dataToJsonString(data).c_str(), response) == SUCCESS)
        {
            #ifdef VERBOSE
            Serial.print("Response: ");
            Serial.println(response);
            #endif
            if (!cache.empty())
                flag = true;
        }
        else
        {
            cache.push(data);
        }
    }

    if (http.disconnect())
        return;

    delay(DELAY);
}
