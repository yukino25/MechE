int start_time;
int lap_time;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(13, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(4), strstp, RISING);
  attachInterrupt(digitalPinToInterrupt(5), lap, RISING);
  start_time = 0;
  digitalWrite(13, LOW);
}

void strstp() {
  if (start_time == 0)
{
    start_time = millis();
    lap_time = start_time;
    digitalWrite(13, HIGH);
}
  else
  {
    int curr = millis();
    if ((curr - start_time) < 100)
    {
      return;
    }
    Serial.print("Lap: ");
    Serial.print(((double)(curr - lap_time))/1000);
    Serial.print("s, Total Time: ");
    Serial.print(((double)(curr - start_time))/1000);
    Serial.println("s");
    digitalWrite(13, LOW);
    start_time = 0;
    lap_time = 0;
  }
}


void lap() {
  if (start_time != 0)
  {
    int curr = millis();
    if((curr - lap_time) < 100)
    {
      return;
    }
    Serial.print(((double)(curr - lap_time))/1000);
    Serial.println("s");
    lap_time = curr;
  }
}

void loop(){}
