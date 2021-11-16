#!/usr/bin/env python
import rospy
from std_msgs.msg import Float32
State_prev = 1
Uncertainty_prev = 5
Kalman_G = 0
pub = rospy.Publisher('yawKF', Float32, queue_size=10)

def callback(data):
    Sensor_Read = data.data
    Kalman_Filter(Sensor_Read)
    
def listener():
    global State_prev
    x = State_prev
    rospy.init_node('yawRAW', anonymous=True)
    rate = rospy.Rate(10)
    while not rospy.is_shutdown():
        pub.publish(x)
        rate.sleep()
        rospy.Subscriber("chatter", Float32, callback)
    rospy.spin()    


def Kalman_Filter(Sensor_read):
    global State_prev
    global Uncertainty_prev
    global  Kalman_G 
    #process_variance = 2
    sensor_variance  = 4
    Pred_State = State_prev
    Pred_Uncertainty = Uncertainty_prev - Uncertainty_prev *  Kalman_G 
    #Pred_Uncertainty = Uncertainty_prev + process_variance
    Kalman_G = Pred_Uncertainty / (Pred_Uncertainty + sensor_variance)
    State_prev = Kalman_G * Sensor_read + (1-Kalman_G) * Pred_State
    Uncertainty_prev = (1-Kalman_G) * Pred_Uncertainty
    
if __name__ == '__main__':
    listener()