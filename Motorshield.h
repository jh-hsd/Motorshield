/* L298N motor control library */

#ifndef MOTORSHIELD_h
#define MOTORSHIELD_h

class MotorShield {
    public:
        enum State {
            OFF, /* motor off */
            CW,  /* turn clock-wise */
            CCW, /* turn counter clock wise */
        };

        enum Motor {
            MOT_A, /* motor A */
            MOT_B,  /* motor B */
        };

        /* initialize motor class
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

        void setDirection(Motor mot, MotorShield::State dir);
        MotorShield::State direction(Motor mot) const;

        /* adjust speed
           mot: Motor selection
           speed: PWM duty cycle (0-255)
           duration: speed ramp is in milli seconds */
        void setRawSpeed(Motor mot, int speed, int duration = 0);
        /* adjust speed
           mot: Motor selection
           speed: in percent (0-100)
           duration: speed ramp is in milli seconds */
        void setSpeed(Motor mot, int speed, int duration = 0);
        /* returns either raw speed value (duty-cycle) or 
           percent value depending on the setSpeed method used */
        int speed(Motor mot) const;
 
    private:
        struct MotorDefinition {
            int dirPin;
            int pwmPin;
            int minPwm;
            MotorShield::State dir;
            int speed;
        };

        struct MotorShield::MotorDefinition &_motor(Motor mot);
        const struct MotorShield::MotorDefinition &_const_motor(Motor mot) const;
        int _dutyCycleFromPercent(struct MotorShield::MotorDefinition &motor);
        void _off(struct MotorShield::MotorDefinition &motor);
        void _update(struct MotorShield::MotorDefinition &motor);

        /* pins */
        struct MotorDefinition _motA;
        struct MotorDefinition _motB;
};

#endif
