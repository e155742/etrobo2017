/**
 * @file line_stopper.cpp
 * @brief ライン検知で終了判定
 *
 * @author Suguru Kouchi
 */
#include "line_stopper.hpp"
#include "robo_meta_datas.hpp"

namespace ie {

LineStopper::LineStopper():
colorSensor_(COLOR_SENSOR_PORT), taigetThreshold_(0.0){
}

LineStopper::LineStopper(double taigetThreshold):
colorSensor_(COLOR_SENSOR_PORT), taigetThreshold_(taigetThreshold){
}

bool LineStopper::doStop() {
    colorSensor_.getRawColor(rgb_);
    if (rgb_.r + rgb_.g + rgb_.b < taigetThreshold_) {
        return true;
    }
    return false;
}

/**
 * ライン上と見なすRGB値合計の閾値を設定する。
 *
 * @param taigetThreshold ライン上と見なす閾値
 */
void LineStopper::setTaigetThreshold(double taigetThreshold) {
    taigetThreshold_ = taigetThreshold;
}

}
