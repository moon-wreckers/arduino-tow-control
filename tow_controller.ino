/*
 * rosserial Servo Control Example
 * This sketch demonstrates the control of hobby R/C servos using ROS and the arduiono
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

claw_state claw_status;
winch_state winch_status;

std_msgs::String winch_msg;
std_msgs::String claw_msg;

void winch_cb(const std_msgs::String &cmd_msg){
    if (cmd_msg.data == "retract") {
        winch_status = RETRACTING;
    }else if (cmd_msg.data == "release"){
        winch_status = EXTENDING;
    }else if (cmd_msg.data == "stop") {
        winch_status = STOPPED;
    }
}
void claw_cb( const std_msgs::String &cmd_msg){

    if (cmd_msg.data == "close") {
            claw_status = CLOSING;
    }else if (cmd_msg.data == "open"){
            claw_status = OPENING;
    }
}

ros::Subscriber<std_msgs::String> winch_sub("towing/winch/control ", winch_cb);
ros::Subscriber<std_msgs::String> claw_sub("towing/claw/control", claw_cb);

ros::Publisher claw_pub("towing/claw/status", &claw_msg);
ros::Publisher winch_pub("towing/winch/status", &winch_msg);


long trigger_rest =0;

void setup(){

    lac_setup();

    nh.initNode();
    nh.subscribe(winch_sub);
    nh.subscribe(claw_sub);

    Serial.begin(9600);
    PRINTLN("startup");

    while ( lac_state()>3)
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

    switch(winch_status){
        case RETRACTED:
            //winch should respond to state being set in callback
            winch_msg.data = "retracted";
            break;
        case RETRACTING:
            winch_status = retract();
            winch_msg.data = "retracting";
            break;
        case STOPPED:
            winch_msg.data = "stopped";
            break;
        case EXTENDING:
            winch_status = extend();
            winch_msg.data = "releasing";
            break;
        case EXTENDED:
            winch_msg.data = "released";
            //We don't actually have the ability to determine this at yet
            break;
    }
    int lac_d = lac_state(); ;
    switch(claw_status){
        case OPEN:
            
            claw_msg.data = "open";
            if (abs(lac_d - trigger_rest)>20){
                claw_status = grab(); 
                claw_msg.data = "closing";
            }
            break;
        case CLOSING:
            claw_status = grab();
            claw_msg.data = "closing";
            break;
        case CLOSED:
            //claw should respond to state being set to "OPENING" in callback
            claw_msg.data = "closed";
            break;
        case OPENING:
            claw_status = release();
            claw_msg.data = "opening";
            break;
    }
   

    winch_pub.publish(&winch_msg);
    claw_pub.publish(&claw_msg);

    delay(1);
}
