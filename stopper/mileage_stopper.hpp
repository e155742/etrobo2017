/**
 * @file mileage_stopper.hpp
 * @author Suguru Kouchi
 */
#ifndef MILEAGE_STOPPER_HPP
#define MILEAGE_STOPPER_HPP

#include "stopper.hpp"
#include <Motor.h>

namespace ie {

class MileageStopper : public Stopper {
public:
    // MileageStopper(ev3api::Motor leftWheel, ev3api::Motor rightWheel);
    MileageStopper();
    MileageStopper(double targetMileage);
    void setMileage(double targetMileage);
    void resetMileage();
    bool doStop() const;

private:
    ev3api::Motor leftWheel_;
    ev3api::Motor rightWheel_;
    double beginMileage_ = 0.0;
    double targetMileage_;
    double getMileage() const;

};

}

#endif
