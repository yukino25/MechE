/*
Automatic Door Lock that uses photoresistor to detect light in the room
when light level is low enough a timer is started, if light level is still low
after the timer expires the door is locked with a servo
if light level is above the threshold at any point for over 2 seconds the timer is reset 

Components:
- Arduino Pro Micro
- Photoresistor
- SG90 Servo
- 10k ohm resistor
- l7805 voltage regulator
- 100uF capacitor
- 10uF capacitor
- 2x 0.1uF capacitors



Potential additions:
- Blue LED to indicate light level is below threshold and timer is running
*/

#include <Servo.h>

Servo myservo; //create servo object to control the servo
#define SERVO_PIN 10
#define PHOTORESISTOR_PIN A0

const int lightThreshold = 500; // On/Off threshold for the photoresistor (0-1023)
const int lockDelay = 5000; // Time in milliseconds before locking the door after low light is detected
const int lockResetDelay = 2000; // Time in milliseconds to reset the lock timer if light goes above threshold

unsigned long lowLightStartTime = 0; // Time when low light was first
unsigned long lastHighLightTime = 0; // Time when high light was last detected
int lightLevel; //Analog value from the photoresistor (0-1023)


void setup(){
    Serial.begin(9600);
    pinMode(SERVO_PIN, OUTPUT);
    pinMode(PHOTORESISTOR_PIN, INPUT);
    myservo.attach(SERVO_PIN); //servo library to define the servo pin
}

void loop(){
    checkLightLevel();
}

void checkLightLevel() {
    lightLevel = analogRead(PHOTORESISTOR_PIN); // Read the light level from the photoresistor, ranege 0-1023
    Serial.print("Light Level: ");
    Serial.println(lightLevel);


    // Check if light level is below the threshold to consider it "dark"
    if (lightLevel < lightThreshold) {
        // If low light is detected and the timer hasn't started, start the timer
        if (lowLightStartTime == 0) {
            lowLightStartTime = millis(); // Start the timer
        // If the timer has started and the delay has passed, lock the door
        } else if (millis() - lowLightStartTime >= lockDelay) {
            lockDoor(); // function to lock the door with the servo
            lowLightStartTime = 0; // Reset the timer after locking
        }
    // If light level is above the threshold set last high time to current time
    } else {
        lastHighLightTime = millis(); // Update last high light time
        // If the light level goes above the threshold and the timer is running, reset the timer if it has been too long
        if(lowLightStartTime != 0 && millis() - lastHighLightTime >= lockResetDelay) {
            lowLightStartTime = 0; // Reset the timer if light goes above threshold for too long
            lastHighLightTime = 0; // Reset last high light time

        }
    }
}

void lockDoor(){
    // Servo movement code to lock the door
    myservo.write(90); //move servo to 90 deg angle, locking position
    delay(200);
    myservo.write(0); //move servo back to 0 deg angle, resting position
}