#include "Arduino.h"
#include <CreatingLibraries.h>


Myfoods::Myfoods(){
  //add code to initialize variables
    char Response;
    char c;
    char sz;
    bool spacefound = false;


}

void Myfoods::begin(){
    // set up code
    Serial.setTimeout(10000); // set timeout for serial input to 10 seconds
    Serial.println("Enter a color and size (sm, med, lg) separated by a space to get a food suggestion");
    Serial.println("color options: red, orange, yellow, green, blue, purple, brown, white");
}

//read input from serial monitor, call function from library.
void Myfoods::readFoods(){
    while(!Serial.available()){
      //wait for input
    }

         Response = Serial.read();
         //go through response and split into color and size variables where color is before space and size is after space
         for (int i = 0; i < length(Response); i++){
            // at the space, split the response into color and size variables
              if (Response[i] == ' '){
                spacefound = true;
                // loop through response and add characters to corresponding variables
                for (int j = 0; j < i; j++){
                  c += Response[j];
                }
                for (int k = i+1; k < length(Response); k++){
                  sz += Response[k];
                }
              }
         }
         if (!spacefound){
           Serial.println("Invalid input, please enter color and size separated by a space");
           //return to beginning of loop to wait for new input
           return;
           // reset variables to be ready for next input
            Response = "";
            c = "";
            sz = "";
            spacefound = false;
         }
         else{
           // continue to let functions determine food based on color and size in ino file
         }
}


void Myfoods::getFruit(){
    //! add code to return fruit based on color and size
    //! make array or class of fruits with different colors and sizes, then use random function to return a different fruit each time with same input
    // read food needs to run before this function, if function is called before read food, return error message to serial monitor and return to beginning of loop to wait for new input
    if (c == "" || sz == ""){
        Serial.println("readFoods function must be called before getFruit function");
        //return to beginning of loop to wait for new input
        return;
    }else{
        // continue to let functions determine food based on color and size in ino file
        // at end of function reset variables to be ready for next input
        Response = "";
        c = "";
        sz = "";
        spacefound = false;
    }
}

void Myfoods::getVegetable(){
    //!add code to return vegetable based on color and size
    //! make array or class of vegetables with different colors and sizes, then use random function to return a different vegetable each time with same input
    // read food needs to run before this function, if function is called before read food, return error message to serial monitor and return to beginning of loop to wait for new input
    if (c == "" || sz == ""){
        Serial.println("readFoods function must be called before getVegetable function");
        //return to beginning of loop to wait for new input
        return;
    }else{
        // continue to let functions determine food based on color and size in ino file
        // at end of function reset variables to be ready for next input
        Response = "";
        c = "";
        sz = "";
        spacefound = false;
    }
}

void Myfoods::getStarch(){
    //! add code to return starch based on color and size
    //! make array or class of starches with different colors and sizes, then use random function to return a different starch each time with same input
    // read food needs to run before this function, if function is called before getStarch function, return error message to serial monitor and return to beginning of loop to wait for new input
    if (c == "" || sz == ""){
        Serial.println("readFoods function must be called before getStarch function");
        return;
    }else{
        // continue to let functions determine food based on color and size in ino file
        // at end of function reset variables to be ready for next input
        Response = "";
        c = "";
        sz = "";
        spacefound = false;
    }
}



void Myfoods::printFoods(){
  //add code to print out foods
}

