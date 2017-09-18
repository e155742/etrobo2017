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
    MileageStopper();
    MileageStopper(double targetMileage);
    bool doStop() const;
    void setMileage(double targetMileage);

private:
    ev3api::Motor leftWheel_;
    ev3api::Motor rightWheel_;
    double beginMileage_ = 0.0;
    double targetMileage_;
    double getMileage() const;

};

}

#endif
