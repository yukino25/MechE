/*
A moving average is used to smooth data and reduce the noise within a signal. 
In a moving average operation, a window size is chosen and passed through the data, averaging within each window. 
For example, if we are using a window size of 10, the first window will start on element 1 and average elements 1-10; 
the next window will start on element 2 and will average elements 2-11; and so on through all elements. 
At the end, you will lose a few elements due to the window overlapping the end; this is called a boundary, and can be handled many ways. 
For this assignment, we will just ignore it and remove the elements that require values beyond N.

Directions: For this program, students will be using the built-in light sensor (A8).

1.Write a program that can measure 100 samples of the light sensor with 100 ms between each sample.

2.Use the serial plotter to plot the data before and after a 10-element moving average.

3.Try different window sizes and observe the differences.


Submit your code and a graphical representation of the algorithm

Adafruit Circuit Playground Express was used
*/

#define LightSensor A8
#define WindowSize 10
int LightData[100] = {};
void setup(){
    Serial.begin(9600);
    pinMode(LightSensor, INPUT);
}

void loop(){

    for(int i = 0; i < 100; i++){
        LightData[i] = analogRead(LightSensor);
        if(i >= WindowSize - 1){
            int sum = 0;
            for(int j = i - WindowSize + 1; j <= i; j++){
                sum += LightData[j];
            }
            Serial.print("Average:");
            Serial.print(sum / WindowSize);
            Serial.print("Raw Data:");
            Serial.println(LightData[i]);
        }
        delay(100);
    }
    for(;;);
}
