float pulse_width, distance;
#include <Servo.h>
#include <NewPing.h>
#include <Wire.h>
#define TRIGGER_PIN   8  // Trigger pin of the ultrasonic sensor.
#define ECHO_PIN      9  // Echo pin of the ultrasonic sensor.
#define MAX_DISTANCE  200 // Movement range of pin pon ball in mm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

Servo myservo;  // Indicated servo name
int potpin = 0;  // Potantiometer pin
int val;    // variable to read the value from the analog pin
int delta;
int newval; 
int old_distance;
int new_distance;


//-----------------------PID-----------------------------//
int Read = 0;
float elapsedTime, time, timePrev;  
float distance_previous_error, distance_error;
int period = 50;

//-------------------PID VALUES////-----------------------//
float kp=2; 
float ki=0.05;
float kd=100;
float distance_setpoint = 14.5;  // the point at which we desire to make it stable in cm    
float PID_p, PID_i, PID_d, PID_total;
///////////////////////////////////////////////////////


void setup() {
  // put your setup code here, to run once:
 
  Serial.begin(9600); 
  time = millis();
  myservo.attach(3); //Servo attached pin

}



void loop() {
    
  distance=sonar.ping_cm();  //get the distance by library function in cm
  
  //---------the data will be taken in a shorter time and this part decreases the small errors between values--------//
  old_distance=distance;
  new_distance=(distance+old_distance)/2; 
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//----------------I had a problem with some sensor values, so this part is not necessary normally-----------------//
  if (new_distance>=29){                                                                                          //
    if(old_distance>new_distance){                                                                                //
      new_distance=old_distance-1;                                                                                //
      Serial.print("Distance = ");                                                                                //
  Serial.print(new_distance);                                                                                     //
  Serial.print(" cm");                                                                                            //
    }                                                                                                             //
    else if(old_distance<new_distance){                                                                           //
      new_distance=old_distance+1;                                                                                //
      Serial.print("Distance = ");                                                                                //
  Serial.print(new_distance);                                                                                     //
  Serial.print(" cm");                                                                                            //
    }}                                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   
    else{
       Serial.print("Distance = ");
  Serial.print(new_distance);
  Serial.print(" cm");
    
  }

//--------------------P controller-------------------//
 distance_error = distance- distance_setpoint ;  
  PID_p = kp * distance_error;
//////////////////////////////////////////////////////


//--------------------I controller-------------------//
if(-3 < distance_error && distance_error < 3)
    {
      PID_i = PID_i + (ki * distance_error);
    }
    else
    {
      PID_i = 0;
      }
///////////////////////////////////////////////////////



//--------------------D controller------------------------------//
  PID_d = kd*((distance_error - distance_previous_error)/period);
//////////////////////////////////////////////////////////////////

//----------------------PID controller--------------------------//
  PID_total = PID_p + PID_i + PID_d; 

  
  //PID_total = map(PID_total, -150, 150, 0, 80); //if limiting of the value is required

  
myservo.write(PID_total+41);  //in my system, the angle which makes the system exact horizontal position was 41 degrees, so it will change for each system
Serial.print("PID TOTAL= "); 
Serial.print(PID_total);      //printing the pid value


Serial.print("distance_error= ");
Serial.print(distance_error);   //printing the value how the ball is far from the set point

 
  //delay(250);                //adding delay can be useful in order to see the errors in sensor data, otherwise it can be removed
   val = analogRead(potpin);   // gets the value of the potentiometer
  Serial.print(", pot_val=");  // printing string characters 
  Serial.println(val);         // printing the value of potantiometer
}
