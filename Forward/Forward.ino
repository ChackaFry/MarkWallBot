/*----------------Importing Libraries-----------------------------*/
#include "Motor.h"

/*----------------Defining Pins-----------------------------------*/
//Left Motor
#define ENC1 3 //INPUT FROM ENCODER
#define M1A 8 //MOTOR PIN
#define M1B 9 //MOTOR PIN

//Right Motor
#define ENC2 2 //INPUT FROM ENCODER
#define M2A 12 //MOTOR PIN
#define M2B 11 //MOTOR PIN

/*----------------Defining Constants------------------------------*/
#define COUNTDOWN 250000 //NEW RPM READING TAKEN EVERY .25 SECOND

#define DEG360 50000 //TIME TAKEN FOR 360DEG TURN OF ROBOT

/*----------------Declaring Objects-------------------------------*/
Motor left = Motor(M1A, M1B, ENC1, 50, "Left", 0.08, 0.0007, 0.00002);
Motor right = Motor(M2A, M2B, ENC2, 50, "Right", 0.08, 0.0007, 0.00002);

//Motor left = Motor(M1A, M1B, ENC1, 59, "Left", 0.008, 0.0075, 0.0077);
//Motor right = Motor(M2A, M2B, ENC2, 58, "Right", 0.067, 0.0033, 0.0072);

//Motor left = Motor(M1A, M1B, ENC1, 57, "Left", 0.003, 0.0008, 0.0008);
//Motor right = Motor(M2A, M2B, ENC2, 56, "Right", 0.001, 0.0005, 0.005);

//Motor left = Motor(M1A, M1B, ENC1, 53, "Left", 0.07476, 0.9968, 0.0104175); // 0.017, 0.0042, 0.00773
//Motor right = Motor(M2A, M2B, ENC2, 50, "Right", 0.1298, 0.00, 0.00);

/*----------------Global Variables--------------------------------*/
int targetSpins = 850; //TARGET RPM BOTH MOTORS ARE TRYING TO ACHIEVE
unsigned long timerNow = 0; //TIME NOW
unsigned long timerStart = 0; //TIME SINCE LAST READING (RPM)
unsigned long elapsedTime = 0; //timerNow - timerStart == elapsedTime

/*----------------Setup-------------------------------------------*/
void lInc(){  left.revInc(); } //INTERRUPT ROUTINE FOR LEFT MOTOR
void rInc(){  right.revInc(); } //INTERRUPT ROUTINE FOR RIGHT MOTOR

void setup() {
  Serial.begin(9600); //SERIAL COMMUNICATIONS
  
  attachInterrupt(digitalPinToInterrupt(ENC1), lInc, RISING); //INTERRUPT FOR LEFT MOTOR
  attachInterrupt(digitalPinToInterrupt(ENC2), rInc, RISING); //INTERRUPT FOR RIGHT MOTOR

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

void forward(){ //MOVE FORWARD
  left.turnACW(); //BY MOVING LEFT MOTOR CLOCKWISE
  right.turnCW(); //BY MOVING RIGHT MOTOR ANTI-CLOCKWISE
  
  wheelAdjust(); //GET THE RPM OF BOTH MOTORS AND THEN ADJUST LEFT ACCORDINGLY
}

void wheelAdjust(){ //GET THE RPM OF BOTH MOTORS
	timerNow = micros();
	elapsedTime = timerNow - timerStart;
	
	if (elapsedTime >= COUNTDOWN) { //GET RPM ONLY AT CERTAIN INTERVALS (EVERY .25 SECOND)
		left.getRPM(elapsedTime); //LEFT MOTOR RPM
		Serial.print(" , ");
		right.getRPM(elapsedTime); //RIGHT MOTOR RPM
		Serial.println();
   
		timerStart = micros(); //NEW READING, THEREFORE RESET THIS VAR
    left.PID(targetSpins , elapsedTime); //MAKE THE LEFT MOTOR MOVE AT THE SAME SPEED AS THE RIGHT MOTOR
//    Serial.print(" , ");
    right.PID(targetSpins , elapsedTime); //MAKE THE LEFT MOTOR MOVE AT THE SAME SPEED AS THE RIGHT MOTOR
//    Serial.println();
	}
}

/*----------------Main/Loop---------------------------------------*/
void loop() {
  forward();
}
