/*
    Functions are used to run lines of code without repeatedly writing out every line. if a 100 line program needs to run
    10 times with different variables it would be much faster and simpler to create the function with the variables as inputs and simply
    call the function 10 times with each of the variables. using functions makes code more readable and quicker to develop.
*/


#include <Adafruit_Circuit_Playground.h>
#include <Adafruit_CircuitPlayground.h>


float ZArray[100];
float LightArray[100];
float r_top = 0;
float r_bottom = 0;
float r = 0;
float SumZ = 0;
float SumLight = 0;
float AverageZ = 0;
float AverageLight = 0;
float Z_bottom = 0;
float Light_bottom = 0;

void setup() {
    Serial.begin(9600);
    while(!Serial);
    CircuitPlayground.begin();

}

void loop(){

    for(int i = 0; i < 100; i++){
        ZArray[i] = CircuitPlayground.motionZ();
        LightArray[i] = CircuitPlayground.lightSensor();
        delay(50);
    }
    PearsonCorrelationCoefficient(ZArray, LightArray);

}


void PearsonCorrelationCoefficient(float Z[], float Light[]){
    Average();
    for(int i = 0; i < 100; i++){
        r += (Z[i] - AverageZ) * (Light[i] - AverageLight);
    }
    for(int i = 0; i < 100; i++){
         Z_bottom += pow(Z[i] - AverageZ, 2);
         Light_bottom += pow(Light[i] - AverageLight, 2);
    }
    r_bottom = sqrt(Z_bottom * Light_bottom);
    r = r_top/r_bottom;

    Serial.print(("Pearson Correlation Coefficient: "));
    Serial.println(r);
    AverageLight = 0;
    AverageZ = 0;
    SumLight = 0;
    SumZ = 0;
    r = 0;

}

void Average(){
    for(int i = 0; i < 100; i++){
        SumZ += ZArray[i];
        SumLight += LightArray[i];
    }
    AverageZ = SumZ/100;
    AverageLight = SumLight/100;
}