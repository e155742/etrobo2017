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
#include "util.h"

namespace ie {

class Motion {
public:
    Motion();
    void rotateTail(int degree, int pwm);
    void raiseArm(int degree, int pwm);
    void stop();

    void left(int pwm);
    void left(Stopper& stopper, int pwm);

    void right(int pwm);
    void right(Stopper& stopper, int pwm);

    void wheel(int leftPwm, int lightPwm);
    void wheel(Stopper& stopper, int leftPwm, int lightPwm);

    void steering(int pwm, int turnRatio);
    void steering(Stopper& stopper, int pwm, int turnRatio);

    void goStraight(Control& control, int pwm);
    void goStraight(Stopper& stopper, Control& control, int pwm);

    void spin(Control& control, int pwm);
    void spin(Stopper& stopper, Control& control, int pwm);

private:
    ev3api::Motor leftWheel_;
    ev3api::Motor rightWheel_;
    ev3api::Steering steering_;
    ev3api::Motor tail_;
    ev3api::Motor arm_;
    ev3api::ColorSensor colorSensor_;
    void onoffSetPwm(Control& control, int pwm);
    void goStraightHelper(Control& control, int pwm);
    void spinHelper(Control& control, int pwm);
};

}

#endif
