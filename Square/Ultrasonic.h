#ifndef _ULTRASONIC_H
#define _ULTRASONIC_H

#define ARR_L 1 //NUMBER OF SAMPLES FOR DISTANCE MEASUREMENT
#define SPEED_OF_SOUND_HALF 171.5 //SPEED OF SOUND

#define TIMEOUT 8900 //TAKE NEW ULTRASONIC READING EVERY .05 SECOND

class Ultrasonic {
  
  private:
    String m_Name; //NAME OF THE MOTOR
    byte TRIG; //TRIG PIN
    byte ECHO; //ECHO PIN
    
    double distances[ARR_L]; //ARRAY OF ALL THE SAMPLES
    byte i = 0; //CURRENT SAMPLE NUMBER

    void sort(); //SORTS ALL THE SAMPLES IN NUMERICAL ORDER [BUBBLE SORT]

  public:
    unsigned long timerStart; //TIME AT WHEN ECHO IS RECIEVED
  
    Ultrasonic(byte, byte, String); //Ultrasonic Sensor CONSTRUCTOR
    
    double pulse(); //SENDS OUT THE ULTRASONIC PULSE

    double medianDist(); //GET THE MEDIAN DEISTANCE FROM ALL THE SAMPLE COLLECTED
};

#endif // _ULTRASONIC_H   
