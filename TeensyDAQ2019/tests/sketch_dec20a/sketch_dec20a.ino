//Timer-based interrupt test taken from 
//http://jamesgregson.blogspot.com/2013/01/periodic-interrupt-timers-on-teensy-30.html

#define TIE 0x2
#define TEN 0x1
 
void pit0_isr(void){
  digitalWrite( 13, !digitalRead(13) );
  PIT_TFLG0 = 1; 
}
 
void setup(){
  pinMode(13,OUTPUT);
  SIM_SCGC6 |= SIM_SCGC6_PIT;
  PIT_MCR = 0x00;
  NVIC_ENABLE_IRQ(IRQ_PIT_CH0);
  PIT_LDVAL0 = 0x2faf080;
  PIT_TCTRL0 = TIE;
  PIT_TCTRL0 |= TEN;
  PIT_TFLG0 |= 1;
}
 
void loop(){
  delay(2000);
}
