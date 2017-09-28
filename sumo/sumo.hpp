/**
 * @file sumo.hpp
 * @author Suguru Kouchi
 */
#ifndef SUMO_HPP
#define SUMO_HPP

#include <SonarSensor.h>
#include "motion.hpp"

namespace ie {

class Sumo {
public:
    Sumo(ev3api::SonarSensor& sonarSensor, Motion motion);
    void trainWait(int n);

private:
    ev3api::SonarSensor& sonarSensor_;
    Motion& motion_;
    static constexpr double TRAIN_WAIT_TIME = 1000; // 目の前を電車が通った時に停止しておく時間(ms)
};

}

#endif
