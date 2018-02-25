#ifndef _TIME_H_
#define _TIME_H_

#include <TimeLib.h>

#define OK "OK\r\n"

#define CPRS_TYPE "AT+SAPBR=3,1, \"Contype\",\"GPRS\"\r\n"
#define APN "AT+SAPBR=3,1, \"APN\",\"internet\"\r\n"
#define CONTEXT "AT+SAPBR=1,1\r\n"
#define NTP_PROFILE "AT+CNTPCID=1\r\n"
#define NTP_SERVER "AT+CNTP=\"time.google.com\",0\r\n"
#define NTP "AT+CNTP\r\n"
#define TIME "AT+CCLK?\r\n"

#define BUF 128


struct DateTime
{
  byte y = 0;
  byte mm = 0;
  byte d = 0;
  byte h = 0;
  byte m = 0;
  byte s = 0;
};

class GSMNTP : public SIM800
{
  public:
    GSMNTP(unsigned int baudRate, unsigned int rxPin, unsigned int txPin, unsigned int rstPin, bool debug = TRUE):
      SIM800(baudRate, rxPin, txPin, rstPin, debug)
    {
    };

    time_t getTimestamp()
    {
      Serial.println("Time status: " + String(timeStatus()));
      
      if (timeStatus() == timeNotSet || timeStatus() == timeNeedsSync)
        if (!syncTime())
          return 0;

      return now();
    }

    private:
      bool parseTimeResponse(DateTime& dt, const char * buf)
      {
        char d[20];
        
        for (int i = 0; i < BUF && buf[i] != '\0'; i++)
        {
          if (buf[i] == '"')
          {
            i++;
            for (int j = 0; j < 20; j++)
              d[j] = buf[i++];
            break;
          }
        }
        
        String date(d);
        
        dt.y = date.substring(0, 2).toInt();
        dt.mm = date.substring(3, 5).toInt();
        dt.d = date.substring(6, 8).toInt();
        dt.h = date.substring(9, 11).toInt();
        dt.m = date.substring(12, 14).toInt();
        dt.s = date.substring(15, 17).toInt();

//        Serial.println(dt.y);
//        Serial.println(dt.mm);
//        Serial.println(dt.d);
//        Serial.println(dt.h);
//        Serial.println(dt.m);
//        Serial.println(dt.s);

        return true;
      }
      
      bool syncTime()
      {
        char buf[128];
      
        sendCmdAndWaitForResp(CPRS_TYPE, OK, 2000);
        sendCmdAndWaitForResp(APN, OK, 2000);
        sendCmdAndWaitForResp(CONTEXT, OK, 2000);
        sendCmdAndWaitForResp(NTP_PROFILE, OK, 2000);
        sendCmdAndWaitForResp(NTP_SERVER, OK, 2000);
        
        if (!sendCmdAndWaitForResp(NTP, OK, 2000))
          return false;
          
        sendCmd(TIME);
        
        cleanBuffer(buf, sizeof(buf));
        readBuffer(buf, sizeof(buf), 2000);

        DateTime dt;
        if (!parseTimeResponse(dt, buf))
          return false;
        
        setTime(dt.h, dt.m, dt.s, dt.d, dt.mm, dt.y);

        Serial.println("Time synced, timestamp: " + String(now()));

        return true;
      }
};

#endif
