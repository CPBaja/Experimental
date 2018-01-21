#include <math.h>

const int PIN_NUM = 2;

unsigned long prevTime;

// car parameters
const int WHEEL_SPOKES = 1;
const float WHEEL_DIAMETER = 20;
const float WHEEL_RADIUS_MILES = WHEEL_DIAMETER / 2 / 12 / 5280;

void setup() {
    Serial.begin(9600);     // starts the serial monitor
    Serial.println("started");
    delay(1000);
    pinMode(PIN_NUM, INPUT);  // initializes the sensor pin
    prevTime = micros();      // holds the time of the last state change
}

void loop() {
    /*   When the state changes. Prints the calculated speed
     *   to the serial monitor.
     */
    boolean triggered = !digitalRead(PIN_NUM);
    while (!triggered) {
        if (digitalRead(PIN_NUM) == triggered) {
            Serial.println("changed!");
            unsigned long currTime = micros();
            Serial.println(groundSpeed(currTime - prevTime));
            prevTime = currTime;
            triggered = 1;
        }
    }
}

float groundSpeed(unsigned long timeDiff) {
    /*  Calculates the wheel speed in miles per hour using
     *  the time difference between sensor state changes,
     */
     //rotations per second
    float secPerRotation = (timeDiff / 1000000.0) * WHEEL_SPOKES;
    float rotationPerSec = 1 / secPerRotation;
    float radPerSec = rotationPerSec * 2 * PI;
    float radPerHour = radPerSec * 3600;
    return radPerHour * WHEEL_RADIUS_MILES;
}
