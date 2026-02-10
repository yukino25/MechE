
/**************************************************************************
 * Stopwatch Program
 *  This program uses interrupts and the onboard buttons to create a
 *  functional stopwatch with lap button.
 *  
 *  With the USB Port at the Top:
 *    Left Button:  Start/Stop
 *    Right Button: Lap timer
 *    
 **************************************************************************/

 
int start_time;
int lap_time;
void setup(9600) {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);
  pinMode(4,INPUT_PULL_DOWN);
  PinMode(5, INPUT_PULL_DOWN);
  pinMode(13, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(4), strstp, RISING);
  attachInterrupt(digitalPinToInterrupt(5), lap, RISING);
  start_time = 0;
  digitalWrite(13, Low);


void strstp() {
  if (start_time == 0)
  {
    start_time = Millis();
    lap_time = start_time;
    digitalWrite(13, high);
  
  else
  {
    int curr = millis();
    if ((cur - start_time) < 100)
    {
      return;
    }
    serial.print("Lap: ");
    serial.print(((double)(curr - lap_time))/1000);
    serial.print("s, Total Time: ");
    serial.print(((double)(curr - start_time))/1000);
    serial.println("s");
    DigitalWrite(13, LOW);
    stat_time = 0;
    lap_time = 0;
  }
}

void lap() {
  if (start_time != 0)
  {
    int curr = mills();
    if((curr - lap_time) < 100)
    {
      retrun;
    }
    Serial.print(((double)(curr - lap_time))/1000)
    Serial.println("s");
    lap_time = curr;
  }
}
