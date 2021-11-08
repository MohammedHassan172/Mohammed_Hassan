#include <ros.h>
#include <std_msgs/Int64.h>
long long counter = 0;
//const int pps = 360;
#define SIGNAL_A PB12
#define SIGNAL_B PB13
HardwareSerial Serial3(PB11, PB10);
ros::NodeHandle  nh;
ArduinoHardware ad;
std_msgs::Int64 int_msg;
ros::Publisher chatter("chatter", &int_msg);
void setup() {
  //Serial3.begin(115200);
  pinMode(SIGNAL_A,INPUT_PULLUP);
  pinMode(SIGNAL_B,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SIGNAL_A),ISR_A,CHANGE);
  attachInterrupt(digitalPinToInterrupt(SIGNAL_B),ISR_B,CHANGE);
  nh.initNode();
  nh.advertise(chatter);
  (nh.getHardware())->setPort(&Serial3);
  (nh.getHardware())->setBaud(1511200);
}

void loop() 
{ 
  int_msg.data = counter;
  chatter.publish( &int_msg );
  nh.spinOnce();
  //Serial3.println(counter);
  
 
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
