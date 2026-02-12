#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_Circuit_Playground.h>

int timer=30000; //sets timer to 30,000ms = 30sec
void setup() {
  CircuitPlayground.begin();
  //Setting up the ButtonA & Slider as inputs
  pinMode(4, INPUT_PULLDOWN);
  pinMode(7, INPUT_PULLUP);
  CircuitPlayground.setBrightness(48); //Brightness can be set from 1 to 255
}

void loop() {
  //Declaring variables for program
  int timer = 30000; //sets timer to 30sec
  int num_pixels = 10; //the number of NeoPixels on the Circuit Playground Express
  int count_time = timer/num_pixels; //Calculates the rate at which NeoPixels will stop being lit.
  int i, j; //loop counters
  int red=68;
  int green=0;
  int blue=153;
  if(digitalRead(7)) //slider will enable the timer
  {
    if(digitalRead(4)) //button D4 starts the timer
    {
      //For loop to loop over each fading step
      for(i=9; i>=0; i--) //turns on off NeoPixels one by one
      {

        for(j=0; j <= i; j++) //turns on all NeoPixels up to current counter
        {
          CircuitPlayground.setPixelColor(j,red,green,blue); //illuminates in purple
        }
        delay(count_time); //per fade step delay
        CircuitPlayground.clearPixels(); //turns off the NeoPixels
        
      }
      tone(A0, 399, 200); //plays a pleasant 399Hz sound for 200ms to signal end of timer.
      CircuitPlayground.clearPixels(); //turns off the NeoPixels
    }
    else{
      CircuitPlayground.setPixelColor(j, red, green, blue);
    }
  }
}