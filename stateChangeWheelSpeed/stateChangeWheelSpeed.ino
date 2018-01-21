#include <math.h>
#include <WheelSpeed.h>

WheelSpeed myWheel = WheelSpeed(2, 1);

unsigned long prevTime;

void setup() {
    Serial.begin(9600);     // starts the serial monitor
    Serial.println("started");
    delay(1000);
    prevTime = millis();
}

void loop() {
    /*   When the state changes. Prints the calculated speed
     *   to the serial monitor.
     */

     
     if (millis() - prevTime > 1) {
        Serial.println(myWheel.getSpeed());
        prevTime = millis();
     }

}
    
//
//float groundSpeed(unsigned long timeDiff) {
//    /*  Calculates the wheel speed in miles per hour using
//     *  the time difference between sensor state changes,
//     */
//     //rotations per second
//    float secPerRotation = (timeDiff / 1000000.0) * WHEEL_SPOKES;
//    float rotationPerSec = 1 / secPerRotation;
//    return rotationPerSec;
//    float radPerSec = rotationPerSec * 2 * PI;
//    float radPerHour = radPerSec * 3600;
//    return radPerHour * WHEEL_RADIUS_MILES;
//}
