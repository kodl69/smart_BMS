/*
  basic balance function
  balancer with attiny85:
                        _________________
  +  10k   reset    -1| o            +Vcc |8 +V cell, 2.5-4.5V
HIGH - balance on   -2| output 3   adc A1 |7 ref. 1.2V LM385    
ball. current input -3| adc A2     out 1  |6 output TX software serial   
              gnd   -4| GND        in 0   |5 in RX, PCINT0 for wake up from sleep mode 
                       ___________________

*/

const int balance= 351; // balance=1024*Vref/Vbalance , for Vref =1200 mV and Vbalance 3.5V is balance=351 
const int balancePin= 3;

// the setup routine runs once when you press reset:
void setup() {
 
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 1:
  int sensorValue = analogRead(A1); // VCC=1024*Vref/analogRead(A1)  - for Vref=1200mV  is VCC=1228800/analogRead(A1)
 if (sensorValue>balance)
   {
    digitalWrite(balancePin, HIGH);
   }
  else
    {
      digitalWrite(balancePin, LOW);
    }
  delay(1000);        // delay for battery voltage incerase/decrease
}
