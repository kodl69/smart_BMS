/*
  Software serial multple atiiny in daisychain



 
 Receives from software serial, sends to software serial.

balancer with attiny85:
                        _________________
  +  10k   reset    -1| o            +Vcc |8 +V cell, 2.5-4.5V
HIGH - balance on   -2| output 3   adc A1 |7 ref. 1.2V LM385    
ball. current input -3| adc A2     out 1  |6 output TX software serial   
              gnd   -4| GND        in 0   |5 in RX, INT0 for wake up from sleep mode 
                       ___________________

 sopurce: created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example
 
 modified for attiny85 27.1.2019 Karel Sychra
 
 This example code is in the public domain.

 */
#include <SoftwareSerial.h>

SoftwareSerial mySerial(0, 1); // RX, TX
String datagram;

void setup() {
//mySerial.setTimeout(10);  //set timeout end of string (1ms is optional?) - default value is 1000ms
            
  mySerial.begin(9600); // set the data rate for the SoftwareSerial port
  mySerial.println("attiny85 BMS");
}

void loop() {                                                           // run over and over
 if (mySerial.available()) {             //Wait for data on tx
   datagram = mySerial.readString();
   mySerial.print(datagram);
 }
}
