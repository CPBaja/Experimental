//Main Data-recieving code, currently only for gps data

#include <SPI.h>
#include "FileWriter.h"
#include "WheelSpeed.h"

const byte HE_1 = 0;
const byte HE_2 = 1;

const byte TRIGGERS = 12;
const byte CHARS = 3;

unsigned long prev_write;
uint32_t logTime;
unsigned long REFRESH_RATE = 250;//miliseconds

const int MAX_BUFFER = 12; //TODO: find max buffer

File file;
SdFatSdioEX sdEx;
String data_buffer;

//Instantiate front wheel speed
WheelSpeed fWheel = WheelSpeed(TRIGGERS);
WheelSpeed rWheel = WheelSpeed(TRIGGERS);
  

void setup() {
//  Serial1.begin(19200); //Slave connection
//    while (!Serial1) {
//    SysCall::yield();
//  }
//  Serial1.setRX(5);
  
    fileSetUp(file, sdEx);

    const byte fWheelInterrupt = digitalPinToInterrupt(HE_1);
    attachInterrupt(fWheelInterrupt, fWheelISR, RISING);
  
    const byte rWheelInterrupt = digitalPinToInterrupt(HE_2);
    attachInterrupt(rWheelInterrupt, rWheelISR, RISING);
  
    prev_write = millis();
	  
 }//end setup

void loop() {
  // Time for next record.

  if(prev_write - millis() >= REFRESH_RATE){
    prev_write = millis();
    file.print(hour());
    file.print(":");
    file.print(minute());
    file.print(":");
    file.print(second());
    file.print(",");
    file.print(fWheel.getRPS() * 5.75958653);
    file.print(",");
    file.print(rWheel.getRPS() * 5.75958653);
    file.print(String(millis(), 5)); // Assumes 11" effective wheel radius
    file.print("\n");
  }
  
//  if(data_buffer > MAX_BUFFER){
//    cli();
//    file.print(",,,,,");
//    file.print(data_buffer);
//    file.print("\n");
//    data_buffer = "";
//    sei();  
//  }

   // Force data to SD and update the directory entry to avoid data loss.
  if (!file.sync() || file.getWriteError()) {
    error("write error");
  }
}//end loop

void fWheelISR() {
  fWheel.calcRPS();
}

void rWheelISR() {
  rWheel.calcRPS();
}

//ISR for slave
//void serialEvent1(){
//  while(Serial1.available() > 0){ //need to read enitre string, might not be the way to go
//    data_buffer += Serial1.read(); 
//  }  
//}
