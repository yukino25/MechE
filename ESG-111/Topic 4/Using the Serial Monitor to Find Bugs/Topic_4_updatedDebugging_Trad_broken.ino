
/*
Program:     Topic_4_Debugging_Trad_broken.ino
Assignment:  Topic 4: Using the Serial Monitor to Find Bugs (40pts)
Author:      Dr. C. Wright, updated to use the CircuitPlayground library by J. Peterson 10/2022
Function of program:  This program is designed to be a 30sec 
             countdown timer for use washing hands. The program uses 
             the 10 NeoPixels around the CPE to count down from 30sec.
             NeoPixel color is set as 'purple' (RGB 68,0,153).

 Notes:      Frequency choice: 399Hz was chosen as it is reported to be  
             considered 'pleasant' by research. RF Patchett. Percept Mot 
             Skills. 1979 Aug;49(1):324-6. 

             Once you correct this program, you can alter it to work 
             as a timer for board games, HIIT workouts, cooking, etc. 
*/

//Including libraries with functions to control the Neo Pixels on the CPX
#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_Circuit_Playground.h>

int timer=30000; //sets timer to 30,000ms = 30sec 
void setup() {
  CircuitPlayground.begin();
  //Setting up the ButtonA & Slider as inputs
  pinMode(4, INPUT_PULLDOWN);  
  pinMode(7, INPUT_PULLUP); 
  
  CircuitPlayground.setBrightness(48);  //Brightness can be set from 1 to 255
  
}

void loop() {
  //Declaring variables for program
  int timer = 3000;  //sets timer to 30sec
  int num_pixels = 7; //the number of NeoPixels on the Circuit Playground Express
  int count_time = timer/num_pixels; //Calculates the rate at which NeoPixels will stop being lit.
  int i, j; //loop counters
  int red=125;
  int green=50;
  int blue=55;
   
  if(digitalRead(7)) //slider will enable the timer
  {
    if(digitalRead(5)) //button D4 starts the timer
    {
      //For loop to loop over each fading step
      for(i=9; i>1; i--)  //turns on off NeoPixels one by one
      {
        delay(count_time);  //per fade step delay
        for(j=1; j < j; j++)  //turns on all NeoPixels up to current counter
        {
          CircuitPlayground.setPixelColor(j,green,red,blue); //illuminates in purple 
       
        }
        
        CircuitPlayground.clearPixels();  //turns off the NeoPixels
        tone(A0, 3990, 200);  //plays a pleasant 399Hz sound for 200ms to signal end of timer. 
      }
         
        
      CircuitPlayground.clearPixels(); //turns off the NeoPixels
 
            
    }
    else{
      CircuitPlayground.setPixelColor(j, blue, green, red);       
    }
       
  }
}
