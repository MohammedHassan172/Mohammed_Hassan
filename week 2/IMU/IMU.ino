#include <MPU6050_6Axis_MotionApps20.h>
#include "I2Cdev.h"
#include "Wire.h"
HardwareSerial Serial3(PB11, PB10);
MPU6050 mpu;
uint8_t fifoBuffer[64];
Quaternion q;
VectorFloat gravity;
uint8_t devStatus;
float ypr[3];
bool dmpReady = false;
void setup() {
Wire.begin();
Wire.setClock(400000);
Serial.begin(115200); 
Serial3.begin(115200);
  
mpu.initialize();
devStatus = mpu.dmpInitialize();
  if (devStatus == 0) 
  {
   mpu.CalibrateGyro(30);
   mpu.setDMPEnabled(true);
   dmpReady = true;
  }
  else
  {
     Serial.print(F("DMP Initialization failed (code "));
     Serial.print(devStatus);
     Serial.println(F(")"));
  }
}

void loop() {
  if (!dmpReady) return;
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer))
  {
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    Serial.print("yaw = ");
    Serial.print(ypr[0] * 180/M_PI);
    Serial.print("\tpitch = ");
    Serial.print(ypr[1] * 180/M_PI);
    Serial.print("\troll = ");
    Serial.print(ypr[2] * 180/M_PI);

    Serial3.print("yaw = ");
    Serial3.print(ypr[0] * 180/M_PI);
    Serial3.print("\tpitch = ");
    Serial3.print(ypr[1] * 180/M_PI);
    Serial3.print("\troll = ");
    Serial3.print(ypr[2] * 180/M_PI);
    
  }
  

}
