/**
 * @file color_stopper.cpp
 * @brief ブロック置き場の色を読み込むと終了判定
 *
 * @author Suguru Kouchi
 */
#include "color_stopper.hpp"
#include "robo_meta_datas.hpp"
#include "util.h"
namespace ie {

ColorStopper::ColorStopper():
colorSensor_(COLOR_SENSOR_PORT), colorJudge_(hsvConverter_) {
}

bool ColorStopper::doStop() {
    colorSensor_.getRawColor(rgb_);
    colorid_t color = colorJudge_.getColorNumber(rgb_.r, rgb_.g, rgb_.b);

    if (color == COLOR_BLACK || color == COLOR_WHITE || color == COLOR_NONE) {
        return false;
    }
    return true;
}

}
