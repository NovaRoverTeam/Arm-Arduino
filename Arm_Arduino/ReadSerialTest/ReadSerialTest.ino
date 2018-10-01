/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogReadSerial
*/
#define pin 13
char cTMP;


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  pinMode(pin, OUTPUT);
  SerialUSB.begin(2000000);
  while (SerialUSB.available()>0){ cTMP=SerialUSB.read();}  // flush the buffer
}

// the loop routine runs over and over again forever:
void loop() {
  if (SerialUSB.available() > 0) {
    String data = SerialUSB.readStringUntil('\n');
    if (data == "light_on") {
      digitalWrite(pin,HIGH);     
    }
    else if (data == "light_off") {
      digitalWrite(pin,LOW);   
    }

}
}
