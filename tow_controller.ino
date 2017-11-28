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
#include <std_msgs/UInt16.h> 
#include "tow_controller.h"

ros::NodeHandle  nh;

claw_state claw_status = CLAW_OPEN;
winch_state winch_status = WINCH_RETRACTED;

std_msgs::UInt16 winch_msg;
std_msgs::UInt16 claw_msg;

void winch_cb(const std_msgs::UInt16 &cmd_msg){
    if (cmd_msg.data == 1) {
        winch_status = WINCH_RETRACTING;
    }else if (cmd_msg.data == 2){
        winch_status = WINCH_EXTENDING;
    }else if (cmd_msg.data == 0) {
        winch_status = WINCH_STOPPED;
    }
}
void claw_cb( const std_msgs::UInt16 &cmd_msg){
    PRINTLN("Clawback");
    PRINTLN(cmd_msg.data);
    /*claw_status = OPENING;*/
  digitalWrite(13, HIGH-digitalRead(13));   // blink the led

    if (cmd_msg.data==1){
            claw_status = CLAW_CLOSING;
    }else if (cmd_msg.data==2){
            claw_status = CLAW_OPENING;
    }else if (cmd_msg.data==0){
            claw_status = CLAW_STOPPED;
    }
}

ros::Subscriber<std_msgs::UInt16> winch_sub("towing/winch/control", winch_cb);
ros::Subscriber<std_msgs::UInt16> claw_sub("towing/claw/control", claw_cb);

ros::Publisher claw_pub("towing/claw/status", &claw_msg);
ros::Publisher winch_pub("towing/winch/status", &winch_msg);

long trigger_rest =0;

void setup(){

  pinMode(13, OUTPUT);
    /*Serial.begin(115200);*/
    /*Serial.println("startup");*/

    lac_setup();
    winch_setup();

    nh.getHardware()->setBaud(115200);
    nh.initNode();
    nh.subscribe(winch_sub);
    nh.subscribe(claw_sub);

    nh.advertise(claw_pub);
    nh.advertise(winch_pub);

    /*Serial.begin(115200);*/
    /*Serial.println("startup");*/

    calibrate_lac();

    for(int i = 1;i<101;i++)
    {
        trigger_rest+=analogRead(HAIRTRIGGER_PIN);
        delay(2);
    }

    trigger_rest/=100;
    PRINT("trigger rest: ");
    PRINTLN(trigger_rest);

    PRINTLN("end startup");
}

void loop(){
    switch(winch_status){
        case WINCH_RETRACTED:
            //winch should respond to state being set in callback
            /*winch_msg.data = "retracted";*/
            break;
        case WINCH_RETRACTING:
            winch_status = retract();
            /*winch_msg.data = "retracting";*/
            break;
        case WINCH_STOPPED:
            stop();
           /*winch_msg.data = "stopped";*/
            break; 
        case WINCH_EXTENDING:
            winch_status = extend();
            /*winch_msg.data = "releasing";*/
            break;
        case WINCH_EXTENDED:
            /*winch_msg.data = "released";*/
            //We don't actually have the ability to determine this at yet
            break;
    }
    int trigger_state = analogRead(HAIRTRIGGER_PIN);
    /*PRINTLN(trigger_state);*/
    switch(claw_status){
        case CLAW_STOPPED:
        case CLAW_OPEN:
    /*Serial.println("OPEN");*/
            /*claw_msg.data = "open";*/
            stop_actuator();
            if (abs(trigger_state - trigger_rest)>20){
            PRINTLN(trigger_state);
                claw_status = grab(); 
                /*claw_msg.data = "closing";*/
            }
            break;
        case CLAW_CLOSING:
    /*Serial.println("CLOSING");*/
            claw_status = grab();
            /*claw_msg.data = "closing";*/
            break;
        case CLAW_CLOSED:
    /*Serial.println("CLOSED");*/
            //claw should respond to state being set to "OPENING" in callback
            stop_actuator();
            /*claw_msg.data = "closed";*/
            break;
        case CLAW_OPENING:
    /*Serial.println("OPENING");*/
            claw_status = release();
            /*claw_msg.data = "opening";*/
            break;
    }

    if (limit_winch()){
       stop(); 
    }

    claw_msg.data = claw_status;
    winch_msg.data = winch_status;
   
    winch_pub.publish(&winch_msg);
    claw_pub.publish(&claw_msg);

    nh.spinOnce();

    delay(100);
}
