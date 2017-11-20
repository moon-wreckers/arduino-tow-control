/*This file controls the PQ12 linear actuator that opens and closes the claw*/

#include "tow_controller.h"

int lac_min =0;
int lac_max =1000;

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
    int lac_d = lac_state();
    if(lac_d <95){
        PRINTLN("extending ");
        PRINT(" lac_d ");
        PRINTLN(lac_d);
        analogWrite(DC_MOTOR_EN, HIGH);
        digitalWrite(DC_MOTOR_1, LOW);
        digitalWrite(DC_MOTOR_2, HIGH);
        return CLOSING;
    }
    else 
        return CLOSED;
}

claw_state release()
{
    PRINTLN("release");
    int lac_d = lac_state();
    if(lac_d >5){
        PRINT(" lac_d ");
        PRINTLN(lac_d);
        analogWrite(DC_MOTOR_EN, HIGH);
        digitalWrite(DC_MOTOR_2, LOW);
        digitalWrite(DC_MOTOR_1, HIGH);
        return OPENING;
    }
    else 
        return OPEN;

}

void stop_actuator()
{
    analogWrite(DC_MOTOR_EN, LOW);
    digitalWrite(DC_MOTOR_2, LOW);
    digitalWrite(DC_MOTOR_1, LOW);
}

void calibrate_lac()
{
    /*analogWrite(DC_MOTOR_EN, HIGH);*/
    digitalWrite(DC_MOTOR_2, LOW);
    digitalWrite(DC_MOTOR_1, HIGH);
    delay(1500);
    lac_min = analogRead(DC_POT_PIN);
    PRINT("lac min: ");
    PRINTLN(lac_min);

    /*analogWrite(DC_MOTOR_EN, HIGH);*/
    digitalWrite(DC_MOTOR_1, LOW);
    digitalWrite(DC_MOTOR_2, HIGH);
    delay(1500);
    lac_max = analogRead(DC_POT_PIN);
    PRINT("lac max: ");
    PRINTLN(lac_max);
    /*analogWrite(DC_MOTOR_EN, HIGH);*/
    digitalWrite(DC_MOTOR_2, LOW);
    digitalWrite(DC_MOTOR_1, HIGH);
    delay(1000);
    stop_actuator();
}



int lac_state()
{
    int lacp_val = analogRead(DC_POT_PIN);
    /*PRINT(" lac_p ");*/
    /*PRINTLN(lacp_val);*/
    return map(lacp_val,lac_min,lac_max,0,100);;
}

