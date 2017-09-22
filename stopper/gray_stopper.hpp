/**
 * @file gray_stopper.hpp
 * @author Suguru Kouchi
 */
#ifndef GRAY_STOPPER_HPP
#define GRAY_STOPPER_HPP

#include "stopper.hpp"
#include <ColorSensor.h>

namespace ie {

class GrayStopper : public Stopper {
public:
    GrayStopper();
    GrayStopper(double taigetThreshold);
    bool doStop();
    void setTaigetThreshold(double taigetThreshold);

private:
    ev3api::ColorSensor colorSensor_;
    double taigetThreshold_;
    rgb_raw_t rgb_;

};

}

#endif
