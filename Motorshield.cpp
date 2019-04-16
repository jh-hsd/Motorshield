// motor shield control library

#include "Arduino.h"
#include "Motorshield.h"

MotorShield::MotorShield(int dirAPin, int pwmAPin, int minPwmA,
                         int dirBPin, int pwmBPin, int minPwmB)
{
    _motA.dirPin = dirAPin;
    _motA.pwmPin = pwmAPin;
    _motA.minPwm = minPwmA;
    _motA.dir = OFF;
    _motA.speed = OFF;

    _motB.dirPin = dirBPin;
    _motB.pwmPin = pwmBPin;
    _motB.minPwm = minPwmB;
    _motB.dir = OFF;
    _motB.speed = OFF;

    // init pwr and direction outputs
    pinMode(_motA.dirPin, OUTPUT);
    pinMode(_motA.pwmPin, OUTPUT);
    pinMode(_motB.dirPin, OUTPUT);
    pinMode(_motB.pwmPin, OUTPUT);

    // turn the motors off
    analogWrite(_motA.pwmPin, OFF);
    analogWrite(_motB.pwmPin, OFF);
}

void MotorShield::setDirection(Motor mot, MotorShield::State dir)
{
    struct MotorDefinition &motor = _motor(mot);

    motor.dir = dir;
    _update(motor);
}

MotorShield::State MotorShield::direction(Motor mot) const
{
    const struct MotorDefinition &motor = _const_motor(mot);

    return motor.dir;
}

void MotorShield::setRawSpeed(Motor mot, int speed, int duration)
{
    struct MotorDefinition &motor = _motor(mot);
    int steps;
    int delayTime;
    int offset;

    speed = constrain(speed, 0, 255);

    steps = speed - motor.speed;
    delayTime = duration / abs(steps);
    offset = (steps > 0) ? +1 : -1;

    // ensure that direction is set
    if (motor.speed == OFF && !!speed)
        _update(motor);

    // loop over steps if any
    for (int i = 0; i < abs(steps); i++) {
        motor.speed += offset;
        analogWrite(motor.pwmPin, motor.speed);
        delay(delayTime);
    }

    // ensure correct final value
    motor.speed = speed;
    analogWrite(motor.pwmPin, motor.speed);
}

void MotorShield::setSpeed(Motor mot, int speed, int duration)
{
    struct MotorDefinition &motor = _motor(mot);
    int steps;
    int delayTime;
    int offset;

    speed = constrain(speed, 0, 100);
    steps = speed - motor.speed;
    delayTime = duration / abs(steps);
    offset = (steps > 0) ? +1 : -1;

    // loop over steps if any
    for (int i = 0; i < abs(steps); i++) {
        motor.speed += offset;
        _update(motor);
        delay(delayTime);
    }

    // ensure correct final value
    motor.speed = speed;
    _update(motor);
}

int MotorShield::speed(Motor mot) const
{
    const struct MotorDefinition &motor = _const_motor(mot);

    return motor.speed;
}

struct MotorShield::MotorDefinition &MotorShield::_motor(Motor mot)
{
    switch (mot) {
    case MOT_A:
        return _motA;
    case MOT_B:
        return _motB;
    }

    return _motA;
}

const struct MotorShield::MotorDefinition &MotorShield::_const_motor(Motor mot) const
{
    switch (mot) {
    case MOT_A:
        return _motA;
    case MOT_B:
        return _motB;
    }

    return _motA;
}

int MotorShield::_dutyCycleFromPercent(struct MotorDefinition &motor)
{
    int d = 255 - motor.minPwm;
    int l = d / 100 * motor.speed;
    return (l + motor.minPwm);
}

void MotorShield::_off(struct MotorDefinition &motor)
{
    digitalWrite(motor.dirPin, LOW);
    analogWrite(motor.pwmPin, 0);
}

void MotorShield::_update(struct MotorDefinition &motor)
{
    switch (motor.speed) {
    case OFF:
        _off(motor);
        break;
    default:
        analogWrite(motor.pwmPin, _dutyCycleFromPercent(motor));
        break;
    }

    switch (motor.dir) {
    case OFF:
        _off(motor);
        break;
    case CW:
        digitalWrite(motor.dirPin, HIGH);
        break;
    case CCW:
        digitalWrite(motor.dirPin, LOW);
        break;
    }
}
