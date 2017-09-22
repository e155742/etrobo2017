/**
 * @file gray_stopper.cpp
 * @brief 灰色検知で終了判定
 *
 * @author Suguru Kouchi
 */
#include "gray_stopper.hpp"
#include "robo_meta_datas.hpp"

namespace ie {

GrayStopper::GrayStopper():
colorSensor_(COLOR_SENSOR_PORT), taigetThreshold_(0.0){
}

GrayStopper::GrayStopper(double taigetThreshold):
colorSensor_(COLOR_SENSOR_PORT), taigetThreshold_(taigetThreshold){
}

bool GrayStopper::doStop() {
    colorSensor_.getRawColor(rgb_);
    if (taigetThreshold_ < rgb_.r + rgb_.g + rgb_.b) {
        return true;
    }
    return false;
}

/**
 * 灰色と見なすRGB値合計の閾値を設定する。
 *
 * @param taigetThreshold 灰色と見なす閾値
 */
void GrayStopper::setTaigetThreshold(double taigetThreshold) {
    taigetThreshold_ = taigetThreshold;
}

}
