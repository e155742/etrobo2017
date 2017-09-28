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
    Sumo(ev3api::SonarSensor& sonarSensor, float target);
    void trainWait(Motion& motion, int n);
    void moveTocross(Motion& motion, double lineThreshold);

private:
    ev3api::SonarSensor& sonarSensor_;
    float target_;
    static constexpr double TRAIN_WAIT_TIME = 1000; // 目の前を電車が通った時に停止しておく時間(ms)
};

}

#endif
