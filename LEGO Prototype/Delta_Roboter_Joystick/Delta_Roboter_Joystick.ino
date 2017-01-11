#include <Servo.h>

//Servo pin numbers
#define servo1 3
#define servo2 5
#define servo3 6

//Joystick pin numbers
#define joyPinX 0
#define joyPinY 1
#define joyPinBtn 7

//Default position at startup
#define defaultx 0
#define defaulty 0
#define minz -200
#define maxz -130

//Movement speed
#define spd 0.3

//robot geometry
const float e = 40.0;     //end effector radius
const float f = 70.0;     //base radius
const float re = 185.0;   //forearm length
const float rf = 100.0;   //bicep length

//trigonometric constants
const float sqrt3 = sqrt(3.0);
const float pi = 3.141592653;
const float sin120 = sqrt3 / 2.0;
const float cos120 = -0.5;
const float tan60 = sqrt3;
const float sin30 = 0.5;
const float tan30 = 1 / sqrt3;

//Joystick inputs
float joyX, joyY;
int joyBtn;

//Position
float x, y, z;

//Servo objects
Servo s1;
Servo s2;
Servo s3;

//Servo Angles
float ang1, ang2, ang3;

//0 = valid point, -1 = non existing point
int valid = 0;

//inverse kinematics
//helper functions, calculates angle theta1 (for YZ-pane)
int delta_calcAngleYZ(float x0, float y0, float z0, float &theta)
{
  float y1 = -0.5 * tan30 * f;  //calculate motor shaft position from base center
  y0 -= 0.5 * tan30 * e;        //calculate end effector attachment position from center
                   
  float a = (x0*x0 + y0*y0 + z0*z0 + rf*rf - re*re - y1*y1) / (2 * z0);
  float b = (y1 - y0) / z0;
  float d = -(a + b*y1)*(a + b*y1) + rf*(b*b*rf + rf);//discriminant
  if (d < 0) return -1;                               //non-existing point
  float yj = (y1 - a*b - sqrt(d)) / (b*b + 1);        //choosing outer point
  float zj = a + b*yj;                                //z = a + b*y
  theta = 180.0*atan(-zj / (y1 - yj)) / pi + ((yj>y1) ? 180.0 : 0.0);
  return 0;
}

//inverse kinematics: (x0, y0, z0) -> (theta1, theta2, theta3)
//returned status: 0=OK, -1=non-existing position
int delta_calcInverse(float x0, float y0, float z0, float &theta1, float &theta2, float &theta3)
{
  theta1 = theta2 = theta3 = 0;
  int stat = delta_calcAngleYZ(x0, y0, z0, theta1);
  if (stat == 0) stat = delta_calcAngleYZ(x0*cos120 + y0*sin120, y0*cos120 - x0*sin120, z0, theta2);  //rotate coords to +120 deg
  if (stat == 0) stat = delta_calcAngleYZ(x0*cos120 - y0*sin120, y0*cos120 + x0*sin120, z0, theta3);  //rotate coords to -120 deg
  return stat;
}

void setup()
{
  //PinModes für Joystick Input
  pinMode(joyPinX,INPUT);
  pinMode(joyPinY,INPUT);
  pinMode(joyPinBtn,INPUT_PULLUP);

  //Servos den Pins zuweisen
  s1.attach(servo1);
  s2.attach(servo2);
  s3.attach(servo3);

  //Ausgangsposition setzen
  x = defaultx;
  y = defaulty;
  z = maxz;

  //Serielle Schnittstelle aktivieren
  Serial.begin(250000);
}

void loop()
{
  //Joystick Inputs einlesen
  joyX = (analogRead(joyPinX)-501)/512.0;
  joyY = (analogRead(joyPinY)-499)/512.0;
  joyBtn = digitalRead(joyPinBtn);

  //Koordinaten anhand der Joystickposition ändern
  x += joyX*spd;
  y += joyY*spd;
  z += (joyBtn - !joyBtn)*spd;

  //Bereich begrenzen
  x = constrain(x,-100,100);
  y = constrain(y,-100,100);
  z = constrain(z,minz,maxz);

  //Winkel aus Koordinaten berechnen
  valid = delta_calcInverse(x, y, z, ang1, ang2, ang3);
  //Offset +90°, weil der Bereich der Servos 0 - 180° und nicht -90 - 90° ist
  ang1 += 90;
  ang2 += 90;
  ang3 += 90;

  //Debug Output
  /*
  Serial.print("\t1: ");
  Serial.print(x);
  Serial.print("\t2: ");
  Serial.print(y);
  Serial.print("\t3: ");
  Serial.println(z);
  */

  //Winkel an die Servos senden
  if(valid == 0)
  {
    s1.write(ang1);
    s2.write(ang2);
    s3.write(ang3);
  }
}
