/**
 * @file color_stopper.hpp
 * @author Suguru Kouchi
 */
#ifndef COLOR_STOPPER_HPP
#define COLOR_STOPPER_HPP

#include "stopper.hpp"
#include <ColorSensor.h>
#include "hsv_converter.hpp"
#include "color_judge.hpp"

namespace ie {

class ColorStopper : public Stopper {
public:
    ColorStopper();
    bool doStop();

private:
    ev3api::ColorSensor colorSensor_;
    HsvConverter hsvConverter_;
    ColorJudge colorJudge_;
    rgb_raw_t rgb_;

};

}

#endif
