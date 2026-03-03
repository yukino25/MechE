#include "Arduino.h"
#include <CreatingLibraries.h>


Myfoods::Myfoods(){
  //add code to initialize variables
}

void Myfoods::begin(){
  Serial.begin(9600);
  //add more set up code
}

void Myfoods::getFruit(char c, char sz){
  //add code to return fruit based on color and size
}

void Myfoods::getVegetable(char c, char sz){
  //add code to return vegetable based on color and size
}

void Myfoods::getStarch(char c, char sz){
  //add code to return starch based on color and size
}

void Myfoods::printFoods(){
  //add code to print out foods
}

