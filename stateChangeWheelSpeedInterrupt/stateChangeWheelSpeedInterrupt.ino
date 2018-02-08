#include <WheelSpeedInterrupt.h>

WheelSpeed myWheel = WheelSpeed(3, 1);

unsigned long prevTime;

void setup() {
    Serial.begin(9600);     // starts the serial monitor
    Serial.println("started");
    delay(1000);
    attachInterrupt(digitalPinToInterrupt(myWheel.getPin()), stateChangeISR, RISING);
}

void loop() {
    /*   When the state changes. Prints the calculated speed
     *   to the serial monitor.
     */
     Serial.println(myWheel.getRps());
}
    
void stateChangeISR() {
    myWheel.calcRps();
}

