#if defined(MODEL_EVSHIELD_D)
  // Arduino Duemilanove, Uno
  #define SCL_BAS1  A5
  #define SDA_BAS1  A4
  #define SCL_BAS2  2
  #define SDA_BAS2  A0
  #define SCL_BBS1  4
  #define SDA_BBS1  A1
  #define SCL_BBS2  7
  #define SDA_BBS2  A2

  // deepak
  #define BTN_RIGHT  4
  #define BTN_LEFT  1
  // deepak end

  #define BTN_GO  2
  #define LED_RED  8
  #define LED_GREEN  A3
  #define LED_BLUE  12

#else
  // Arduino mega, 2560
  #define SCL_BAS1  21
  #define SDA_BAS1  20
  #define SCL_BAS2  19
  #define SDA_BAS2  A13
  #define SCL_BBS1  17
  #define SDA_BBS1  A14
  #define SCL_BBS2  18
  #define SDA_BBS2  A15

  #define BTN_LEFT  16
  #define BTN_GO  15
  #define BTN_RIGHT 14
  #define LED_RED  16
  #define LED_GREEN  15
  #define LED_BLUE  14
  
#endif

#include <ArduinoHardware.h>
#include <EVShield.h>
#include <ros.h>
#include <Wire.h>
#include <Arduino.h>
//#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Twist.h>
//#define USE_USBCON
//#define _SS_MAX_EX_BUFF 2048
#include <EVShieldAGS.h>
#include <EVs_EV3Color.h>
#include <std_msgs/Int32.h>

EVShield          evshield(0x34,0x36); 
EVs_EV3Color myColor1;
EVs_EV3Color myColor2;
int linear=0;
int angular=0;
int color1;
int color2;
//float winkel=0.0;

ros::NodeHandle nh;
//geometry_msgs::Twist vel_angular_back;

//void motor_cb(const std_msgs::Int8& vel){
//writeToControler(vel);
//vel_angular_back.linear.x=vel.linear.x;
//vel_angular_back.angular.z=vel.angular.z;
void motor_cb( const geometry_msgs::Twist& cmd_msg){
 // evshield.bank_a.motorRunUnlimited(SH_Motor_1,SH_Direction_Forward,50);
 // evshield.bank_a.motorSetSpeed(SH_Motor_1,50);
  //digitalWrite(13, HIGH-digitalRead(13));
  color1=myColor1.getVal();
  color2=myColor2.getVal();
  linear=cmd_msg.linear.x*40;
  angular=cmd_msg.angular.z*100;
 
  if(  (angular==200) )
  { if(color2!=5)
    {
                   evshield.bank_a.motorRunDegrees(SH_Motor_2,               
                     SH_Direction_Forward, 
                     SH_Speed_Full,
                     45, 
                     SH_Completion_Wait_For,
                     SH_Next_Action_BrakeHold);
                     delay(100);
  }
  else
  {
   evshield.bank_a.motorRunUnlimited(SH_Motor_1,SH_Direction_Reverse,linear);
   delay(1);
  }
  }
 else if(angular==300)
   {     if(color2!=7)             
     {
                   evshield.bank_a.motorRunDegrees(SH_Motor_2,               
                     SH_Direction_Reverse, 
                     SH_Speed_Full,
                     45, 
                     SH_Completion_Wait_For,
                     SH_Next_Action_BrakeHold);
                     delay(100);
  }
  else
  {
    evshield.bank_a.motorRunUnlimited(SH_Motor_1,SH_Direction_Reverse,linear);
    delay(1);
   }
   }
  else if(angular==400)
 {  if(color2==7)
    { evshield.bank_a.motorRunDegrees(SH_Motor_2,               
                     SH_Direction_Forward, 
                     SH_Speed_Full,
                     45, 
                     SH_Completion_Wait_For,
                     SH_Next_Action_BrakeHold);
                     delay(100);
    }
    else if(color2==5)
    {
     evshield.bank_a.motorRunDegrees(SH_Motor_2,               
                     SH_Direction_Reverse, 
                     SH_Speed_Full,
                     45, 
                     SH_Completion_Wait_For,
                     SH_Next_Action_BrakeHold);
                     delay(100);
    }
    else 
    {
  evshield.bank_a.motorRunUnlimited(SH_Motor_1,SH_Direction_Reverse,linear);
  //evshield.bank_a.motorRunUnlimited(SH_Motor_2,SH_Direction_Forward,angular);
  delay(1);
    }
 }
    else
     {evshield.bank_a.motorRunUnlimited(SH_Motor_1,SH_Direction_Reverse,linear);
      evshield.bank_a.motorRunUnlimited(SH_Motor_2,SH_Direction_Forward,angular);
      delay(1);
 }
 
 // digitalWrite(13, HIGH-digitalRead(13));  
   //evshield.bank_a.motorRunUnlimited(SH_Motor_1,SH_Direction_Forward,50);
}


//ros::Subscriber<std_msgs::Int8> sub("/simple_arduino_control/motor_vel",&motor_cb);
  
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel1",&motor_cb);
//ros::Publisher ecu_back("ecu_back",&vel_angular_back);
//ros::Subscriber<std_msgs::Int8> sub("/simple_arduino_control/motor_vel",&motor_cb);
std_msgs::Int32 encoder_msg;
ros::Publisher pub_encoder("encoder", &encoder_msg);

void setup(){
 // pinMode(13, OUTPUT);
/* Serial.begin(57600); 
 delay(500);
 while(linear<100){
 if(linear<100){
 Serial.println(linear);
 }
 }*/
//nh.getHardware()->setBaud(57600);
//delay(500);
evshield.init( SH_HardwareI2C ); 
//
//evshield.bank_a.motorReset();
nh.initNode();
//nh.advertise(ecu_back);
nh.subscribe(sub);
nh.advertise(pub_encoder);
myColor1.init( &evshield, SH_BBS1 );
myColor2.init( &evshield, SH_BBS2 );
myColor1.setMode(MODE_Color_MeasureColor);
myColor2.setMode(MODE_Color_MeasureColor);
//evshield.init( SH_HardwareI2C ); 
//
evshield.bank_a.motorReset();              
//evshield.bank_b.motorReset();

 
}
int encoder;
void loop()                          
{
 
  encoder = evshield.bank_a.motorGetEncoderPosition(SH_Motor_1);
  encoder_msg.data = encoder;
  pub_encoder.publish(&encoder_msg);
 // if (linear>0)
  //{
   nh.spinOnce();
  //}
  
}
// float v_motor1=abs(linear);
//float v_motor2=abs(winkel);
  //evshield.bank_a.motorRunUnlimited(SH_Motor_1,SH_Direction_Forward,v_motor1);
  //evshield.bank_a.motorRunUnlimited(SH_Motor_2,SH_Direction_Forward,v_motor2);
 // int motor1_speed=linear;
  
  




/*void writeToControler(int motor1_speed)
{ motor1_speed=linear;
  if (motor1_speed>=0)
  {
  evshield.bank_a.motorRunUnlimited(SH_Motor_1,SH_Direction_Forward,motor1_speed);
  }
  else{
  evshield.bank_a.motorRunUnlimited(SH_Motor_1,SH_Direction_Reverse,abs(motor1_speed));
  }
  
}*/
