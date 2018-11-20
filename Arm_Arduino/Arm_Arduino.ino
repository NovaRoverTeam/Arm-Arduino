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

int sensorPin = A0;


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
int16_t velEffector_Angle = -2048;
int posEffector_Position = 1500; //4750
double posEffector_Position_Measured;
int16_t velEffector_Position = 0;

String data;
int dir;

int count = 0;

double Ang = 90;
double Pos = 55;

double beta;
double alpha;

double coefficient_list[61][3] = {{2.5765,-0.596369,0.0744735},{2.57074,-0.593139,0.0719147},{2.56491,-0.589887,0.0693766},{2.55899,-0.586609,0.0668574},{2.55299,-0.583304,0.0643557},{2.54691,-0.579969,0.06187},{2.54073,-0.576604,0.059399},{2.53447,-0.573204,0.0569412},{2.52811,-0.56977,0.0544955},{2.52165,-0.566298,0.0520606},{2.5151,-0.562788,0.0496355},{2.50844,-0.559237,0.047219},{2.50168,-0.555644,0.04481},{2.4948,-0.552007,0.0424078},{2.48782,-0.548324,0.0400111},{2.48072,-0.544593,0.0376193},{2.47351,-0.540814,0.0352314},{2.46617,-0.536985,0.0328466},{2.45871,-0.533104,0.0304642},{2.45113,-0.529169,0.0280833},{2.44341,-0.52518,0.0257035},{2.43556,-0.521135,0.0233239},{2.42757,-0.517032,0.0209439},{2.41943,-0.51287,0.0185631},{2.41115,-0.508648,0.0161808},{2.40272,-0.504365,0.0137966},{2.39414,-0.500018,0.01141},{2.3854,-0.495608,0.00902051},{2.37649,-0.491133,0.0066278},{2.36742,-0.486592,0.00423149},{2.35817,-0.481983,0.00183126},{2.34874,-0.477306,-0.000573172},{2.33913,-0.472559,-0.00298206},{2.32933,-0.467741,-0.00539561},{2.31933,-0.462852,-0.00781401},{2.30913,-0.45789,-0.0102374},{2.29872,-0.452855,-0.0126659},{2.2881,-0.447744,-0.0150995},{2.27725,-0.442559,-0.0175383},{2.26617,-0.437297,-0.0199822},{2.25485,-0.431957,-0.0224313},{2.24328,-0.42654,-0.0248854},{2.23146,-0.421043,-0.0273445},{2.21937,-0.415466,-0.0298083},{2.20699,-0.409807,-0.0322766},{2.19433,-0.404067,-0.0347492},{2.18136,-0.398244,-0.0372259},{2.16807,-0.392336,-0.0397063},{2.15446,-0.386343,-0.04219},{2.14049,-0.380264,-0.0446767},{2.12615,-0.374096,-0.0471661},{2.11143,-0.367839,-0.0496575},{2.09629,-0.361491,-0.0521505},{2.08072,-0.35505,-0.0546447},{2.06469,-0.348514,-0.0571394},{2.04817,-0.34188,-0.0596342},{2.03111,-0.335145,-0.0621285},{2.01349,-0.328306,-0.0646216},{1.99526,-0.321359,-0.067113},{1.97636,-0.314298,-0.0696022},{1.95673,-0.307117,-0.0720886}};

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
  posEffector_Position_Measured = spiTransfer(ssEffector_Position, velEffector_Position);

  

  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
  update_Endeffector_Angle();
  effector_angle(posEffector_Angle, ssEffector_Angle);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

  //Pos = constrain((double)dir*0.01 + Pos,0, 55.72);
    
  /*
  count = count+1;
  if (count == 1000){
      velEffector_Angle = velEffector_Angle - 100;
      if (velEffector_Angle<-2048){
        velEffector_Angle = -1024;
      }
      count = 0;
  }
  */
 
  //SerialUSB.print(analogRead(A0));
  //SerialUSB.print( velEffector_Angle);
  /*
  SerialUSB.print(analogRead(A0));
  SerialUSB.print("\t");
  SerialUSB.print(analogRead(A3));

  */

  //effector_position(posEffector_Position, ssEffector_Position);
/*
  SerialUSB.print("\t");
  SerialUSB.print(Ang);
  SerialUSB.print("\t");
  SerialUSB.print(alpha);
  SerialUSB.print("\t");
  SerialUSB.print(posEffector_Angle);
  SerialUSB.print("\t");
  SerialUSB.print(Pos);
  SerialUSB.print("\t");
  SerialUSB.print(beta);
  SerialUSB.print("\t");
  SerialUSB.print(posEffector_Position);
  
  
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

  if (Pos >= 55.72 && velEffector_Position > 0){
    velEffector_Position = 0;
  }


}

void update_Endeffector_Angle()
{
  
  beta = doublemap(posEffector_Position_Measured,0,994,135,153);

  Pos = 12.5 + 61.78*cos(1.5707963267948966 - 0.017453292519943295*beta);

  alpha = coefficient_list[round(Pos)][2] * pow((float)Ang/180*3.14159,2)+ coefficient_list[round(Pos)][1] * (float)Ang/180*3.14159+coefficient_list[round(Pos)][0];

  alpha = alpha/3.14159*180;
/*
  SerialUSB.print("\t");
  SerialUSB.print(round(Pos));
  SerialUSB.print("\t");
  SerialUSB.print(coefficient_list[round(Pos)][2]);
*/
  posEffector_Angle = round(doublemap(alpha,80,113,0,2564));
  
  
}

void update_Endeffector()
{
  beta = 57.29577951308232*acos(-0.2023308514082227 + 0.016186468112657816*Pos)+90;

  posEffector_Position = round(doublemap(beta,135,153,0,994));

  alpha = coefficient_list[round(Pos)][2] * pow((float)Ang/180*3.14159,2)+ coefficient_list[round(Pos)][1] * (float)Ang/180*3.14159+coefficient_list[round(Pos)][0];

  alpha = alpha/3.14159*180;
/*
  SerialUSB.print("\t");
  SerialUSB.print(round(Pos));
  SerialUSB.print("\t");
  SerialUSB.print(coefficient_list[round(Pos)][2]);
*/
  posEffector_Angle = round(doublemap(alpha,78,113,0,2564));
  
  
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

    Ang = constrain((double)dir*0.01 + Ang,20, 90);

    update_Endeffector();
    
  }
  if (data == "effector_position"){

    
    velEffector_Position = 4095*dir;
    
    //Pos = constrain((double)dir*0.01 + Pos,0, 55.72);

    //update_Endeffector_Angle();

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

  speed = constrain(65 * (abs(set - position)) + 1024, 0, 4095);
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


  //SerialUSB.print("\t");
  //SerialUSB.print(position);
  //SerialUSB.print("\t");
}

void effector_position(int set, int ss)
{
  int16_t speed;

  //SerialUSB.print("eP: \t");
  //SerialUSB.print(velEffector_Position);
  posEffector_Position_Measured = spiTransfer(ss, velEffector_Position);

  speed = constrain(65 * (abs(set - posEffector_Position_Measured)) + 1024, 0, 4095);
  if (posEffector_Position_Measured >   set + 10)
  {
    velEffector_Position = speed;
  }
  else if (posEffector_Position_Measured < set - 10)
  {
    velEffector_Position = -speed;
  }
  else
  {
    velEffector_Position = 0;
  }


  SerialUSB.print("\t");
  SerialUSB.print(posEffector_Position_Measured);
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

double doublemap(double x, double in_min, double in_max, double out_min, double out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



