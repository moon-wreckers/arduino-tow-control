/*This file controls the PQ12 linear actuator that opens and closes the claw*/

#include "tow_controller.h"

int lac_d =50;

void lac_setup()
{
    pinMode(DC_MOTOR_EN, OUTPUT);
    pinMode(DC_MOTOR_1, OUTPUT);
    pinMode(DC_MOTOR_2, OUTPUT);

    pinMode(DC_POT_PIN, INPUT);

    pinMode(HAIRTRIGGER_PIN,INPUT_PULLUP);
}

claw_state grab()
{
    PRINTLN("grabbing");
    if(lac_d <80){
        PRINTLN("extending ");
        PRINT(" lac_d ");
        PRINTLN(lac_d);
        analogWrite(DC_MOTOR_EN, HIGH);
        digitalWrite(DC_MOTOR_1, LOW);
        digitalWrite(DC_MOTOR_2, HIGH);
    }
}

claw_state release()
{
    PRINTLN("release");
    while(lac_d >2){
        PRINT("retracting ");
        int lacp_val = analogRead(DC_POT_PIN);
        PRINT(" lac_p ");
        PRINT(lacp_val);
        lac_d = map(lacp_val,20,975,0,100);;
        PRINT(" lac_d ");
        PRINTLN(lac_d);
        analogWrite(DC_MOTOR_EN, HIGH);
        digitalWrite(DC_MOTOR_2, LOW);
        digitalWrite(DC_MOTOR_1, HIGH);
    }
}

int lac_state()
{
    int lacp_val = analogRead(DC_POT_PIN);
    PRINT(" lac_p ");
    PRINT(lacp_val);
    return map(lacp_val,20,975,0,100);;
}

