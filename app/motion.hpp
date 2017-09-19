/**
 * @file motion.hpp
 * @author Suguru Kouchi
 */
#ifndef MOTION_HPP
#define MOTION_HPP

#include <Motor.h>
#include <Steering.h>
#include <ColorSensor.h>

#include "control.hpp"
#include "stopper.hpp"

namespace ie {

class Motion {
public:
    Motion();
    void rotateTail(int degree, int pwm);
    void raiseArm(int degree, int pwm);
    void stop();

    void setPwmLeft(int pwm);
    void setPwmLeft(Stopper& stopper, int pwm);

    void setPwmRight(int pwm);
    void setPwmRight(Stopper& stopper, int pwm);

    void setPwmBoth(int leftPwm, int lightPwm);
    void setPwmBoth(Stopper& stopper, int leftPwm, int lightPwm);

    void setSteeringPower(int pwm, int turnRatio);
    void setSteeringPower(Stopper& stopper, int pwm, int turnRatio);

    void goStraight(Control& control, int pwm);
    void goStraight(Stopper& stopper, Control& control, int pwm);

    void spin(Control& control, int pwm);
    void spin(Stopper& stopper, Control& control, int pwm);

    void lineTrace(Control& control, int pwm, bool isRightSide);
    void lineTrace(Stopper& stopper, Control& control, int pwm, bool isRightSide);

private:
    ev3api::Motor leftWheel_;
    ev3api::Motor rightWheel_;
    ev3api::Steering steering_;
    ev3api::Motor tail_;
    ev3api::Motor arm_;
    ev3api::ColorSensor colorSensor_;
    rgb_raw_t rgb_;
    void onoffSetPwm(Control& control, int pwm);
    void goStraightHelper(Control& control, int pwm);
    void spinHelper(Control& control, int pwm);
    void lineTraceHelper(Control& control, int pwm, bool isRightSide);
};

}

#endif
