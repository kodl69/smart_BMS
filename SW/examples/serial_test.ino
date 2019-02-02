/*
  
 balancer s attiny 85
  zapojení vývodů:      _________________
  +  10k   reset    -1| o            +Vcc |8 +V cell, 2.5-4.5V
  ref. 1.2V LM385   -2| adc A3      in 2  |7- RX, INT0 for wake up from sleep mode
bal. current input  -3| adc A2     out 1  |6- output TX software serial   
              gnd   -4| GND        out 0  |5- HIGH - balance on 
                       ___________________
 */
#include <SoftwareSerial.h>
SoftwareSerial mySerial(0, 1); // RX, TX
String inputString = "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int voltage=3540;  //napětí v milivoltech 
int current=20;  //proud (nap. na snímacím odporu)
int refvolt=1200; //napětí reference v mV, doplnit zápis a čtení z eeprom 
int Ssense=111; // převrácená hodnota snímacího odporu proudu --  7miliohm=142 S 9miliohm =111 S

/*   přerušení?
  const byte ledPin = 13;
const byte interruptPin = 0;
volatile byte state = LOW;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
*/
void setup() {
  // initialize serial:
  mySerial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {

measure();
comunication();
  
  // print the string when a newline arrives:
  if (stringComplete) {
    mySerial.print(inputString); //odešle na port
    mySerial.print((170), HEX ); //start char
    mySerial.print(voltage, HEX);  //naměřená hodnota - fake data, OD D4 HEX
    mySerial.print(current, HEX);
    mySerial.print("@"); //end char
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {  //to asi nefunguje, je potřeba to napsat na přerušení
  while (mySerial.available()) {
    // get the new byte:
    char inChar = (char)mySerial.read();
    if (inChar == '@') {
      stringComplete = true;
    }
    else{
    // add it to the inputString:
    inputString += inChar;
    }
  }
}

void comunication() {  //tady by měla být všechno o komunikaci - spouštění přerušením
  
}

void measure() {  //tady by mělo být všechno o měření
int pom = analogRead(A1);    //měření naprázdno   
int reference= (analogRead(A1)+7*reference)/8;  // digitální filtr 
voltage=int (long (refvolt) * 1024L)/long(reference);
pom = analogRead(A2);    //měření naprázdno
current= int(long(analogRead(A2)*refvolt*Ssense)/long(reference*1000L)) ; //výpočet proudu
  
}
