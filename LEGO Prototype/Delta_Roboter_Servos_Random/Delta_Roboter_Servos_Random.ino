#include <Servo.h>

//Servo pin numbers
#define servo1 3
#define servo2 5
#define servo3 6

//Default angle for all Servos at startup
#define defaultang 70

//Min and max angles for Servos
#define minang 50
#define maxang 170

//Amount of Samples per move
int samples = 0;

//Servo objects
Servo s1;
Servo s2;
Servo s3;

//Time variable
int t = 65535;

//Target angles
float tang1,tang2,tang3;

//Actual angles
float ang1,ang2,ang3;

//Range from actual angle to target angle
float range1,range2,range3;

//Pi constant
#define pi 3.141592654

//Current time since start in milliseconds
int curtime = 0;

//Last move
int lasttime = 0;

//Interval between moves
int interval = 2000;

//Samples per distance (higher values = slower but more accurate movement)
float samplemul = 1;

void setup()
{
  s1.attach(servo1);
  s2.attach(servo2);
  s3.attach(servo3);
  
  s1.write(defaultang);
  s2.write(defaultang);
  s3.write(defaultang);
  
  ang1 = defaultang;
  ang2 = defaultang;
  ang3 = defaultang;
  tang1 = defaultang;
  tang2 = defaultang;
  tang3 = defaultang;
  range1 = 0;
  range2 = 0;
  range3 = 0;

  randomSeed(12345);
  
  Serial.begin(9600);
}

void loop()
{
  curtime = millis();
  
  //if(curtime-lasttime > interval)
  if(t >= samples)
  {
    tang1 = random(minang,maxang);
    tang2 = random(minang,maxang);
    tang3 = random(minang,maxang);
    
    range1 = tang1-ang1;
    range2 = tang2-ang2;
    range3 = tang3-ang3;    
    
    Serial.print("\t1: ");
    Serial.print(tang1);
    Serial.print("\t2: ");
    Serial.print(tang2);
    Serial.print("\t3: ");
    Serial.println(tang3);

    samples = (abs(range1)+abs(range2)+abs(range3))*samplemul;
    Serial.println(samples);
    
    t = 0;
    lasttime = curtime;
  }
  
  if(range1 != 0) {ang1 = tang1-range1+calcAng(range1);}
  if(range2 != 0) {ang2 = tang2-range2+calcAng(range2);}
  if(range3 != 0) {ang3 = tang3-range3+calcAng(range3);}
  
  //Serial.print("t: ");
  //Serial.println(t);
  //Serial.print("\t1: ");
  //Serial.print(ang1);
  //Serial.print("\t2: ");
  //Serial.print(ang2);
  //Serial.print("\t3: ");
  //Serial.println(ang3);
  
  s1.write(ang1);
  s2.write(ang2);
  s3.write(ang3);

  t++;
  t = constrain(t,0,samples);
  
  delay(10);
}
float calcAng(float range)
{
  float x = (range/pi)*((pi*t)/samples - cos((pi*t)/samples) * sin((pi*t)/samples));
  return x;
}
