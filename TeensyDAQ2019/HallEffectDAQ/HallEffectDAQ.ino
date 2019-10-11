
#include <TimeLib.h>
#include <Wire.h>
#include <DS1307RTC.h> 

#include "WheelSpeed.h"
#include "Slave_Ports.h"


//TODO: add timestamps (read RTC library and examples)
//TODO: oversampling

const byte TRIGGERS = 1;
const byte CHARS = 3;
const int MAX_BUFFER_SIZE = 12; //TODO: find max write buffer 
String Buffer = "";

// Instantiate front wheel speed
WheelSpeed FR = WheelSpeed(TRIGGERS);
WheelSpeed FL = WheelSpeed(TRIGGERS);
WheelSpeed RW = WheelSpeed(TRIGGERS);
WheelSpeed En = WheelSpeed(TRIGGERS);

void setup() {
   Serial1.begin(19200);//random, verify max baud rate
   while (!Serial1) {
    ; // Wait for serial port to connect. Needed for native USB port only.
   }
   Serial1.setTX(5);

  // Set up front wheel
  const byte i1 = digitalPinToInterrupt(HE_FR);
  attachInterrupt(i1, ISR_FR, RISING);
  const byte i2 = digitalPinToInterrupt(HE_FL);
  attachInterrupt(i2, ISR_FL, RISING);
  const byte i3 = digitalPinToInterrupt(HE_RW);
  attachInterrupt(i3, ISR_RW, RISING);
  const byte i4 = digitalPinToInterrupt(HE_E);
  attachInterrupt(i4, ISR_E, RISING);

  
}//end setup

void loop() {
  if(Buffer.length() >= MAX_BUFFER_SIZE){
    writeBuffer();
  }
}//end loop

void writeBuffer(){
  cli();
  Serial1.print(Buffer);
  Buffer = "";
  sei();
}


void ISR_FR() {
  FR.calcRPS();
  Buffer += "r"+ ((byte)FR.getRPS()) + String(millis(), 5) ;
}

void ISR_FL() {
  FL.calcRPS();
  Buffer += "l"+ ((byte)FL.getRPS())+ String(millis(), 5);
}

void ISR_RW() {
  RW.calcRPS();
  Buffer += "w"+ ((byte)RW.getRPS())+ String(millis(), 5);
}

void ISR_E() {
  En.calcRPS();
  Buffer += "e"+ ((byte)En.getRPS())+ String(millis(), 5);
}
