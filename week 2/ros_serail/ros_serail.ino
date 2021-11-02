#include <ros.h>
#include <std_msgs/Int64.h>
long long counter = 0;
const int pps = 360;
#define SIGNAL_A PB3
#define SIGNAL_B PB4
ros::NodeHandle  nh;
std_msgs::Int64 int_msg;
ros::Publisher chatter("chatter", &int_msg);
void setup() {
  pinMode(SIGNAL_A,INPUT_PULLUP);
  pinMode(SIGNAL_B,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SIGNAL_A),ISR_A,CHANGE);
  attachInterrupt(digitalPinToInterrupt(SIGNAL_B),ISR_B,CHANGE);
  nh.initNode();
  nh.advertise(chatter);
}

void loop() 
{ 
  int_msg.data = counter;
  chatter.publish( &int_msg );
  nh.spinOnce();
  delay(1000);
 
}


void ISR_A()
{
  if(digitalRead(SIGNAL_A)!= digitalRead(SIGNAL_B))
  {
     counter++;
  }
  else
  {
    counter--;
  }  
}


void ISR_B()
{
  if(digitalRead(SIGNAL_A) == digitalRead(SIGNAL_B))
  {
    counter++;
  }  
  else
  {
    counter--;
  }  
}
