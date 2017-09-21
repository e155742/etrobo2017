/**
 * @file calibration.hpp
 * @author Suguru Kouchi
 */
#ifndef CALIBRATION_HPP
#define CALIBRATION_HPP

#include <ColorSensor.h>

namespace ie {

class Calibration {
public:
    Calibration();
    int calibrate();
    void calibrate(int& target);

private:
    ev3api::ColorSensor colorSensor_;
    int pushCenterButton();
};

}

#endif
