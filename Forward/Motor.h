#ifndef _MOTOR_H
#define _MOTOR_H

class Motor {
  
  private:
    byte CW;
    byte ACW;
    String m_Name;

    volatile int rev = 0;

    double error;
    double oldError = 0;
    double integral;

    double kp;
    double ki;
    double kd;

  public:
    byte PWM;
    
    double spins = 0;
  
    Motor(byte, byte, byte, byte, String, double, double, double);
    void turnCW();
    void turnACW();
    void turnCW(byte);
    void turnACW(byte);
	
	  void revInc();
    void revClear();
    void getRPM(unsigned long);    
    void PID(double, unsigned long);
};

#endif // _MOTOR_H   
