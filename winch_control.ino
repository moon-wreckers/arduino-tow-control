/*This file controls the PQ12 linear actuator that opens and closes the claw*/

#include "tow_controller.h"
#include <Servo.h> 

Servo servo;
void winch_setup()
{
    servo.attach(SERVO_PIN); //attach it to pin 9

    pinMode(LIMIT_SWITCH_1, INPUT_PULLUP);
    pinMode(LIMIT_SWITCH_2, INPUT_PULLUP);

}


winch_state extend()
{
    PRINTLN("paying out winch ");
    servo.write(1000); 
    return EXTENDING;
}

winch_state retract()
{
    PRINTLN("retracting winch");
    if(limit_winch){
        servo.write(1500); 
        return RETRACTING;
    } else{
        servo.write(2000); 
        return RETRACTED;
    }
}

winch_state stop()
{
    PRINTLN("stopping winch");
    servo.write(1500); 
    if(limit_winch){
        return RETRACTED;
    }else{
        return STOPPED;
    }
}

boolean limit_winch()
{
    int lm1 = digitalRead(LIMIT_SWITCH_1);
    int lm2 = digitalRead(LIMIT_SWITCH_2);

    return lm1 && lm2;
}

