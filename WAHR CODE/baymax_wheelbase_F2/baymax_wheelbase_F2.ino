//Programmer Name: Saheel Siyam, Arnav Kothekar
//Project Name: Wall Avoiding Humanoid Robot (WAHR)aka 'Baymax'
//Code File Description: WAHR Slave Arduino - controls the 2 DC Motors 
//                       Receives signals from WAHR Master Arduino which controls the LED Matrix, Ultrasonic Sensor and Servo Motors
#include "Communication.h"
//COMMUNICATION LIBRARY CREDIT GOES TO  THE LAST OUTPOST WORKSHOP
//https://dev.azure.com/overlording/The%20Last%20Outpost%20Workshop/_git/CommArduinoA?path=/arduinoA.ino


#define MASTER_ARDUINO_ADDRESS 10
#define SLAVE_ARDUINO_ADDRESS 20

static Communication Comm(SLAVE_ARDUINO_ADDRESS);

int leftmotorpin1 = 2;
int leftmotorpin2 = 3;

int rightmotorpin1 = 4;
int rightmotorpin2 = 5;

int code;

void setup() {
  
  pinMode(leftmotorpin1, OUTPUT);
  pinMode(leftmotorpin2, OUTPUT);
  pinMode(rightmotorpin1, OUTPUT);
  pinMode(rightmotorpin2, OUTPUT);

  Serial.begin(9600);
  delay(100);

}

void loop() {
  //Serial.println("CODE: ");
  //Serial.println(code);  

  switch (Comm.Received()){

    Serial.print("MESS RECVD");
    
    case STRING_MESSAGE:
      // Received a string message
      Serial.println(Comm.GetStringMessage());
      break;

    case INT_MESSAGE:
      code = Comm.GetIntMessage();
      Serial.println(Comm.GetIntMessage());
      if (code == 11){
        brake();
        brake();
        brake();
        turnMotors();
        Serial.println("TURNN");
        turnMotors();
        Serial.println("TURNN");
      }else if (code == 1){
        brake();
        brake();
        brake();
        Serial.println("WARN!");
      }
      break;

  }
  forward();
  forward();
  forward();
  brake();

} 

void forward(){
  digitalWrite(leftmotorpin1, LOW);
  digitalWrite(leftmotorpin2, HIGH);

  digitalWrite(rightmotorpin1, HIGH);
  digitalWrite(rightmotorpin2, LOW);
  delay(200); 
}
   
void brake(){
  Serial.println("BRAKE!");
    //turns off motors after a bit
  digitalWrite(leftmotorpin1, LOW);
  digitalWrite(leftmotorpin2, LOW);

  digitalWrite(rightmotorpin1, LOW);
  digitalWrite(rightmotorpin2, LOW);
  delay(500);


} 



void turnMotors(){
  
    //turns off motors after a bit


  Serial.println("REVERSE!");
  digitalWrite(leftmotorpin1, HIGH);
  digitalWrite(leftmotorpin2, LOW);

  digitalWrite(rightmotorpin1, LOW);
  digitalWrite(rightmotorpin2, HIGH);
  delay(500); 
  
  brake();
  
  
  Serial.println("TURN NOW!");
  digitalWrite(leftmotorpin1, HIGH);
  digitalWrite(leftmotorpin2, LOW);

  digitalWrite(rightmotorpin1, LOW);
  digitalWrite(rightmotorpin2, LOW);
  delay(700); 

  brake();


  code = 0;

}
