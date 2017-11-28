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
    servo.write(2000); 
    return WINCH_EXTENDING;
}

winch_state retract()
{
    PRINTLN("retracting winch");
    servo.write(1000); 
    return WINCH_RETRACTING;
}

winch_state stop()
{
    PRINTLN("stopping winch");
    servo.write(1500); 
    if(limit_winch){
        return WINCH_RETRACTED;
    }else{
        return WINCH_STOPPED;
    }
}

boolean limit_winch()
{
    int lm1 = digitalRead(LIMIT_SWITCH_1);
    int lm2 = digitalRead(LIMIT_SWITCH_2);
    if (lm1&&lm2)
        digitalWrite(13, HIGH);   // blink the led
        else
        digitalWrite(13, LOW);   // blink the led
    return lm1 && lm2;
}

