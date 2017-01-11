// robot geometry
// (look at pics above for explanation)
const float e = 50.0;     //end effector radius
const float f = 200.0;    //base radius
const float re = 660.0;   //forearm length
const float rf = 700.0;   //bicep length

// trigonometric constants
const float sqrt3 = sqrt(3.0);
const float pi = 3.141592653;    // PI
const float sin120 = sqrt3 / 2.0;
const float cos120 = -0.5;
const float tan60 = sqrt3;
const float sin30 = 0.5;
const float tan30 = 1 / sqrt3;


// inverse kinematics
// helper functions, calculates angle theta1 (for YZ-pane)
int delta_calcAngleYZ(float x0, float y0, float z0, float &theta) {
  float y1 = -0.5 * tan30 * f;  //f/2 * tg 30
  y0 -= 0.5 * tan30 * e;      //shift center to edge

  float a = (x0*x0 + y0*y0 + z0*z0 + rf*rf - re*re - y1*y1) / (2 * z0);
  float b = (y1 - y0) / z0;
  float d = -(a + b*y1)*(a + b*y1) + rf*(b*b*rf + rf);//discriminant
  if (d < 0) return -1;               //non-existing point
  float yj = (y1 - a*b - sqrt(d)) / (b*b + 1);    //choosing outer point
  float zj = a + b*yj;                //z = a + b*y
  theta = 180.0*atan(-zj / (y1 - yj)) / pi + ((yj>y1) ? 180.0 : 0.0);
  return 0;
}

// inverse kinematics: (x0, y0, z0) -> (theta1, theta2, theta3)
// returned status: 0=OK, -1=non-existing position
int delta_calcInverse(float x0, float y0, float z0, float &theta1, float &theta2, float &theta3) {
  theta1 = theta2 = theta3 = 0;
  int stat = delta_calcAngleYZ(x0, y0, z0, theta1);
  if (stat == 0) stat = delta_calcAngleYZ(x0*cos120 + y0*sin120, y0*cos120 - x0*sin120, z0, theta2);  //rotate coords to +120 deg
  if (stat == 0) stat = delta_calcAngleYZ(x0*cos120 - y0*sin120, y0*cos120 + x0*sin120, z0, theta3);  //rotate coords to -120 deg
  return stat;
}

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  float x, y, z;
  float ang1, ang2, ang3;
  int starttime = 0;
  int timediff = 0;

  randomSeed(123456789);

  starttime = micros();

  for (int i = 0; i < 1000; i++)
  {
    x = random(-300,300);
    y = random(-300,300);
    z = random(-500,-400);
    
    delta_calcInverse(x, y, z, ang1, ang2, ang3);
  }
  timediff = micros() - starttime;
  Serial.println(timediff/1000.0);
}
