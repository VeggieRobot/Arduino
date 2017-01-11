#include <Servo.h>

//Servo pin numbers
#define servo1 3
#define servo2 5
#define servo3 6

//Default position at startup
#define defaultx 0
#define defaulty 0
#define defaultz -100

//Servo objects
Servo s1;
Servo s2;
Servo s3;

// robot geometry
const float e = 40.0;     //end effector radius
const float f = 70.0;     //base radius
const float re = 185.0;   //forearm length
const float rf = 100.0;   //bicep length

// trigonometric constants
const float sqrt3 = sqrt(3.0);
const float pi = 3.141592653;
const float sin120 = sqrt3 / 2.0;
const float cos120 = -0.5;
const float tan60 = sqrt3;
const float sin30 = 0.5;
const float tan30 = 1 / sqrt3;

//Target position
float tx, ty, tz;

//Actual position
float x, y, z;

//Range from actual position to target position
float rangex,rangey,rangez;

//Servo Angles
float ang1, ang2, ang3;

//Amount of Samples per move
int samples = 0;

//Time variable
int t = 65535;

//0 = valid point, -1 = non existing point
int valid = 0;

// inverse kinematics
// helper functions, calculates angle theta1 (for YZ-pane)
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

// inverse kinematics: (x0, y0, z0) -> (theta1, theta2, theta3)
// returned status: 0=OK, -1=non-existing position
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
  s1.attach(servo1);
  s2.attach(servo2);
  s3.attach(servo3);
  
  x = defaultx;
  y = defaulty;
  z = defaultz;
  tx = defaultx;
  ty = defaulty;
  tz = defaultz;
  rangex = 0;
  rangey = 0;
  rangez = 0;
  Serial.begin(250000);
}

void loop()
{
  while(Serial.available() > 0)
  {
    tx = Serial.readStringUntil(':').toInt();
    ty = Serial.readStringUntil(':').toInt();
    tz = Serial.readString().toInt();

    valid = delta_calcInverse(tx, ty, tz, ang1, ang2, ang3);

    if(valid == 0)
    {
      Serial.print("x: ");
      Serial.print(tx);
      Serial.print("\ty: ");
      Serial.print(ty);
      Serial.print("\tz: ");
      Serial.println(tz);
  
      rangex = tx - x;
      rangey = ty - y;
      rangez = tz - z;
      
      samples = sqrt(rangex*rangex + rangey*rangey + rangez*rangez);
      
      t = 0;
    }
    else
    {
      Serial.println("ERROR: Target coordinates out of area!");
      tx = x;
      ty = y;
      tz = z;
    }
    valid = 0;
  }
  if(rangex != 0) {x = tx-rangex+calcAng(rangex);}
  if(rangey != 0) {y = ty-rangey+calcAng(rangey);}
  if(rangez != 0) {z = tz-rangez+calcAng(rangez);}

  valid = delta_calcInverse(x, y, z, ang1, ang2, ang3);
  ang1 += 90;
  ang2 += 90;
  ang3 += 90;

  if(valid == 0)
  {
    /*
    Serial.print("t: ");
    Serial.println(t);
    Serial.print("\t1: ");
    Serial.print(x);
    Serial.print("\t2: ");
    Serial.print(y);
    Serial.print("\t3: ");
    Serial.println(z);
    */
    
    s1.write(ang1);
    s2.write(ang2);
    s3.write(ang3);
    
    t++;
    t = constrain(t,0,samples);
  }
  else
  {
    t = samples;
    Serial.println("Error: Out of Area!");
  }
  
  delay(10);
}

float calcAng(float range)
{
  float val = (range/pi)*((pi*t)/samples - cos((pi*t)/samples) * sin((pi*t)/samples));
  return val;
}
