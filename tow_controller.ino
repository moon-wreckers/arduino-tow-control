/*
 * rosserial Servo Control Example
 *
 * This sketch demonstrates the control of hobby R/C servos
 * using ROS and the arduiono
 * 
 * For the full tutorial write up, visit
 * www.ros.org/wiki/rosserial_arduino_demos
 *
 * For more information on the Arduino Servo Library Checkout :
 * http://www.arduino.cc/en/Reference/Servo
 */

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include <ros.h>
#include <std_msgs/String.h>

#include "tow_controller.h"

ros::NodeHandle  nh;


void winch_cb( const std_msgs::String& cmd_msg){
    /*servo.write(cmd_msg.data); //set servo angle, should be from 1000 - 2000 (1500 stops)*/
    if (cmd_msg == "retract") {
        servo.write(1000);
    }else if (cmd_msg == "release"){
        servo.write(1000);
    }else if (cmd_msg == "stop") {
        servo.write(cmd_msg.data);
    }

}

void winch_cb( const std_msgs::String& cmd_msg){
    servo.write(cmd_msg.data); //set servo angle, should be from 1000 - 2000 (1500 stops)
}

ros::Subscriber<std_msgs::UInt16> sub("towing/winch/control ", winch_cb);
ros::Subscriber<std_msgs::UInt16> sub("towing/claw/control", claw_cb);

ros::Publisher claw_status("towing/claw/status", &str_msg);
ros::Publisher winch_status("towing/winch/status", &str_msg);

long trigger_rest =0;

void setup(){
    
  lac_setup();

  nh.initNode();
  nh.subscribe(sub);
  Serial.begin(9600);
    PRINTLN("startup");
    release();
  

  for(int i = 1;i<101;i++)
  {
    trigger_rest+=analogRead(HAIRTRIGGER_PIN);
    delay(2);
  }
  trigger_rest/=100;


PRINTLN("end startup");
}

void loop(){
  nh.spinOnce();

    /*grab();*/
    /*delay(1000);*/
    /*release();*/
    /*delay(1000);*/


  int i = digitalRead(LIMIT_SWITCH_2);
  int trigger = analogRead(HAIRTRIGGER_PIN);
  PRINT(trigger);
  PRINT(" -  ");
  PRINT(trigger_rest);
  PRINT(" =  ");
  PRINTLN(abs(trigger_rest - trigger));

  if(abs(trigger_rest - trigger)>50)
  {
    grab(); 
    delay(10000);
    release(); 
  }


  /*PRINTLN(i);*/
  /*Serial.println(i);*/
  delay(1);
}
