#ifndef tow_controller_h
#define tow_controller_h

//this file contains the pin numbers and basic calibration constants
//for the arduino controlling the towing mechanisms

//#define DEBUG 

#ifdef DEBUG
#define PRINT(x) Serial.print(x)
#define PRINTLN(x) Serial.println(x)
#else
#define PRINT(x) 
#define PRINTLN(x) 
#endif


#define HAIRTRIGGER_REST 805

#define LIMIT_SWITCH_1   4
#define LIMIT_SWITCH_2   5

#define DC_MOTOR_1       6
#define DC_MOTOR_2       7
#define DC_MOTOR_EN      8

#define SERVO_PIN        9


#define DC_POT_PIN       A0
#define HAIRTRIGGER_PIN  A1


typedef enum{
    CLAW_STOPPED =0,
    CLAW_CLOSED,
    CLAW_OPEN,
    CLAW_CLOSING,
    CLAW_OPENING
}claw_state;

typedef enum{
    WINCH_STOPPED =0,
    WINCH_RETRACTED,
    WINCH_EXTENDED,
    WINCH_RETRACTING,
    WINCH_EXTENDING,
}winch_state;
#endif
