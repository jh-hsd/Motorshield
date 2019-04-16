// L298N motor control library

#ifndef MOTORSHIELD_h
#define MOTORSHIELD_h

class MotorShield {
    public:
        enum State {
            OFF, // motor off
            CW,  // turn clock-wise
            CCW, // turn counter clock wise
        };

        enum Motor {
            MOT_A, // motor A
            MOT_B,  // motor B
        };

        enum Type {
            DC_MOT, // Dc motor
            STEPPER,  // stepper motor
        };

        /* initialize motor class for DC motors
           dirA: direction pin motor A
           pwmA: pwm pin motor A
           minPwmA: min. PWM value (0-255) when the motor
           starts turning
           dirB: direction pin motor B
           pwmB: pwm pin motor B
           minPwmB: min. PWM value (0-255) when the motor
           starts turning
        */
        MotorShield(int dirAPin, int pwmAPin, int minPwmA,
                    int dirBPin, int pwmBPin, int minPwmB);

        /* initialize motor class for stepper motors
           dirA: direction pin coil A
           pwmA: pwm pin coil A
           dirB: direction pin coil B
           pwmB: pwm pin coil B
        */
        MotorShield(int dirAPin, int pwmAPin,
                    int dirBPin, int pwmBPin);

        void setDirection(MotorShield::Motor mot, MotorShield::State dir);
        MotorShield::State direction(MotorShield::Motor mot) const;

        /* adjust speed
           mot: Motor selection
           speed: PWM duty cycle (0-255)
           duration: speed ramp is in milli seconds */
        void setRawSpeed(MotorShield::Motor mot, int speed, int duration = 0);
        /* adjust speed
           mot: Motor selection
           speed: in percent (0-100)
           duration: speed ramp is in milli seconds */
        void setSpeed(MotorShield::Motor mot, int speed, int duration = 0);
        /* returns either raw speed value (duty-cycle) or
           percent value depending on the setSpeed method used */
        int speed(MotorShield::Motor mot) const;

        // do one step in the given direction
        void step(MotorShield::State dir);
        // do n steps in the given direction
        void steps(MotorShield::State dir, int steps);

    private:
        struct MotorDefinition {
            MotorShield::Type type;
            int dirPin;
            int pwmPin;
            int minPwm;
            MotorShield::State dir;
            int speed;
        };

        struct MotorShield::MotorDefinition &_motor(MotorShield::Motor mot);
        const struct MotorShield::MotorDefinition &_const_motor(MotorShield::Motor mot) const;
        int _dutyCycleFromPercent(struct MotorShield::MotorDefinition &motor);
        void _off(struct MotorShield::MotorDefinition &motor);
        void _update(struct MotorShield::MotorDefinition &motor);
        bool _isStepper() const;
        bool _isDC(MotorShield::Motor mot) const;
        void _setCoil(MotorShield::Motor mot, MotorShield::State cs);

        // pins
        struct MotorDefinition _motA;
        struct MotorDefinition _motB;

        int _stepPos; // stepper motor current position
};

#endif
