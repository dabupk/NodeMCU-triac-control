#include <Ticker.h>

Ticker trigger;
void ICACHE_RAM_ATTR zeroCrossingInterrupt();  //add ISR in IRAM, neceassry for 2.5.2 core 

int zc_pin = D5;      // connected to GPIO14
int sig_pin = D7;     // connected to GPIO13
int brightness=0;     // variable for counting delay for triggering pulse
int state=0;          

void pulse_gen(){ // timer interrupt to triger gate
 // Serial.println("pulse genrated");
if(state==0){
  state=1;
  digitalWrite(sig_pin,HIGH);     //turns on the triac
trigger.attach_ms(1, pulse_gen);  // repeat timer interupt after 1ms
}// turn on gate

else {
  state=0;
  digitalWrite(sig_pin,LOW);
  trigger.detach();           //disable timer interupt
  } // turn off gate
}

void zeroCrossingInterrupt(){ //zero cross detect interrupt 
trigger.attach_ms(brightness, pulse_gen);    //run timer interupt after selected time by user
//Serial.println("interupt generated");
}

void setup()
{
pinMode(zc_pin, INPUT_PULLUP); //zero cross detect
pinMode(sig_pin, OUTPUT); //TRIAC gate control
attachInterrupt(zc_pin,zeroCrossingInterrupt, RISING); //interupt for detecting zero cross
//Serial.begin(115200);          //serial monitor for debuging
//Serial.println("Board is ready");
} 

void loop(){

brightness = map(analogRead(A0), 0, 1023, 2,9);     //read user input and convert it into time in mili-seconds

if(brightness>8) {
detachInterrupt(zc_pin); // disconnect interrrupt if triggering is closed to next cycle
}

else{
  attachInterrupt(zc_pin,zeroCrossingInterrupt, RISING); // connect interrupt
}

//Serial.print("brightness  : ");
//Serial.println(brightness);
delay(100);
}
