#include "Ultrasonic.h"

Ultrasonic::Ultrasonic(byte TRIG, byte ECHO, String m_Name) {
  this->TRIG = TRIG;
  this->ECHO = ECHO;
  this->m_Name = m_Name;

  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);

  this->timerStart = micros();
}

double Ultrasonic::pulse() {
  digitalWrite(this->TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(this->TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(this->TRIG, LOW);


  this->timerStart = micros();
  while(!digitalRead(this->ECHO)  && (micros() - this->timerStart) <= TIMEOUT);
  this->timerStart = micros();
  while(digitalRead(this->ECHO) && (micros() - this->timerStart) <= TIMEOUT);
  
    this->i++;
    this->distances[i-1] = (micros() - this->timerStart)/10000.0 * SPEED_OF_SOUND_HALF;
    return ((micros() - this->timerStart)/10000.0) * SPEED_OF_SOUND_HALF;
}

double Ultrasonic::medianDist() {
  if(this->i >= ARR_L){
    this->sort();
    this->i = 0;
    return distances[ARR_L / 2];
  }
  return 0.0;
}

void Ultrasonic::sort(){
  for(byte i=0; i<(ARR_L-1); i++) {
    for(byte j=0; j<(ARR_L-(i+1)); j++) {
      if(this->distances[j] > this->distances[j+1]) {
        double t = this->distances[j];
        this->distances[j] = this->distances[j+1];
        this->distances[j+1] = t;
      }
    }
  }
}
