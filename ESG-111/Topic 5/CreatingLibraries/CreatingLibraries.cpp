#include <Arduino.h>
#include <String.h>
#include "CreatingLibraries.h"


Myfoods::Myfoods(){
  //add code to initialize variables
    Response[0] = '\0';
    c[0] = '\0';
    sz[0] = '\0';
    spacefound = false;
    colorfound = false;
    sizefound = false;
    char temp_color[8][8] = {"red", "orange", "yellow", "green", "blue", "purple", "brown", "white"};
    memcpy(color, temp_color, sizeof(temp_color));
    char temp_size[3][4] = {"sm", "med", "lg"};
    memcpy(size, temp_size, sizeof(temp_size));
    color_val = 0;
    size_val = 0;
    food[0] = '\0';
    //[color][size][2 options for each color and size combination]
    char temp_fruit[8][3][2][20] = {
        {{"strawberry", "cherry"}, {"watermelon", "raspberry"}, {"pomegranate", "red apple"}},
        {{"mandarin", "apricot"}, {"orange", "cantaloupe"}, {"papaya", "mango"}},
        {{"lemon", "starfruit"}, {"banana", "pineapple"}, {"yellow pear", "quince"}},
        {{"green grape", "kiwi"}, {"green apple", "lime"}, {"honeydew", "avocado"}},
        {{"blueberry", "blackberry"}, {"bilberry", "blue plum"}, {"blueberry pie", "elderberry"}},
        {{"purple grape", "plum"}, {"purple fig", "boysenberry"}, {"passion fruit", "damson plum"}},
        {{"date", "brown pear"}, {"brown fig", "tamarind"}, {"breadfruit", "jackfruit"}},
        {{"white peach", "lychee"}, {"white grape", "white nectarine"}, {"white fig", "longan"}}
    };
    memcpy(fruit, temp_fruit, sizeof(temp_fruit));
    char temp_vegetable[8][3][2][20] = {
        {{"tomato", "red pepper"}, {"red onion", "radish"}, {"red cabbage", "beet"}},
        {{"carrot", "orange pepper"}, {"sweet potato", "pumpkin"}, {"butternut squash", "acorn squash"}},
        {{"yellow pepper", "corn"}, {"yellow squash", "yellow beet"}, {"yellow potato", "yellow onion"}},
        {{"green beans", "peas"}, {"broccoli", "cucumber"}, {"spinach", "kale"}},
        {{"blue corn", "blue potato"}, {"sea beans", "blue pea"}, {"blue lotus root", "blue pea flower"}},
        {{"eggplant", "purple cabbage"}, {"purple carrot", "purple onion"}, {"purple potato", "purple beet"}},
        {{"mushroom", "brown onion"}, {"shiitake", "portobello"}, {"brown potato", "taro"}},
        {{"cauliflower", "daikon"}, {"white onion", "celery"}, {"white potato", "parsnip"}}
    };
    memcpy(vegetable, temp_vegetable, sizeof(temp_vegetable));
    char temp_starch[8][3][2][20] = {
        {{"red potato", "red quinoa"}, {"red lentils", "red beans"}, {"red rice", "red pasta"}},
        {{"sweet potato", "orange yam"}, {"pumpkin bread", "orange squash"}, {"acorn squash", "orange grits"}},
        {{"yellow potato", "corn"}, {"yellow lentils", "yellow beans"}, {"yellow rice", "polenta"}},
        {{"green potato", "edamame"}, {"green lentils", "green peas"}, {"green rice", "green pasta"}},
        {{"blue corn", "blue potato"}, {"blue corn tortilla", "blue grits"}, {"blue hominy", "blue quinoa"}},
        {{"purple potato", "purple yam"}, {"purple lentils", "purple rice"}, {"purple beans", "taro"}},
        {{"brown rice", "brown quinoa"}, {"brown lentils", "wheat berries"}, {"brown bread", "rye bread"}},
        {{"white rice", "white potato"}, {"white beans", "white pasta"}, {"white bread", "white grits"}}
    };
    memcpy(starch, temp_starch, sizeof(temp_starch));
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

         Serial.readBytesUntil('\n', Response, sizeof(Response));
         //go through response and split into color and size variables where color is before space and size is after space
         for (int i = 0; i < strlen(Response); i++){
            // at the space, split the response into color and size variables
              if (Response[i] == ' '){
                spacefound = true;
                // loop through response and add characters to corresponding variables
                for (int j = 0; j < i; j++){
                    c[j] = Response[j];
                    c[j+1] = '\0'; // add null terminator to end of color string
            
                }
                for (int k = i+1; k < strlen(Response); k++){
                    sz[k-(i+1)] = Response[k];
                    sz[k-i] = '\0'; // add null terminator to end of size string
                }
              }
         }
         if (!spacefound){
           Serial.println("Invalid input, please enter color and size separated by a space");

           // reset variables to be ready for next input
            Response[0] = '\0';
            c[0] = '\0';
            sz[0] = '\0';
            color_val = 0;
            size_val = 0;
            spacefound = false;
            food[0] = '\0';
            //return to beginning of loop to wait for new input
            return;
         }
         else{
           // continue to let functions determine food based on color and size in ino file
         }
}


void Myfoods::getFruit(){
    // read food needs to run before this function, if function is called before read food, return error message to serial monitor and return to beginning of loop to wait for new input
    if (c[0] == '\0' || sz[0] == '\0'){
        Serial.println("readFoods function must be called before getFruit function");
        //return to beginning of loop to wait for new input
        return;
    }else{
        compareFoods();
        //assign food variable to fruit array value based on color and size values determined in compareFoods function, use random function to return different result with same input
        strcpy(food, fruit[color_val][size_val][random(0,2)]);
        // continue to let functions determine food based on color and size in ino file

    }
}

void Myfoods::getVegetable(){
    // read food needs to run before this function, if function is called before read food, return error message to serial monitor and return to beginning of loop to wait for new input
    if (c[0] == '\0' || sz[0] == '\0'){
        Serial.println("readFoods function must be called before getVegetable function");
        //return to beginning of loop to wait for new input
        return;
    }else{
        compareFoods();
        //assign food variable to vegetable array value based on color and size values determined in compareFoods function, use random function to return different result with same input
        strcpy(food, vegetable[color_val][size_val][random(0,2)]);
    }
}

void Myfoods::getStarch(){
    // read food needs to run before this function, if function is called before getStarch function, return error message to serial monitor and return to beginning of loop to wait for new input
    if (c[0] == '\0' || sz[0] == '\0'){
        Serial.println("readFoods function must be called before getStarch function");
        return;
    }else{
        compareFoods();
        //assign food variable to starch array value based on color and size values determined in compareFoods function, use random function to return different result with same input
        strcpy(food, starch[color_val][size_val][random(0,2)]);
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

    Response[0] = '\0';
    c[0] = '\0';
    sz[0] = '\0';
    color_val = 0;
    size_val = 0;
    spacefound = false;
    food[0] = '\0';

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