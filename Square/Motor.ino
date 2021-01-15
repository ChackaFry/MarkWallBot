#include "Motor.h"

Motor::Motor(byte CW, byte ACW, byte ENC, byte PWM, String m_Name, double k1, double k2, double k3) {
  this->CW = CW;
  this->ACW = ACW;
  this->m_Name = m_Name;

  this->PWM = PWM;

  this->kp = k1;
  this->ki = k2;
  this->kd = k3;

  pinMode(ENC, INPUT);
  pinMode(CW, OUTPUT);
  pinMode(ACW, OUTPUT);
}

void Motor::turnCW(){
  analogWrite(this->ACW, 0);
  analogWrite(this->CW, this->PWM);
}

void Motor::turnCW(byte PWM){
  analogWrite(this->ACW, 0);
  analogWrite(this->CW, PWM);
}

void Motor::turnACW(){
  analogWrite(this->CW, 0);
  analogWrite(this->ACW, this->PWM);
}

void Motor::turnACW(byte PWM){
  analogWrite(this->CW, 0);
  analogWrite(this->ACW, PWM);
}

void Motor::revInc() {
  this->rev++;
}

void Motor::revClear() {
  this->rev = 0;
}

void Motor::getRPM(unsigned long elapsedTime) {
    this->spins = this->rev / (elapsedTime / (double)1000000.0);
//    Serial.println(this->m_Name + ": spins: " + String(this->spins));
    this->rev = 0;
//    Serial.print(this->spins);
}

void Motor::PID(double targetSpins, unsigned long elapsedTimeInMicros){
	
	this->oldError = this->error; //Differential
	
	this->error = targetSpins - this->spins; //Error
//	Serial.println(this->m_Name + ": error: " + String(this->error));

	double elapsedTime = (elapsedTimeInMicros)/1000000.0;

  //This is probably correct
  this->integral += (elapsedTime)*error; //Integral
	this->PWM += (byte)(( this->kp*this->error ) + ( this->ki*this->integral ) + ( (this->kd/elapsedTime)*(oldError - error) )); //(P)+(I)+(D)
  
//  Serial.println(this->m_Name + ": PWM: " + String(this->PWM));
//  Serial.println(this->PWM);
}
