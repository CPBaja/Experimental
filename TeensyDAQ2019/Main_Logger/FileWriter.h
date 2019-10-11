#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <Arduino.h>
#include <DS1307RTC.h>
#include <TimeLib.h>
#include "SdFat.h"

// Log file base name.  Must be six characters or less.
#define FILE_BASE_NAME "Data"
#define DIR_BASE_NAME "Run"

// Error messages stored in flash.
#define error(msg) sdEx.errorHalt(F(msg))



const uint8_t ANALOG_COUNT = 1;

void fileSetUp(File &file, SdFatSdioEX &sdEx);

#endif
