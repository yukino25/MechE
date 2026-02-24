/*
Your library should be called myfoods.

The myfoods library contains the following functions: getfruit, getvegetable, and getstarch.

Each function takes two variables, c (for color) and sz (for size), 
and returns a fruit, vegetable, or starch with the appropriate color and size when called.

Choose a variety of colors to support in your library, and at least three sizes: sm, med, and lg.

Include some randomness in your functions, so that a different item can be returned with repeated 
calls using the same parameters.


*/


//read input fron serial monitor, call function from library.
//have random math function to retun different result with same input.
//switch function in library

#include <CreatingLibraries.h>

char color[] = {};
char size[] = {};

void setup() {
  Serial.begin(9600);

}

void loop() {
    Serial.read();

}