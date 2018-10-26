#include <math.h>
#include <SPI.h>

//#define USE_USBCON     // May not need this line, depending on Arduino nano hardware
//#include <ros.h>       // ROS Arduino library
//#include <rover/ArmCmd.h> // ROS msg for arm commands



#define ssBase 10
#define ssActuator_Lower 9
#define ssActuator_Upper 8
#define ssWrist_Horizontal 7
#define ssWrist_Vertical 6
#define ssWrist_Rotate 5
#define ssEffector_Angle 2
#define ssEffector_Position 3


short mask = 0b0000000000000001;
bool b = 1;

uint16_t miso2;

uint8_t misoa2;
uint8_t misob2;

uint16_t position;
int16_t velocity = 0;
int seta = 7.9615 * 0;

uint16_t position2;
int16_t velocity2 = 0;
int16_t speed2;
int set2 = 63 * 0; //3777*90;//22968

int posBase = 0;
int velBase = 0;

int posActuator_Lower = 0; //max = 1820
int posActuator_Upper = 0; //max = 1820
int16_t velActuator_Lower = 0;
int16_t velActuator_Upper = 0;

int posWrist_Horizontal = 0; //max = 12000
int16_t velWrist_Horizontal = 0;//
int posWrist_Vertical = 0; //max =
int16_t velWrist_Vertical = 0;

int16_t posWrist_Rotation = 0;
int16_t velWrist_Rotation = 0;

int posEffector_Angle = 1000;   //max = 4500
int16_t velEffector_Angle = 0;
int posEffector_Position = 1500; //4750
int16_t velEffector_Position = 0;

String data;
int dir;

int count = 0;





// the setup function runs once when you press reset or power the board
void setup() {
  SerialUSB.begin(2000000);
  SPI.begin();
  pinMode( ssBase, OUTPUT );
  pinMode( ssActuator_Lower, OUTPUT );
  pinMode( ssActuator_Upper, OUTPUT );
  pinMode( ssWrist_Horizontal, OUTPUT );
  pinMode( ssWrist_Vertical, OUTPUT );
  pinMode( ssWrist_Rotate, OUTPUT );
  pinMode( ssEffector_Angle, OUTPUT );
  pinMode( ssEffector_Position, OUTPUT );
  digitalWrite( ssBase, HIGH );
  digitalWrite( ssActuator_Lower, HIGH );
  digitalWrite( ssActuator_Upper, HIGH );
  digitalWrite( ssWrist_Horizontal, HIGH );
  digitalWrite( ssWrist_Vertical, HIGH );
  digitalWrite( ssWrist_Rotate, HIGH  );
  digitalWrite( ssEffector_Angle, HIGH );
  digitalWrite( ssEffector_Position, HIGH );

  pinMode( 0, OUTPUT );
  pinMode( 1, OUTPUT );
}



// the loop function runs over and over again forever
void loop() {

  posBase = spiTransfer(ssBase, velBase);
  posActuator_Lower = spiTransfer(ssActuator_Lower, velActuator_Lower);
  posActuator_Upper = spiTransfer(ssActuator_Upper, velActuator_Upper);
  posWrist_Horizontal = spiTransfer(ssWrist_Horizontal, velWrist_Horizontal);
  posWrist_Vertical = spiTransfer(ssWrist_Vertical, velWrist_Vertical);
  posWrist_Rotation = spiTransfer(ssWrist_Rotate, velWrist_Rotation);
  //posEffector_Angle = spiTransfer(ssEffector_Angle, velEffector_Angle);
  //posEffector_Position = spiTransfer(ssEffector_Position, velEffector_Position);

  effector_angle(posEffector_Angle, ssEffector_Angle);
  effector_position(posEffector_Position, ssEffector_Position);
  
/*
  SerialUSB.print(posBase);
  SerialUSB.print('\t');
  SerialUSB.print(posActuator_Lower);
  SerialUSB.print('\t');
  SerialUSB.print(posActuator_Upper);
  SerialUSB.print('\t');
  SerialUSB.print(posWrist_Horizontal);
  SerialUSB.print('\t');
  SerialUSB.print(posWrist_Vertical);
  SerialUSB.print('\t');
  SerialUSB.print(posWrist_Rotation);
  SerialUSB.print('\t');
  SerialUSB.print(posEffector_Angle);
  SerialUSB.print('\t');
  SerialUSB.print(posEffector_Position);
  */
  SerialUSB.print('\n');

   
  pcControl();

}

void pcControl()
{
  if (SerialUSB.available() > 0) {
    data = SerialUSB.readStringUntil(',');
    dir = SerialUSB.readStringUntil('\n').toInt();
  }

  if (data == "base"){
    velBase = 2048*dir;
  }
  if (data == "actuator_lower"){
    velActuator_Lower = 4095*dir;
  }
  if (data == "actuator_upper"){
    velActuator_Upper = 4095*dir;
  }
  if (data == "wrist_horizontal"){
    velWrist_Horizontal = 4095*dir;
  }
  if (data == "wrist_vertical"){
    velWrist_Vertical = 4095*dir;
  }
  if (data == "wrist_rotate"){
    velWrist_Rotation = 4095*dir;
  }
  if (data == "effector_angle"){
    posEffector_Angle = dir + posEffector_Angle;
  }
  if (data == "effector_position"){
    posEffector_Position = dir + posEffector_Position;
  }
  if (data == "reset"){
    if (dir==1){
      spiTransfer(ssEffector_Angle, 4095);
      spiTransfer(ssEffector_Position, 4095);
    }
    else{
      spiTransfer(ssEffector_Angle, 0);
      spiTransfer(ssEffector_Position, 0);
    }
    posEffector_Angle = 0;
    posEffector_Position = 0;
  }
}

static inline int8_t isPositive(int val) {
  if (val < 0) return 0;
  if (val == 0) return 0;
  return 1;
}

void base_actuator(int set, int ss)
{
  int16_t speed;

  //SerialUSB.print("b: \t");
  //SerialUSB.print(velBase);
  position = spiTransfer(ss, 0);//velBase);

  speed = constrain(153.55 * (abs(set - position)) + 1024, 0, 4095);
  if (position > set + 3)
  {
    velBase = speed;
  }
  else if (position < set - 3)
  {
    velBase = -speed;
  }
  else
  {
    velBase = 0;
  }

  //SerialUSB.print("\t");
  //SerialUSB.print(position);
  //SerialUSB.print("\t");
}

void linear_actuatorL(int set, int ss)
{
  int16_t speed;

  //SerialUSB.print("aL: \t");
  //SerialUSB.print(velActuator_Lower);
  //position = spiTransfer(ss, 0);//velActuator_Lower);

  speed = constrain(153.55 * (abs(set - position)) + 1024, 0, 4095);
  if (position < set)
  {
    velActuator_Lower = speed;
  }
  else if (position > set)
  {
    velActuator_Lower = -speed;
  }
  else
  {
    velActuator_Lower = 0;
  }


  //SerialUSB.print("\t");
  //SerialUSB.print(position);
  //SerialUSB.print("\t");


}

void linear_actuatorU(int set, int ss)
{
  int16_t speed;

  //SerialUSB.print("aU: \t");
  //SerialUSB.print(velActuator_Upper);
  position = spiTransfer(ss, 0);//velActuator_Upper);

  speed = constrain(153.55 * (abs(set - position)) + 1024, 0, 4095);
  if (position < set)
  {
    velActuator_Upper = speed;
  }
  else if (position > set)
  {
    velActuator_Upper = -speed;
  }
  else
  {
    velActuator_Upper = 0;
  }


  //SerialUSB.print("\t");
  //SerialUSB.print(position);
  //SerialUSB.print("\t");
}

void wrist_actuatorH(int set, int ss)
{
  int16_t speed;

  //SerialUSB.print("wH: \t");
  //SerialUSB.print(velWrist_Horizontal);
  position = spiTransfer(ss, 0);//velWrist_Horizontal);

  speed = constrain(153.55 * (abs(set - position)) + 1024, 0, 4095);
  if (position >   set + 3)
  {
    velWrist_Horizontal = speed;
  }
  else if (position < set - 3)
  {
    velWrist_Horizontal = -speed;
  }
  else
  {
    velWrist_Horizontal = 0;
  }


  //SerialUSB.print("\t");
  //SerialUSB.print(position);
  //SerialUSB.print("\t");
}

void wrist_actuatorV(int set, int ss)
{
  int16_t speed;

  //SerialUSB.print("wV: \t");
  //SerialUSB.print(velWrist_Vertical);
  position = spiTransfer(ss, 0);//velWrist_Vertical);

  speed = constrain(153.55 * (abs(set - position)) + 1024, 0, 4095);
  if (position >   set + 3)
  {
    velWrist_Vertical = speed;
  }
  else if (position < set - 3)
  {
    velWrist_Vertical = -speed;
  }
  else
  {
    velWrist_Vertical = 0;
  }


  //SerialUSB.print("\t");
  //SerialUSB.print(position);
  //SerialUSB.print("\t");
}


void effector_angle(int set, int ss)
{
  int16_t speed;

  //SerialUSB.print("eA: \t");
  //SerialUSB.print(velEffector_Angle);
  
  position = spiTransfer(ss, velEffector_Angle);

  speed = constrain(153.55 * (abs(set - position)) + 1024, 0, 4095);
  if (position >   set + 10)
  {
    velEffector_Angle = speed;
  }
  else if (position < set - 10)
  {
    velEffector_Angle = -speed;
  }
  else
  {
    velEffector_Angle = 0;
  }


  SerialUSB.print("\t");
  SerialUSB.print(position);
  //SerialUSB.print("\t");
}

void effector_position(int set, int ss)
{
  int16_t speed;

  //SerialUSB.print("eP: \t");
  //SerialUSB.print(velEffector_Position);
  position = spiTransfer(ss, velEffector_Position);

  speed = constrain(153.55 * (abs(set - position)) + 1024, 0, 4095);
  if (position >   set + 10)
  {
    velEffector_Position = speed;
  }
  else if (position < set - 10)
  {
    velEffector_Position = -speed;
  }
  else
  {
    velEffector_Position = 0;
  }


  SerialUSB.print("\t");
  SerialUSB.print(position);
  //SerialUSB.println("\t");
}
uint16_t spiTransfer( int ss, int16_t Tx)
{
  uint8_t MISOl;
  uint8_t MISOu;

  digitalWrite( ss, LOW );

  SPI.beginTransaction( SPISettings( 200000, MSBFIRST, SPI_MODE0 ) );
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
