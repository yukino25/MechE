

/*Finding Voltage. To compute the voltage of an analog port, you must use a conversion which is
based on the voltages range of the port, and the number of bits the analog-to-digital converter
(ADC) produces. The formula is stated as:
q=Vsfr/2^n
q= step size in volts
Vsfr = full scale voltage range of the port
n = number of bits the ADC produces

Most arduinos can measure 0-5V on 10-bit ADC. Voltage calculated as:
Vout=q*M=Vsfr/2^n*M=5-0/2^10*M=5/1024*M
M = measurement from arduino


Adafruit circuit playground express is a 3.3V system with a 10-bit ADC. Voltage calculated as:
Vout=q*M=Vsfr/2^n*M=3.3-0/2^10*M=3.3/1024*M
M = measurement from arduino


the thermistor is Rt to 3.3v with a 10kΩ pulldown resistor, Vout is the midpoint between the two.
Rtotal = Rt + 10kΩ

Rt= 33000[v*Ω]/Vout - 10kΩ


Calculating Temperature with NCP15XH103F03RC by Murata:
R(25C)=10kΩ
B(25/50)=3380K

equation:
T[C]=B/ln(Rt/(R0*e^(-B/T0)))
T0 = 25C
R0 = 10kΩ
B = 3380K

T[F]=T[C]*1.8 + 32

N=4*T[F]-160
N = number of chirps per minute


Adafruit circuit playground express was used
*/

#define TempPin A9
#define Speaker A0

float Vout;
float Rt;
float Temp_C;
float Temp_F;
float Chirp_Min;
int minute = 60000;

void setup() {
    Serial.begin(9600);
    pinMode(TempPin, INPUT);
    pinMode(Speaker, OUTPUT);
}


void loop(){
    //voltage of the thermistor
    Vout = 3.3/1024*analogRead(TempPin);
    //reistance of the thermistor
    Rt = 33000/Vout - 10000;
    //temperature in celcius and fahrenheit
    Temp_C = 3380/log(Rt/(10000*exp(-3380/298.15)));
    Temp_F = Temp_C*1.8 + 32;

    Chirp_Min = 4*Temp_F - 160;
    tone(Speaker, 1000, 100);
    //delay between chiprs to produce the correct number of chirps per minute
    delay(minute/Chirp_Min);
}