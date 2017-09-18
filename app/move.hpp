/**
 * @file move.hpp
 * @author Suguru Kouchi
 */
#ifndef MOVE_HPP
#define MOVE_HPP

#include <cmath>
#include <Motor.h>
#include <Steering.h>
#include <ColorSensor.h>
#include "control.hpp"
#include "localization.hpp"

namespace ie {

class Move {
public:
    Move();
    void rotateTail(int degree, int pwm);
    void raiseArm(int degree, int pwm);
    void stop();
    void left(int pwm);
    void right(int pwm);
    void steering(int power, int turnRatio);
    void goStraight(Control& control, float distance /* mm */, int pwm);
    void goStraight(Control& control, int pwm);
    void spin(Control& control, int degree, int pwm);
    void goPoint(Control& control, Control& spinControl, Localization& localization,
                 point_t pointX, point_t pointY, int pwm, int spinPwm);
    void lineTrace(Control& control, int pwm, bool isRightSide);
    void oldLineTrace(Control& control, float threshold, int pwm, bool isRightSide);
    float getMileage() const;

private:
    ev3api::Motor leftWheel_;
    ev3api::Motor rightWheel_;
    ev3api::Steering steering_;
    ev3api::Motor tail_;
    ev3api::Motor arm_;
    ev3api::ColorSensor colorSensor_;
    void spinForGoPoint(Control& control, Localization& localization,
                        point_t pointX, point_t pointY, point_t& diffRadian, int32_t beginLeftCount, int32_t beginRightCount, int spinPwm);
    void showControlData(int brightness, int controlValue) const;
};

}

#endif
