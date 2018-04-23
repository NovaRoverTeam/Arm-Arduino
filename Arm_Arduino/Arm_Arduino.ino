#include <math.h>
#include <SPI.h>

#define ss 10

short mask = 0b0000000000000001;
bool b = 1;
uint8_t miso;
byte count = 0;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  /* pinMode(0,OUTPUT);
    pinMode(1,OUTPUT);
    pinMode(2,OUTPUT);
    pinMode(3,OUTPUT);
    pinMode(4,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(6,OUTPUT);
    pinMode(7,OUTPUT);
    pinMode(8,OUTPUT);
    pinMode(9,OUTPUT);
    pinMode(10,OUTPUT);
    pinMode(11,OUTPUT);
    pinMode(12,OUTPUT);
    pinMode(13,OUTPUT);
    pinMode(20,OUTPUT);
    pinMode(21,OUTPUT);*/

  SerialUSB.begin(115200);
  SPI.begin();
  pinMode( ss, OUTPUT );

}

// the loop function runs over and over again forever
void loop() {
  
  digitalWrite( ss, LOW );

  
  SPI.beginTransaction( SPISettings( 2000000, MSBFIRST, SPI_MODE0 ) );
  {
  miso = SPI.transfer(count);
  digitalWrite( ss, HIGH );
  }
  SPI.endTransaction();
  
  
  SerialUSB.println("msg");
  SerialUSB.println(count);
  SerialUSB.println(miso);
  
  count = count+1;
  if (count>255)
  {
    count = 0;
  }
  delay(100);
}

void parallel_output(short data)
{
  digitalWrite(0, data & mask << 0);
  digitalWrite(1, data & mask << 1);
  digitalWrite(4, data & mask << 2);
  digitalWrite(3, data & mask << 3);
  digitalWrite(2, data & mask << 4);
  digitalWrite(5, data & mask << 5);
  digitalWrite(6, data & mask << 6);
  digitalWrite(7, data & mask << 7);
  digitalWrite(8, data & mask << 8);
  digitalWrite(9, data & mask << 9);
  digitalWrite(10, data & mask << 10);
  digitalWrite(11, data & mask << 11);
  digitalWrite(12, data & mask << 12);
  digitalWrite(13, data & mask << 13);
  digitalWrite(20, data & mask << 14);
  digitalWrite(21, data & mask << 15);
}

