#define dirPin 2
#define clkPin 3

#define freq 50000
#define spt 98304

float tang = 0;
float ang = 0;

long steps = 0;

void setup()
{
  pinMode(dirPin, OUTPUT);
  pinMode(clkPin, OUTPUT);

  Serial.begin(250000);
}

void loop()
{
  while(Serial.available() > 0)
  {
    tang = Serial.parseFloat();
    
    if(tang != 0)
    {
      steps = tang-ang;
  
      ang = tang;
      
      if(steps < 0)
      {
        digitalWrite(dirPin, HIGH);
      }
      else
      {
        digitalWrite(dirPin, LOW);
      }
      steps = abs((long)steps*spt/360);
      
      for(int i=1; i<steps; i++)
      {
        digitalWrite(clkPin, HIGH);
        delayMicroseconds((long)500000/freq);
        digitalWrite(clkPin, LOW);
        delayMicroseconds((long)500000/freq);
      }
    }
  }
}
