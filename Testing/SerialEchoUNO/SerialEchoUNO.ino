float num = 0;

void setup()
{
  Serial.begin(9600);
}

void loop() 
{
  if(Serial.available() > 0)
  {
    num = Serial.parseFloat();
    if(num != 0)
    {
      Serial.println(num,4);
    }
  }
}
