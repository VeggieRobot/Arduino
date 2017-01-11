float num1 = 0;
float num2 = 0;

void setup()
{
  pinMode(0,INPUT_PULLUP);
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop()
{
  if(Serial.available() > 0)
  {
    num1 = Serial.parseFloat();
    if(num1 != 0)
    {
      Serial1.println(num1,4);
      num1 = 0;
    }
  }
  if(Serial1.available() > 0)
  {
    num2 = Serial1.parseFloat();
    if(num2 != 0)
    {
      Serial.println(num2,4);
      num2 = 0;
    }
  }
}
