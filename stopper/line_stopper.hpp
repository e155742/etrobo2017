/**
 * @file line_stopper.hpp
 * @author Suguru Kouchi
 */
#ifndef LINE_STOPPER_HPP
#define LINE_STOPPER_HPP

#include "stopper.hpp"
#include <ColorSensor.h>

namespace ie {

class LineStopper : public Stopper {
public:
    LineStopper();
    LineStopper(double taigetThreshold);
    bool doStop();
    void setTaigetThreshold(double taigetThreshold);

private:
    ev3api::ColorSensor colorSensor_;
    double taigetThreshold_;
    rgb_raw_t rgb_;

};

}

#endif
