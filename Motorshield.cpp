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
    _motA.type = DC_MOT;

    _motB.dirPin = dirBPin;
    _motB.pwmPin = pwmBPin;
    _motB.minPwm = minPwmB;
    _motB.dir = OFF;
    _motB.speed = OFF;
    _motB.type = DC_MOT;

    // init pwr and direction outputs
    pinMode(_motA.dirPin, OUTPUT);
    pinMode(_motA.pwmPin, OUTPUT);
    pinMode(_motB.dirPin, OUTPUT);
    pinMode(_motB.pwmPin, OUTPUT);

    // turn the motors off
    analogWrite(_motA.pwmPin, OFF);
    analogWrite(_motB.pwmPin, OFF);
}

MotorShield::MotorShield(int dirAPin, int pwmAPin, int dirBPin, int pwmBPin)
{
    _motA.dirPin = dirAPin;
    _motA.pwmPin = pwmAPin;
    _motA.type = STEPPER;

    _motB.dirPin = dirAPin;
    _motB.pwmPin = pwmAPin;
    _motB.type = STEPPER;

    _stepPos = 0;

    pinMode(_motA.pwmPin, OUTPUT);
    pinMode(_motA.dirPin, OUTPUT);
    pinMode(_motB.pwmPin, OUTPUT);
    pinMode(_motB.dirPin, OUTPUT);
}

void MotorShield::setDirection(MotorShield::Motor mot, MotorShield::State dir)
{
    struct MotorDefinition &motor = _motor(mot);

    if (!_isDC(mot))
        return;

    motor.dir = dir;
    _update(motor);
}

MotorShield::State MotorShield::direction(MotorShield::Motor mot) const
{
    const struct MotorDefinition &motor = _const_motor(mot);

    return motor.dir;
}

void MotorShield::setRawSpeed(MotorShield::Motor mot, int speed, int duration)
{
    struct MotorDefinition &motor = _motor(mot);
    int steps;
    int delayTime;
    int offset;

    if (!_isDC(mot))
        return;

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

void MotorShield::setSpeed(MotorShield::Motor mot, int speed, int duration)
{
    struct MotorDefinition &motor = _motor(mot);
    int steps;
    int delayTime;
    int offset;

    if (!_isDC(mot))
        return;

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

int MotorShield::speed(MotorShield::Motor mot) const
{
    const struct MotorDefinition &motor = _const_motor(mot);

    return motor.speed;
}

void MotorShield::step(MotorShield::State dir)
{
    if (dir == OFF)
        return;

    _stepPos += (dir == CW) ? 1 : -1;

    switch (_stepPos % 4) {
    case 0:
        _setCoil(MOT_B, OFF);
        _setCoil(MOT_A, CW);
        break;
    case 1:
    case -1:
        _setCoil(MOT_A, OFF);
        _setCoil(MOT_B, CW);
        break;
    case 2:
    case -2:
        _setCoil(MOT_B, OFF);
        _setCoil(MOT_A, CCW);
        break;
    case 3:
    case -3:
        _setCoil(MOT_A, OFF);
        _setCoil(MOT_B, CCW);
        break;
    }

    /* let motor time to move */
    delayMicroseconds(10);

    /* turn off */
    _setCoil(MOT_A, OFF);
    _setCoil(MOT_B, OFF);
}

void MotorShield::steps(MotorShield::State dir, int steps)
{
    if (!_isStepper())
        return;

    for (int i = 0; i < steps; i++)
        step(dir);
}

struct MotorShield::MotorDefinition &MotorShield::_motor(MotorShield::Motor mot)
{
    switch (mot) {
    case MOT_A:
        return _motA;
    case MOT_B:
        return _motB;
    }

    return _motA;
}

const struct MotorShield::MotorDefinition &MotorShield::_const_motor(MotorShield::Motor mot) const
{
    switch (mot) {
    case MOT_A:
        return _motA;
    case MOT_B:
        return _motB;
    }

    return _motA;
}

int MotorShield::_dutyCycleFromPercent(struct MotorShield::MotorDefinition &motor)
{
    int d = 255 - motor.minPwm;
    int l = d / 100 * motor.speed;
    return (l + motor.minPwm);
}

void MotorShield::_off(struct MotorShield::MotorDefinition &motor)
{
    digitalWrite(motor.dirPin, LOW);
    analogWrite(motor.pwmPin, 0);
}

void MotorShield::_update(struct MotorShield::MotorDefinition &motor)
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

bool MotorShield::_isStepper() const
{
    return (_motA.type == STEPPER && _motB.type == STEPPER);
}

bool MotorShield::_isDC(MotorShield::Motor mot) const
{
    const struct MotorDefinition &motor = _const_motor(mot);
    return (motor.type == DC_MOT);
}

void MotorShield::_setCoil(MotorShield::Motor mot, MotorShield::State dir)
{
    struct MotorDefinition &motor = _motor(mot);

    switch (dir) {
    case CW:
        digitalWrite(motor.pwmPin, HIGH);
        digitalWrite(motor.dirPin, HIGH);
        break;
    case CCW:
        digitalWrite(motor.pwmPin, HIGH);
        digitalWrite(motor.dirPin, LOW);
        break;
    case OFF:
        digitalWrite(motor.pwmPin, LOW);
        digitalWrite(motor.dirPin, LOW);
        break;
    }
}
