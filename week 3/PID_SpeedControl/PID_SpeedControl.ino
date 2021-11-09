#include <ros.h>
#include <std_msgs/Float64.h>
#define pmw PA6
#define dir PA5
ros::NodeHandle nh;
double setpoint; // sent speed.
double output; // the output speed from PID.
double sensor_output; // sensor reading.
double Kp; //proportional gain.
double Ki; //integral gain.
double Kd; //derivative gain.
int time_sample = 1000; // the period for each output
unsigned long last_time;
double total_error; //For the integral term.
double last_error;//For the devirative term.
int max_control;//to prevent overshot if the max speed is reached.
int min_control;//to prevent the motor from taking no voltage for too long.


void messageCb( const std_msgs::Float64& setpoint_ros)
 {
    setpoint = setpoint_ros.data;//the funtion that will be called to set the setpoint from ros.
 }
 
 ros::Subscriber<std_msgs::Float64> sub("set speed", &messageCb );//making the Subscriber.


void setup() 
{
 pinMode(pmw,OUTPUT);
 pinMode(dir,OUTPUT);
 nh.initNode(); 
 nh.subscribe(sub);

}

void loop() 
{
  PID_Control();
  analogWrite(pmw,output);
  if(setpoint > 0)
  {
   digitalWrite(dir,HIGH); 
  }
  else
  {
   digitalWrite(dir,LOW); 
  }
  nh.spinOnce();
  delay(1);

}

void PID_Control()
{
  unsigned long current_time  = millis();
  int delta_time = current_time - last_time;

  if(delta_time >= time_sample)
  {
    double error = setpoint - sensor_output;//calculate the error
    total_error += error;//the sum for the errors for the intrgral
    if (total_error >= max_control) total_error = max_control;
    else if (total_error <= min_control) total_error = min_control;
    double delta_error = error - last_error; //to calculate the change for the devirative.
    output = Kp*error + (Ki*time_sample)*total_error - (Kd/time_sample)*delta_error; //PID control compute

   //(ask why this is needed).
   //{
       if (output >= max_control) output = max_control;
       else if (output <= min_control) output = min_control;
   //}
   
   last_error = error;
   last_time = current_time;
    
  }
}
