#include <math.h>
#include <SPI.h>

#define ss1 10
#define ss2 9

short mask = 0b0000000000000001;
bool b = 1;

uint16_t miso2;

uint8_t misoa2;
uint8_t misob2;

uint16_t position;
int16_t velocity = 0;
int seta = 7.9615*101;

uint16_t position2;
int16_t velocity2 = 0;
int16_t speed2;
int set2 = 62.96*0;//22968

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

  SerialUSB.begin(2000000);
  SPI.begin();
  pinMode( ss1, OUTPUT );
  pinMode( ss2, OUTPUT );
  digitalWrite( ss1, HIGH );
  digitalWrite( ss2, HIGH );

}

// the loop function runs over and over again forever
void loop() {

  linear_actuator(seta);

  
  position2 = spiTransfer(ss2, velocity2);

  speed2 = constrain(78.9 * (abs(set2 - position2)) + 128, 0, 4095);
  if (position2 < set2)
  {
    velocity2 = speed2;
  }
  else if (position2 > set2)
  {
    velocity2 = -speed2;
  }
  else
  {
    velocity2 = 0;
  }

  SerialUSB.print("wormgear: \t");
  SerialUSB.print(velocity2);
  SerialUSB.print("\t");
  SerialUSB.println(position2);

}

void linear_actuator(int set)
{
  int16_t speed;

  position = spiTransfer(ss1, velocity);

  speed = constrain(153.55 * (abs(set - position)) + 1024, 0, 4095);
  if (position < set)
  {
    velocity = speed;
  }
  else if (position > set)
  {
    velocity = -speed;
  }
  else
  {
    velocity = 0;
  }

  SerialUSB.print("linear actuator: \t");
  SerialUSB.print(velocity);
  SerialUSB.print("\t");
  SerialUSB.print(position);
  SerialUSB.print("\t");

  if (set == 1700 & position == 1700) {
    set = 100;
  }
  if (set == 100 & position == 100) {
    set = 1700;
  }
}

uint16_t spiTransfer( int ss, int16_t Tx)
{
  uint8_t MISOl;
  uint8_t MISOu;

  digitalWrite( ss, LOW );

  SPI.beginTransaction( SPISettings( 2000000, MSBFIRST, SPI_MODE0 ) );
  {
    MISOu = SPI.transfer(Tx >> 8);
    MISOl = SPI.transfer(Tx);
    digitalWrite( ss, HIGH );
  }
  SPI.endTransaction();

  return ((MISOu << 8) + MISOl);
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
