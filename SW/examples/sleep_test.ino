/*
 * balancer with attiny85:
     
                        _________________
  +  10k   reset    -1| o            +Vcc |8 +V cell, 2.5-4.5V
HIGH - balance on   -2| output 3   adc A1 |7 ref. 1.2V LM385    
ball. current input -3| adc A2     out 1  |6 output TX software serial   
              gnd   -4| GND        in 0   |5 in RX, INT0 for wake up from sleep mode 
                       ___________________

 */
//http://blog.onlinux.fr/interruption-and-wdt-code-attiny85/

 
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h> 
 
 
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
//
// W H Y  V O L A T I L E ?
//
// Why volatile?
// ---->
// https://www.arduino.cc/en/Reference/Volatile
// volatile keyword:
// Specifically, it directs the compiler to load the variable from RAM and not from a storage register,
// which is a temporary memory location where program variables are stored and manipulated. 
// Under certain conditions, the value for a variable stored in registers can be inaccurate.
 
volatile boolean f_wdt = 1;
 
// Use pin PB0 as wake up pin --- tj pin 5, rx?
const int wakeUpPin = 0;
 
const byte pinLed = 3;   //balance - yellow
const byte pinLedRed = 1;  //tx, red
 
void blink(int ii) {
  pinMode(pinLed, OUTPUT);
  for (byte i = ii ;  i > 0 ; i--){
     digitalWrite(pinLed, HIGH);
     delay(50);
     digitalWrite(pinLed, LOW); 
     delay(50);
  }
 
  pinMode(pinLed, INPUT); // reduce power
}
 
void blinkRed(int ii) {
  pinMode(pinLedRed, OUTPUT);
  for (byte i = ii ;  i > 0 ; i--){
     digitalWrite(pinLedRed, HIGH);
     delay(50);
     digitalWrite(pinLedRed, LOW); 
     delay(50);
  }
  pinMode(pinLedRed, INPUT); // reduce power
}
 
/******************************************************************/
// 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
// 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
void setup_watchdog(int ii) {
 
  byte bb;
  int ww;
  if (ii > 9 ) ii=9;
  bb=ii & 7;
  if (ii > 7) bb|= (1<<5);
  bb|= (1<<WDCE);
  ww=bb;
 
  MCUSR &= ~(1<<WDRF);
  // start timed sequence
  WDTCR |= (1<<WDCE) | (1<<WDE);
  // set new watchdog timeout value
  WDTCR = bb;
  WDTCR |= _BV(WDIE);
} 
 
void system_sleep() {
  cbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter OFF
 
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();
 
  sleep_mode();                        // System sleeps here, waiting for interrupt
 
  sleep_disable();                     // System continues execution here when watchdog timed out 
  sbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter ON
}
 
// Watchdog Interrupt Service / is executed when watchdog timed out
ISR(WDT_vect) {
  f_wdt=1;  // set global flag
}
 
 
void setup()
{
  setup_watchdog(9);
  pinMode(wakeUpPin, INPUT_PULLUP);        // Set the pin to input
 // digitalWrite(wakeUpPin, HIGH);    // Activate internal pullup resistor 
  PCMSK  |= bit (PCINT0);  //                                   Pin Change Mask Register
  GIFR   |= bit (PCIF);    // clear any outstanding interrupts  General Interrupt Flag Register
  GIMSK  |= bit (PCIE);    // enable pin change interrupts      General Interrupt Mask Register
  sei();                   // enable interrupts
}
 
ISR (PCINT0_vect){}
 
void loop()
{
  system_sleep();
  if ( f_wdt > 0) {  // watchdog signal
    blink(5);
    f_wdt=0;         //reset flag
  } else {
    blinkRed(5);     // PCINT0 occured
  }
 
}
