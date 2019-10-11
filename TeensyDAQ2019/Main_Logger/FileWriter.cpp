#include "FileWriter.h"

// Write data header.
void writeHeader(File &file) {
  file.print(F("Time"));
  file.print(F(",Front WheelSpeed"));
  file.print(F(",Rear WheelSpeed"));
  file.println();
}

time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}

void fileSetUp(File &file, SdFatSdioEX &sdEx) {
  const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
  char fileName[13] = FILE_BASE_NAME "00.csv";
  char runName[6] = DIR_BASE_NAME "00";
  
  setSyncProvider(getTeensy3Time);
  Serial.begin(9600);
  
  // Wait for USB Serial 
  while (!Serial) {
    SysCall::yield();
  }
  delay(1000);
  
  String s = String(month()) + "/" + String(day()) + "/" + String(year()) + "_" + runName;
  String p = String(hourFormat12()) + ":" + String(minute()) + ":" + String(second()) + "_" + fileName;
  Serial.println("Folder Name: " + s);
  Serial.println("File Name: " + p);
  
  Serial.println(F("Type any character to start"));
  while (!Serial.available()) {
    SysCall::yield();
  }
  
  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sdEx.begin()) {
    sdEx.initErrorHalt();
  }
  sdEx.chvol();
  // Find an unused file name.
  if (BASE_NAME_SIZE > 6) {
    error("FILE_BASE_NAME too long");
  }
  while (sdEx.exists(runName)) {
    if (runName[3 + 1] != '9') {
      runName[3 + 1]++;
    } else if (runName[3] != '9') {
      runName[3 + 1] = '0';
      runName[3]++;
    } else {
      error("Can't create dir name");
    }
  }
  
  if(!sdEx.mkdir(runName)){
    error("mkdir error");
  }
  
  sdEx.chdir(runName);
  
  while (sdEx.exists(fileName)) {
    if (fileName[BASE_NAME_SIZE + 1] != '9') {
      fileName[BASE_NAME_SIZE + 1]++;
    } else if (fileName[BASE_NAME_SIZE] != '9') {
      fileName[BASE_NAME_SIZE + 1] = '0';
      fileName[BASE_NAME_SIZE]++;
    } else {
      error("Can't create file name");
    }
  }
  if (!file.open(fileName, O_RDWR | O_CREAT)) {
    error("file.open");
  }
  
  // Read any Serial data.
  do {
    delay(10);
  } while (Serial.available() && Serial.read() >= 0);
 
  Serial.print(F("Logging to: "));
  Serial.println(fileName);

  writeHeader(file);
}
