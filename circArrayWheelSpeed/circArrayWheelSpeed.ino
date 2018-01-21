#include <math.h>

//github test
// precision parameters
const int TIMES_PER_SEC = 1;
const int CHECK_EVERY = 2500;
const int RESOLUTION = 1000000 / TIMES_PER_SEC / CHECK_EVERY;

// car parameters
const int NUM_SPOKES = 1;
const float WHEEL_DIAMETER = 20;
const float WHEEL_RADIUS_MILES = WHEEL_DIAMETER / 2 / 12 / 5280;

class Wheel {
    
    private:
    
      int PIN_NUM;                 // pin of the sensor
      boolean recent[RESOLUTION];  // tracks the changes in sensor state
      int idx = 0;                 // index to write the next change to
      boolean lastState = 1;       // last state of the sensor
      int changes = 0;             // amount of state changes over the time interval
    
    public:
    
      void init(int pin) {
          /*   Configures the sensors pin for input
           */
          PIN_NUM = pin;
          pinMode(PIN_NUM, INPUT); // configures the sensor pin
      }
      
      void updateChange() {
          /*   Pings sensor to determine whether the state of the 
           *   sensor has changed. Updates the chnage variable.
           */
          changes -= recent[idx];
          recent[idx] = 0;
          if (lastState != digitalRead(PIN_NUM)) {
              changes ++;
              recent[idx] = 1;
              lastState = !lastState;
          }
          idx = (idx + 1) % RESOLUTION;
      }
  
      float groundSpeed(){
          /*   Uses the changes over the time cycle to compute
           *   the ground speed in miles per hour.
           */
          //return changes;
          float rotationsPerSec = (changes * TIMES_PER_SEC) / (2 * NUM_SPOKES);
          float radsPerSec = rotationsPerSec * 2 * PI;
          float radsPerHour = radsPerSec * 3600;
          return radsPerHour * WHEEL_RADIUS_MILES;
      }
};

Wheel speedWheel;

void setup() {
    /*   Starts the serial monitor and initializes
     *   the wheel with the pin number.
     */
    Serial.begin(115200);
    speedWheel.init(1);
}

void loop() {
    /*   Cycles resolution times. Checks sensor speed on every pass.
     *   Prints the calculated speed to the serial monitor.
     */
    int count = 0;
    unsigned long startTime = 0;
    while (count < RESOLUTION) {
        if (micros() - startTime >= CHECK_EVERY) {
            unsigned long startTime = micros();
            speedWheel.updateChange();
            Serial.println(speedWheel.groundSpeed());
            count++;
        }
    }
}
