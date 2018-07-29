#ifndef _TIME_H_
#define _TIME_H_

#include <TimeLib.h>

#define OK "OK\r\n"

#define CPRS_TYPE "AT+SAPBR=3,1, \"Contype\",\"GPRS\"\r\n"
#define SET_APN "AT+SAPBR=3,1, \"APN\",\"internet\"\r\n"
#define CONTEXT "AT+SAPBR=1,1\r\n"
#define NTP_PROFILE "AT+CNTPCID=1\r\n"
#define NTP_SERVER "AT+CNTP=\"time.google.com\",0\r\n"
#define NTP "AT+CNTP\r\n"
#define TIME "AT+CCLK?\r\n"

#define BUF 40

class GSMNTP : public SIM800
{
public:
    GSMNTP(unsigned int baudRate, unsigned int rxPin, unsigned int txPin, unsigned int rstPin, bool debug = TRUE) :
        SIM800(baudRate, rxPin, txPin, rstPin, debug)
    {
        setSyncInterval(TIME_SYNC_INTERVAL);
    }

    time_t getTimestamp()
    {
        #ifdef VERBOSE
        Serial.println("Time status: " + String(timeStatus()));
        #endif

        if (timeStatus() == timeNotSet || timeStatus() == timeNeedsSync)
            if (!syncTime())
                return 0;

        return now();
    }

private:
    time_t getTime()
    {
        char buf[BUF];

        sendCmdAndWaitForResp(CPRS_TYPE, OK, 2000);
        sendCmdAndWaitForResp(SET_APN, OK, 2000);
        sendCmdAndWaitForResp(CONTEXT, OK, 2000);
        sendCmdAndWaitForResp(NTP_PROFILE, OK, 2000);
        sendCmdAndWaitForResp(NTP_SERVER, OK, 2000);

        if (!sendCmdAndWaitForResp(NTP, OK, 2000))
            return 0;

        sendCmd(TIME);

        time_t dt = millis();

        cleanBuffer(buf, sizeof(buf));
        readBuffer(buf, sizeof(buf), 2000);

        tmElements_t tm;
        if (!parseTimeResponse(tm, buf))
            return 0;

        dt = (millis() - dt) / 1000;

        return makeTime(tm) + dt;
    }

    bool parseTimeResponse(tmElements_t& tm, const char * buf)
    {
        String date;

        for (int i = 0; i < BUF && buf[i] != '\0'; i++)
        {
            if (buf[i] == '"')
            {
                i++;
                for (int j = 0; buf[i] != '"' && buf[i] != '\0'; j++)
                    date += buf[i++];
                break;
            }
        }

        tm.Year = date.substring(0, 2).toInt() + 2000 - 1970;
        tm.Month = date.substring(3, 5).toInt();
        tm.Day = date.substring(6, 8).toInt();
        tm.Hour = date.substring(9, 11).toInt();
        tm.Minute = date.substring(12, 14).toInt();
        tm.Second = date.substring(15, 17).toInt();

        return true;
    }

    bool syncTime()
    {
        time_t t = 0;

        t = getTime();

        if (!t)
            return false;

        if (t < MIN_TIMESTAMP)
            return false;

        setTime(t);

        return true;
    }
};

#endif
