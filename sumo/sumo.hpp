/**
 * @file sumo.hpp
 * @author Suguru Kouchi
 */
#ifndef SUMO_HPP
#define SUMO_HPP

#include <SonarSensor.h>
#include <ColorSensor.h>
#include "motion.hpp"

namespace ie {

class Sumo {
public:
    Sumo(ev3api::SonarSensor& sonarSensor, ev3api::ColorSensor& colorSensor, float target);
    void trainWait(Motion& motion, int n);
    void moveTocross(Motion& motion, Localization* localization, double lineThreshold);
    void turnFirstBlock(Motion& motion, Localization* localization);
    void moveBlock(Motion& motion, int markerNum);

private:
    ev3api::SonarSensor& sonarSensor_;
    ev3api::ColorSensor& colorSensor_;
    float target_;
    colorid_t markers_[8];
    static constexpr double TRAIN_WAIT_TIME = 1000; // 目の前を電車が通った時に停止しておく時間(ms)
    static constexpr double KIRI_DISTANCE   = 70.0; // 押し切りの距離(mm)
    static constexpr double DASHI_DISTANCE  = 115.0; // 押し出しの距離(mm)
};

}

#endif
