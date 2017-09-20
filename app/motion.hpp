/**
 * @file motion.hpp
 * @author Suguru Kouchi
 */
#ifndef MOTION_HPP
#define MOTION_HPP

#include <Motor.h>
#include <Steering.h>
#include <ColorSensor.h>

#include "localization.hpp"
#include "control.hpp"
#include "stopper.hpp"

namespace ie {

class Motion {
public:
    Motion();
    void rotateTail(int degree, int pwm);
    void raiseArm(int degree, int pwm);
    void stop();

    void setLeftPwm(int pwm);
    void setLeftPwm(Stopper& stopper, int pwm);

    void setRightPwm(int pwm);
    void setRightPwm(Stopper& stopper, int pwm);

    void setBothPwm(int leftPwm, int lightPwm);
    void setBothPwm(Stopper& stopper, int leftPwm, int lightPwm);

    void setSteeringPower(int pwm, int turnRatio);
    void setSteeringPower(Stopper& stopper, int pwm, int turnRatio);

    void goStraight(Control& control, int pwm);
    void goStraight(Stopper& stopper, Control& control, int pwm);

    void spin(Control& control, int pwm);
    void spin(Stopper& stopper, Control& control, int pwm);

    void goPoint(Localization& l, Control& control, int pwm, point_t pointX, point_t pointY);

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
    const double margineForGoPpont = 15.0;
    void onoffSetPwm(Control& control, int pwm);
    void goStraightHelper(Control& control, int pwm);
    void spinHelper(Control& control, int pwm);
    void lineTraceHelper(Control& control, int pwm, bool isRightSide);

};

}

#endif
