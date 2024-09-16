//Programmer Name: Saheel Siyam, Arnav Kothekar
//Project Name: Wall Avoiding Humanoid Robot (WAHR)aka 'Baymax'
//Code File Description: WAHR Master Arduino - controls LED Matrix, Ultrasonic Sensor and Servo Motors
//                       Sends signals to WAHR Slave Arduino which controls the 2 DC motors


#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#include "Communication.h"
//COMMUNICATION LIBRARY CREDIT GOES TO  THE LAST OUTPOST WORKSHOP
//https://dev.azure.com/overlording/The%20Last%20Outpost%20Workshop/_git/CommArduinoA?path=/arduinoA.ino
 
#define PIN 6
#define LED_NUM 40
#define FACE_SIZE 14

Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_NUM, PIN, NEO_GRB + NEO_KHZ800);

#define MASTER_ARDUINO_ADDRESS 10
#define SLAVE_ARDUINO_ADDRESS 20

static Communication Comm(MASTER_ARDUINO_ADDRESS);


Servo leftarm;  // create servo object to control a servo
Servo rightarm;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;
int increment = 2;

//happy is green, sad is red
int happyLEDs[14] = {1, 2, 5, 6, 9, 10, 13, 14, 25, 30, 34, 35, 36, 37};
int sadLEDs[14] = {1, 2, 5, 6, 9, 10, 13, 14, 26, 27, 28, 29, 33, 38};
int warnLEDs[14] = {1, 2, 5, 6, 9, 10, 13, 14, 25, 26, 27, 28, 29, 30};

const int trigPin = 9;
const int echoPin = 10;

float duration, distance;

//Face changes if there is a difference between the current state and the last state
bool isHappy = true;
bool isWarn = false;
bool lastHappy = true;

int stabilityCheck = 0;

byte tooClose;

void setup(){
  Serial.begin(9600);

  leds.begin(); // This initializes the NeoPixel library.

  Comm.Transmit(SLAVE_ARDUINO_ADDRESS, "START");
  Serial.print("MESS TRANSM");
  delay(1000);
  leftarm.attach(13);
  rightarm.attach(12);


  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  

  face(happyLEDs, 0, 10, 0);
  

  leftarm.write(90);
  rightarm.write(90);


}

void face(int pinlist[], int R, int G, int B){
  for(int i=0;i<14;i++){
    leds.setPixelColor(pinlist[i], leds.Color(R, G, B));
  }
  leds.show();
}
void led_set(int R, int G, int B)
{
for (int i = 0; i < LED_NUM; i++)
{
leds.setPixelColor(i, leds.Color(R, G, B));
leds.show();
//delay(50);
}
}

void loop(){

  switch (Comm.Received()){
    case STRING_MESSAGE:
      // Received a string message
      Serial.println(Comm.GetStringMessage());
      break;
    case INT_MESSAGE:
      Serial.println(Comm.GetIntMessage());
      break;
  }
 
  if(pos < 0){
    increment = 2;
   
  }else if (pos > 180){
    increment = -2;
    pos -= 2;
  }
  pos += increment;
 
  leftarm.write(pos);
  rightarm.write(pos);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(50);
  //delay(500);

//have an intermediate expression. Yellow LEDS with a straight mouth, if the SENSOR is triggered once 
  if(distance < 20.00){
    
    stabilityCheck ++;
    Serial.print("Stability Check ");
    Serial.println(stabilityCheck);

    if (stabilityCheck >= 25){
      //CODE 11 means 15 consecutive distance checks. Warn mode
      //lastHappy = true;
      isHappy = false;
      isWarn = false;
      faceChange();
      //led_set(0, 0, 0);
      //face(sadLEDs, 10, 0, 0);
      
      Comm.Transmit(SLAVE_ARDUINO_ADDRESS, 11);



    }else if (stabilityCheck >= 3){
      
      //CODE 1 means 3 consecutive distance checks
      isHappy = false;
      isWarn = true;
      //led_set(0, 0, 0);
      //face(warnLEDs, 10, 10, 0);
      Comm.Transmit(SLAVE_ARDUINO_ADDRESS, 1);

    }


  }else{
    //led_set(0, 0, 0);
    face(happyLEDs, 0, 10, 0);
    isHappy = true;
    isWarn = false;
    stabilityCheck = 0;
  }

  Serial.print("isHAPP ");
  Serial.println(isHappy);
  Serial.print("LAST HAPP ");
  Serial.println(lastHappy);

  if(isHappy != lastHappy){
    faceChange();
  }
  


//led_set(0, 0, 0);
 
//delay(1500);
//led_set(0, 0, 0);
//delay(500);
}

void faceChange(){

  led_set(0, 0, 0);


  if(isHappy == false){
    Serial.print("WARN STATUS");
    Serial.println(isWarn);
    if (isWarn == true){
      
      face(warnLEDs, 10, 10, 0);
      lastHappy = false;
      //delay(500);

    }else if (isWarn == false){

      face(sadLEDs, 10, 0, 0);
      lastHappy = false;

      delay(1500);
      leftarm.write(90);
      rightarm.write(90);
      Serial.print("Moving to 90*");
      delay(1500);
      leftarm.write(0);
      rightarm.write(180);
      Serial.print("Moving to 180*");
      delay(2000);
      leftarm.write(90);
      rightarm.write(90);
      Serial.print("Moving to 90*");
      delay(2000);
    }

  }else{
    face(happyLEDs, 0, 10, 0);
    lastHappy = true;
  }
  
  

}
