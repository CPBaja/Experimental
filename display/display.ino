#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WheelSpeed.h>

Adafruit_SSD1306 display(4);

WheelSpeed myWheel = WheelSpeed(1);

unsigned long lastChange;

void setup() {
    Serial.begin(9600);
    Serial.print("Started");
    
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
    display.clearDisplay();
    display.setTextSize(4);
    display.setTextColor(WHITE);
    display.setCursor(5,16);
    display.println("Hello");
    display.display();
    delay(2000);

    attachInterrupt(digitalPinToInterrupt(3), stateChangeISR, RISING);
}

void loop() {
    display.clearDisplay();
    display.setTextSize(5);
    display.setTextColor(WHITE);
    display.setCursor(5,16);
    if (micros() - lastChange >= 750000) {
      myWheel.overrideRPS();
    }
    display.println(myWheel.getRPS());
    display.display();
}
    
void stateChangeISR() { myWheel.calcRPS(); lastChange = micros(); }
