#ifndef _ERROR_H_
#define _ERROR_H_

//DHT Errors
// Success.
//#define SimpleDHTErrSuccess 0
// Error to wait for start low signal.
//#define SimpleDHTErrStartLow 100
// Error to wait for start high signal.
//#define SimpleDHTErrStartHigh 101
// Error to wait for data start low signal.
//#define SimpleDHTErrDataLow 102
// Error to wait for data read signal.
//#define SimpleDHTErrDataRead 103
// Error to wait for data EOF signal.
//#define SimpleDHTErrDataEOF 104
// Error to validate the checksum.
//#define SimpleDHTErrDataChecksum 105
// Error when temperature and humidity are zero, it shouldn't happen.
//#define SimpleDHTErrZeroSamples 106

//BMP180 Errors
// 0 = Success
// 1 = Data too long to fit in transmit buffer
// 2 = Received NACK on transmit of address
// 3 = Received NACK on transmit of data
// 4 = Other error

#define TIME_SYNC_ERROR 201

#endif
