/* Motor shield library test */

#include "Motorshield.h"

#define DC_MOT_CURRENT MotorShield::MOT_A

#define DC_MOT 1
#define DC_MOT_ADJUST 1
//#define DC_MOT_RUN 1

/* DC motor setup */
#ifdef DC_MOT
MotorShield mot(2, 3, 90, 8, 9, 90);
#endif

void dcMotSetup() {
    Serial.begin(115200);
    Serial.println("*** DC motor demo for MotorShield ***");
}

void dcMotAdjustHelp() {
    Serial.println("Setup: use following keys");
    Serial.println("       h: print help");
    Serial.println("       c: rotate clock-wise");
    Serial.println("       a: rotate counter-clock-wise");
    Serial.println("       +: increase speed by 1 (raw)");
    Serial.println("       -: decrease speed by 1 (raw)");
    Serial.println("       o: turn off");
    Serial.println("       f: turn on at full speed");
}

void dcMotPrintStat() {
    Serial.print("Setup: Motor turns ");
    Serial.print(mot.direction(DC_MOT_CURRENT) == MotorShield::CW ?
                 "clock-wise" : "counter-clock-wise");
    Serial.print(" at ");
    Serial.println(mot.speed(DC_MOT_CURRENT));
}

void dcMotAdjust() {
    int c;
    Serial.println("Setup: Motor off");
    mot.setDirection(DC_MOT_CURRENT, MotorShield::CW);
    mot.setRawSpeed(DC_MOT_CURRENT, MotorShield::OFF);
    dcMotPrintStat();

    for (;;) {
        c = Serial.read();
        switch (c) {
        case 'h':
            dcMotAdjustHelp();
            break;
        case 'c':
            mot.setDirection(DC_MOT_CURRENT, MotorShield::CW);
            break;
        case 'a':
            mot.setDirection(DC_MOT_CURRENT, MotorShield::CCW);
            break;
        case '+':
            mot.setRawSpeed(DC_MOT_CURRENT,
                            mot.speed(DC_MOT_CURRENT) + 1);
            break;
        case '-':
            mot.setRawSpeed(DC_MOT_CURRENT,
                            mot.speed(DC_MOT_CURRENT) - 1);
            break;
        case 'o':
            mot.setRawSpeed(DC_MOT_CURRENT, 0);
            break;
        case 'f':
            mot.setRawSpeed(DC_MOT_CURRENT, 255);
            break;
        }
        if (c > 0)
            dcMotPrintStat();
    }
}

void dcMotRun() {
    Serial.println("Demo: Run clock wise at 1% speed for 5s");
    mot.setDirection(DC_MOT_CURRENT, MotorShield::CW);
    mot.setSpeed(DC_MOT_CURRENT, 1);
    delay(5000);

    Serial.println("Demo: Run counter clock wise at 80% speed for 5s");
    mot.setDirection(DC_MOT_CURRENT, MotorShield::CCW);
    mot.setSpeed(DC_MOT_CURRENT, 80);
    delay(5000);

    Serial.println("Demo: Slow down to 0% within 10s");
    mot.setSpeed(DC_MOT_CURRENT, 0, 10000);
    Serial.println("Demo: Speed up to 80% within 10s");
    mot.setSpeed(DC_MOT_CURRENT, 80, 10000);

    Serial.println("Demo: Turn off and sleep for 1s");
    mot.setSpeed(DC_MOT_CURRENT, 0);
    mot.setDirection(DC_MOT_CURRENT, MotorShield::CW);
    delay(1000);
}

/* setup */
void setup() {
#ifdef DC_MOT
    dcMotSetup();
#endif
};

/* main */
void loop() {
#ifdef DC_MOT_ADJUST
    dcMotAdjust();
#elif DC_MOT_RUN
    dcMotRun();
#endif
}
