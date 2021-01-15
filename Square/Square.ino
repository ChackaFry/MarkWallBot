/*----------------Importing Libraries-----------------------------*/
#include "Motor.h"
#include "Ultrasonic.h"
#include "Sound.h"

/*----------------Defining Pins-----------------------------------*/
//Left Motor
#define ENC1 3 //INPUT FROM ENCODER
#define M1A 8 //MOTOR PIN
#define M1B 9 //MOTOR PIN

//Right Motor
#define ENC2 2 //INPUT FROM ENCODER
#define M2A 12 //MOTOR PIN
#define M2B 11 //MOTOR PIN

//Front Ultrasonic
#define TRIG1 5
#define ECHO1 18

/*----------------Defining Constants------------------------------*/
#define COUNTDOWN 250000 //NEW RPM READING TAKEN EVERY .25 SECOND

#define DEG90 750 //TIME TAKEN FOR 360DEG TURN OF ROBOT

#define TARGET_SPINS 850 //TARGET RPM BOTH MOTORS ARE TRYING TO ACHIEVE

/*----------------Declaring Objects-------------------------------*/
//Motor left = Motor(M1A, M1B, ENC1, 53, "Left", 0.008, 0.0075, 0.0077);
//Motor right = Motor(M2A, M2B, ENC2, 50, "Right", 0.067, 0.0033, 0.0072);

//IDEAL VALUES OF MOTORS TO GO AT 850 TICK PER SECOND
Motor left = Motor(M1A, M1B, ENC1, 50, "Left", 0.08, 0.0007, 0.00002);
Motor right = Motor(M2A, M2B, ENC2, 50, "Right", 0.08, 0.0007, 0.00002);

Ultrasonic front = Ultrasonic(TRIG1, ECHO1, "Front");

/*----------------Global Variables--------------------------------*/
//unsigned long timerNow = 0; //TIME NOW
unsigned long timerStart = 0; //TIME SINCE LAST READING (RPM)
//unsigned long elapsedTime = 0; //micros() - timerStart = elapsedTime

unsigned long timerStart2 = 0; //TIME AT 1 CORNER
unsigned long elapsedTimes[4]; //TIME FROM CORNER TO NEXT CORNER
byte turnId = 0; //IDENTIFIES WHICH CORNER JUST CURRENLY PASSED

//unsigned long forwardTiming; //TIME THE ROBOT SHOULD GO FORWARD FOR TO REACH CENTRE
//unsigned long rightTiming; //TIME THE ROBOT SHOULD GO RIGHT FOR TO REACH CENTRE

double distFront = 30.0; //DISTANCE TO WALL IN FRONT

/*----------------Setup-------------------------------------------*/
void lInc(){  left.revInc(); } //INTERRUPT ROUTINE FOR LEFT MOTOR
void rInc(){  right.revInc(); } //INTERRUPT ROUTINE FOR RIGHT MOTOR

void setup() {
  Serial.begin(9600); //SERIAL COMMUNICATIONS
  
  attachInterrupt(digitalPinToInterrupt(ENC1), lInc, RISING); //INTERRUPT FOR LEFT MOTOR
  attachInterrupt(digitalPinToInterrupt(ENC2), rInc, RISING); //INTERRUPT FOR RIGHT MOTOR

  timerStart2 = micros(); //T=0
}

void forward(){ //MOVE FORWARD
  left.turnACW(); //BY MOVING LEFT MOTOR CLOCKWISE
  right.turnCW(); //BY MOVING RIGHT MOTOR ANTI-CLOCKWISE
  
  wheelAdjust(); //GET THE RPM OF BOTH MOTORS AND THEN ADJUST LEFT ACCORDINGLY
}

void wheelAdjust(){ //GET THE RPM OF BOTH MOTORS
//  timerNow = micros();
  unsigned long elapsedTime = micros() - timerStart;
  
  if (elapsedTime >= COUNTDOWN) { //GET RPM ONLY AT CERTAIN INTERVALS (EVERY .25 SECOND)
    left.getRPM(elapsedTime); //LEFT MOTOR RPM
//    Serial.print(" , ");
    right.getRPM(elapsedTime); //RIGHT MOTOR RPM
//    Serial.println();
   
    timerStart = micros(); //NEW READING, THEREFORE RESET THIS VAR
    left.PID(TARGET_SPINS , elapsedTime); //MAKE THE LEFT MOTOR MOVE AT THE SAME SPEED AS THE RIGHT MOTOR
    right.PID(TARGET_SPINS , elapsedTime); //MAKE THE LEFT MOTOR MOVE AT THE SAME SPEED AS THE RIGHT MOTOR
  }
}

void distance(){
  for(byte i = 0; i < ARR_L; i++){
      front.pulse();
      double distance = front.medianDist();
      
      if(distance != 0)  {
          distFront = distance;
          Serial.println(distFront);
      }
  }     
}

void turnRight(){
  //STOP THE ROBOT
  left.turnCW(0);
  right.turnCW(0);
  delay(200);
  rev_Clear();
  
  while(right.rev <= 1080) //TURN THE RIGHT WHEEL FOR 1100 TICKS
    right.turnCW(30); //TURN AT SPEED 50PWM

  //STOP THE ROBOT
  left.turnCW(0);
  right.turnCW(0);
  delay(200);

  rev_Clear(); //CLEAR REV COUNTER

  left.PWM = 50;
  right.PWM = 50;
}

void rev_Clear(){
  left.revClear();
  right.revClear();
}

void lapAround(){
  if(distFront <= 15.0){
    elapsedTimes[turnId] = micros() - timerStart2;
    turnRight();
    timerStart2 = micros(); //T=0
    turnId++;
  }
  else forward();
}

void toCentre(){
  unsigned long forwardTiming = (elapsedTimes[0] + elapsedTimes[2])/4.95;
  unsigned long rightTiming = (elapsedTimes[1] + elapsedTimes[3])/4.2;
  
  timerStart2 = micros();
  while(micros() - timerStart2 <= forwardTiming){
    forward();
  }

  turnRight();

  timerStart2 = micros();
  while(micros() - timerStart2 <= rightTiming){
    forward();
  }

  left.turnCW(0);
  right.turnACW(0);

  turnId++;
}

/*----------------Main/Loop---------------------------------------*/
void loop() {
  if(turnId < 4){
    distance();
    lapAround();
  }else if(turnId == 4){
//    forwardTiming = (elapsedTimes[0] + elapsedTimes[2])/4.7;
//    rightTiming = (elapsedTimes[1] + elapsedTimes[3])/4.3;
    toCentre(); 
  }else if(turnId == 5){
    sSound();
    turnId++;
  }
}
