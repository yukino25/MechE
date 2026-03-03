#include <Arduino.h>
#include <String.h>
#include "CreatingLibraries.h"


Myfoods::Myfoods(){
  //add code to initialize variables
    char Response[32];
    char c[8];
    char sz[4];
    bool spacefound = false;
    bool colorfound = false;
    bool sizefound = false;
    char color[8][8] = {"red", "orange", "yellow", "green", "blue", "purple", "brown", "white"};
    char size[3][4] = {"sm", "med", "lg"};
    int color_val;
    int size_val;
    char food[20];
    //[color][size][2 options for each color and size combination]
    char fruit[8][3][2][20] = {
        {{"strawberry", "cherry"}, {"watermelon", "raspberry"}, {"pomegranate", "red apple"}},
        {{"mandarin", "apricot"}, {"orange", "cantaloupe"}, {"papaya", "mango"}},
        {{"lemon", "starfruit"}, {"banana", "pineapple"}, {"yellow pear", "quince"}},
        {{"green grape", "kiwi"}, {"green apple", "lime"}, {"honeydew", "avocado"}},
        {{"blueberry", "blackberry"}, {"bilberry", "blue plum"}, {"blueberry pie", "elderberry"}},
        {{"purple grape", "plum"}, {"purple fig", "boysenberry"}, {"passion fruit", "damson plum"}},
        {{"date", "brown pear"}, {"brown fig", "tamarind"}, {"breadfruit", "jackfruit"}},
        {{"white peach", "lychee"}, {"white grape", "white nectarine"}, {"white fig", "longan"}}
    };
    char vegetable[8][3][2][20] = {
        {{"tomato", "red pepper"}, {"red onion", "radish"}, {"red cabbage", "beet"}},
        {{"carrot", "orange pepper"}, {"sweet potato", "pumpkin"}, {"butternut squash", "acorn squash"}},
        {{"yellow pepper", "corn"}, {"yellow squash", "yellow beet"}, {"yellow potato", "yellow onion"}},
        {{"green beans", "peas"}, {"broccoli", "cucumber"}, {"spinach", "kale"}},
        {{"blue corn", "blue potato"}, {"sea beans", "blue pea"}, {"blue lotus root", "blue pea flower"}},
        {{"eggplant", "purple cabbage"}, {"purple carrot", "purple onion"}, {"purple potato", "purple beet"}},
        {{"mushroom", "brown onion"}, {"shiitake", "portobello"}, {"brown potato", "taro"}},
        {{"cauliflower", "daikon"}, {"white onion", "celery"}, {"white potato", "parsnip"}}
    };
    char starch[8][3][2][20] = {
        {{"red potato", "red quinoa"}, {"red lentils", "red beans"}, {"red rice", "red pasta"}},
        {{"sweet potato", "orange yam"}, {"pumpkin bread", "orange squash"}, {"acorn squash", "orange grits"}},
        {{"yellow potato", "corn"}, {"yellow lentils", "yellow beans"}, {"yellow rice", "polenta"}},
        {{"green potato", "edamame"}, {"green lentils", "green peas"}, {"green rice", "green pasta"}},
        {{"blue corn", "blue potato"}, {"blue corn tortilla", "blue grits"}, {"blue hominy", "blue quinoa"}},
        {{"purple potato", "purple yam"}, {"purple lentils", "purple rice"}, {"purple beans", "taro"}},
        {{"brown rice", "brown quinoa"}, {"brown lentils", "wheat berries"}, {"brown bread", "rye bread"}},
        {{"white rice", "white potato"}, {"white beans", "white pasta"}, {"white bread", "white grits"}}
    };
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
         for (int i = 0; i < strlen(Response); i++){
            // at the space, split the response into color and size variables
              if (Response[i] == ' '){
                spacefound = true;
                // loop through response and add characters to corresponding variables
                for (int j = 0; j < i; j++){
                  c += Response[j];
                }
                for (int k = i+1; k < strlen(Response); k++){
                  sz += Response[k];
                }
              }
         }
         if (!spacefound){
           Serial.println("Invalid input, please enter color and size separated by a space");

           // reset variables to be ready for next input
            Response = "";
            c = "";
            sz = "";
            color_val = 0;
            size_val = 0;
            spacefound = false;
            food = "";
            //return to beginning of loop to wait for new input
            return;
         }
         else{
           // continue to let functions determine food based on color and size in ino file
         }
}


void Myfoods::getFruit(){
    // read food needs to run before this function, if function is called before read food, return error message to serial monitor and return to beginning of loop to wait for new input
    if (c == "" || sz == ""){
        Serial.println("readFoods function must be called before getFruit function");
        //return to beginning of loop to wait for new input
        return;
    }else{
        compareFoods();
        food = fruit[color_val][size_val][random(0,2)];
        // continue to let functions determine food based on color and size in ino file

    }
}

void Myfoods::getVegetable(){
    // read food needs to run before this function, if function is called before read food, return error message to serial monitor and return to beginning of loop to wait for new input
    if (c == "" || sz == ""){
        Serial.println("readFoods function must be called before getVegetable function");
        //return to beginning of loop to wait for new input
        return;
    }else{
        compareFoods();
        food = vegetable[color_val][size_val][random(0,2)];
    }
}

void Myfoods::getStarch(){
    // read food needs to run before this function, if function is called before getStarch function, return error message to serial monitor and return to beginning of loop to wait for new input
    if (c == "" || sz == ""){
        Serial.println("readFoods function must be called before getStarch function");
        return;
    }else{
        compareFoods();
        food = starch[color_val][size_val][random(0,2)];
    }
}


// print food suggestion to serial monitor & reset variables to be ready for next input
void Myfoods::printFoods(){
    Serial.print("Based on your inputs: color = ");
    Serial.print(c);
    Serial.print(" and size = ");
    Serial.print(sz);
    Serial.print(", we suggest you eat: ");
    Serial.println(food);

    Response = "";
    c = "";
    sz = "";
    color_val = 0;
    size_val = 0;
    spacefound = false;
    food = "";

}

//compare food inputs to food arrays to determine integer values for color and size to be used in getFruit, getVegetable, and getStarch functions
void Myfoods::compareFoods(){
    for(int i = 0; i < 8; i++){
        if(strcmp(c, color[i]) == 0){
            colorfound = true;
            color_val = i;
            break; 
        } 
    }
    if(!colorfound){
        Serial.println("Invalid color input, please enter a valid color");
        //return to beginning of loop to wait for new input
        return;
    }
    for(int j = 0; j < 3; j++){
        if(strcmp(sz, size[j]) == 0){
            sizefound = true;
            size_val = j;
            break; 
        } 
    }
    if(!sizefound){
        Serial.println("Invalid size input, please enter a valid size (sm, med, lg)");
        //return to beginning of loop to wait for new input
        return;
    }
}