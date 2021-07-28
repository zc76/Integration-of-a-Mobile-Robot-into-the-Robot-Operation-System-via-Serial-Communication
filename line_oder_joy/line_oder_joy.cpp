#include <ros/ros.h>  
#include "std_msgs/String.h"
#include <boost/thread.hpp>
#include <sstream>
#include <geometry_msgs/Twist.h>
#include<float.h>

  
  
 
  float linear1;
  float linear2;
  float linear3;
  float angular1;
  float angular2;
  float angular3;
  int newline_left;
  int newline_right;
class SubscribeAndPublish  
{  

private:  
  ros::NodeHandle n_; 
  ros::Publisher pub_; 
  ros::Subscriber sub_;
  ros::Subscriber sub2_; 
  ros::Subscriber sub3_; 
  geometry_msgs::Twist msg1;
  geometry_msgs::Twist msg2;
  geometry_msgs::Twist msg3;
  geometry_msgs::Twist msg;
 
public:  
  
 
  SubscribeAndPublish()  
  {  
    
    //Topic you want to publish  

      
    
    pub_= n_.advertise<geometry_msgs::Twist>("/cmd_vel1", 1000);  
    //Topic1 you want to subscribe  
    sub_= n_.subscribe(("cmd_topic", "/cmd_vel"), 10, &SubscribeAndPublish::callback1, this); 
    //Topic2 you want to subscribe  
    sub2_= n_.subscribe("/cmd_vel_rot", 10, &SubscribeAndPublish::callback2, this);  
    sub3_= n_.subscribe("/cmd_vel_gelb", 10, &SubscribeAndPublish::callback3, this);  
  }  

  void callback1(const geometry_msgs::Twist& msg1)  
  {
   
   linear1=msg1.linear.x;
   angular1=msg1.angular.z;
   newline_left=msg1.angular.x;
   newline_right=msg1.angular.y;
   
  }  
  void callback2(const geometry_msgs::Twist& msg2)
  {
  // ROS_INFO("%f",linear1);
   linear2=msg2.linear.x;
   angular2=msg2.angular.z;
  }
  
void callback3(const geometry_msgs::Twist& msg3)  
  {
   linear3=msg3.linear.x;
   angular3=msg3.angular.z;
  if(linear1==0 && angular1==0)
{
  if(linear3==0)
  {
   if(newline_left==0 && newline_right==0)
   {
     msg.linear.x=linear2;
     msg.angular.z=angular2;
   }
   else if(newline_left==0 && newline_right!=0)
   {
    msg.linear.x=0.15;
    msg.angular.z=3;
   }
   else if(newline_left!=0 && newline_right==0)
   {
    msg.linear.x=0.15;
    msg.angular.z=2;
   }
   else
   { 
    msg.linear.x=0;
    msg.angular.z=0;
   }
  }
  else
 {   msg.linear.x=linear3;
     msg.angular.z=angular3;
 }
}
  else
{
     msg.linear.x=linear1;
     msg.angular.z=angular1;
} 
  pub_.publish(msg);
  ros::spinOnce();
  }  
  //void wayofwork(float linear1,float linear2,float angular1,float angular2)
//{  
    
   
//}
};

int main(int argc, char **argv)  
{  
 
  //Initiate ROS  
  ros::init(argc, argv, "line_oder_joy");  
  
  //Create an object of class SubscribeAndPublish that will take care of everything  
  SubscribeAndPublish test;  
  //ros::spin();
  ros::MultiThreadedSpinner s(3);  //多线程
  ros::spin(s);  

  


 

  return 0;  
}  



//————————————————
//版权声明：本文为CSDN博主「cyliujc」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
//原文链接：https://blog.csdn.net/cyliujc/article/details/78707583
