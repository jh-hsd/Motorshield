// Motor shield library test

#include "Motorshield.h"

#define DC_MOT_CURRENT MotorShield::MOT_A

// you can choose between 4 example modes (mutually exclusive)
#define DC_MOT_ADJUST 1 // control DC motor with serial commands
//#define DC_MOT_RUN 1 // run sequence on DC motor
//#define STEPPER_ADJUST 1 // control stepper with serial commands
//#define STEPPER_RUN 1 // run sequence on stepper

#if defined DC_MOT_ADJUST || defined DC_MOT_RUN
#define DC_MOT 1
#endif

#if defined STEPPER_ADJUST || defined STEPPER_RUN
#define STEPPER 1
#endif

// DC motor setup
#ifdef DC_MOT
MotorShield mot(2, 3, 90, 8, 9, 90);
#endif

// Stepper setup
#ifdef STEPPER
MotorShield mot(2, 3, 8, 9);
#endif

void dcMotSetup()
{
    Serial.begin(115200);
    Serial.println("*** DC motor demo for MotorShield ***");
}

void stepperSetup()
{
    Serial.begin(115200);
    Serial.println("*** Stepper demo for MotorShield ***");
}

void dcMotAdjustHelp()
{
    Serial.println("Setup: use following keys");
    Serial.println("       h: print help");
    Serial.println("       c: rotate clock-wise");
    Serial.println("       a: rotate counter-clock-wise");
    Serial.println("       +: increase speed by 1 (raw)");
    Serial.println("       -: decrease speed by 1 (raw)");
    Serial.println("       o: turn off");
    Serial.println("       f: turn on at full speed");
}

void stepperAdjustHelp()
{
    Serial.println("Setup: use following keys");
    Serial.println("       h: print help");
    Serial.println("       c: rotate clock-wise");
    Serial.println("       a: rotate counter-clock-wise");
    Serial.println("       C: rotate clock-wise by 30 steps");
    Serial.println("       A: rotate counter-clock-wise by 30 steps");
}

void dcMotPrintStat()
{
    Serial.print("Setup: Motor turns ");
    Serial.print(mot.direction(DC_MOT_CURRENT) == MotorShield::CW ?
                 "clock-wise" : "counter-clock-wise");
    Serial.print(" at ");
    Serial.println(mot.speed(DC_MOT_CURRENT));
}

void dcMotAdjust()
{
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

void stepperAdjust()
{
    int c;
    Serial.println("Setup: Motor off");

    for (;;) {
        c = Serial.read();
        switch (c) {
        case 'h':
            stepperAdjustHelp();
            break;
        case 'c':
            mot.step(MotorShield::CW);
            break;
        case 'a':
            mot.step(MotorShield::CCW);
            break;
        case 'C':
            mot.steps(MotorShield::CW, 30);
            break;
        case 'A':
            mot.steps(MotorShield::CCW, 30);
            break;
        }
    }
}

void dcMotRun()
{
    Serial.println("Demo: Run clock-wise at 1% speed for 5s");
    mot.setDirection(DC_MOT_CURRENT, MotorShield::CW);
    mot.setSpeed(DC_MOT_CURRENT, 1);
    delay(5000);

    Serial.println("Demo: Run counter-clock-wise at 80% speed for 5s");
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

void stepperRun()
{
    Serial.println("Demo: Step 100 steps clock-wise");
    mot.steps(MotorShield::CW, 100);
    delay(1000);

    Serial.println("Demo: Step 10 times 10 steps counter-clock-wise");
    for (int i = 0; i < 10; i++) {
        mot.steps(MotorShield::CCW, 100);
        delay(500);
    }

    Serial.println("Demo: Step 100 steps counter-clock-wise");
    mot.steps(MotorShield::CCW, 100);
    delay(1000);

    Serial.println("Demo: Step 10 times 10 steps clock-wise");
    for (int i = 0; i < 10; i++) {
        mot.steps(MotorShield::CW, 100);
        delay(500);
    }

    delay(1000);
}

// setup
void setup()
{
#ifdef DC_MOT
    dcMotSetup();
#elif STEPPER
    stepperSetup();
#endif
};

// main
void loop()
{
#ifdef DC_MOT_ADJUST
    dcMotAdjust();
#elif DC_MOT_RUN
    dcMotRun();
#elif STEPPER_ADJUST
    stepperAdjust();
#elif STEPPER_RUN
    stepperRun();
#endif
}
