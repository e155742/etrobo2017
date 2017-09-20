/**
 * @file angle_stopper.hpp
 * @author Suguru Kouchi
 */
#ifndef ANGLE_STOPPER_HPP
#define ANGLE_STOPPER_HPP

#include "stopper.hpp"
#include <Motor.h>
#include "robo_meta_datas.hpp"

namespace ie {

class AngleStopper : public Stopper {
public:
    AngleStopper();
    AngleStopper(double targetAngle);
    bool doStop();
    void setAngle(double targetAngle);

private:
    ev3api::Motor leftWheel_;
    ev3api::Motor rightWheel_;
    int32_t beginLeftCount_  = 0;
    int32_t beginRightCount_ = 0;
    int32_t targetWheelAngle_;

};

}

#endif
