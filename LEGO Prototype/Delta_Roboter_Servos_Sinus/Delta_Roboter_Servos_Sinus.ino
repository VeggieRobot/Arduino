#include <Servo.h>

int servo1 = 3;
int servo2 = 5;
int servo3 = 6;

Servo s1;
Servo s2;
Servo s3;

float t = 0;

float spd = 0.02;
int defaultang = 100;
int angle = 30;

float ang1,ang2,ang3;

void setup()
{
  s1.attach(servo1);
  s2.attach(servo2);
  s3.attach(servo3);

  s1.write(defaultang);
  s2.write(defaultang);
  s3.write(defaultang);
  
  Serial.begin(9600);
}

void loop()
{
  ang1 = defaultang + (float)angle*sin(t);
  ang2 = defaultang + (float)angle*sin(t+2);
  ang3 = defaultang + (float)angle*sin(t+4);

  s1.write(ang1);
  s2.write(ang2);
  s3.write(ang3);
  
  t += spd;
  delay(10);
}
