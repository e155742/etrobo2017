/**
 * @file color_stopper.cpp
 * @brief ブロック置き場の色を読み込むと終了判定
 *
 * @author Suguru Kouchi
 */
#include "color_stopper.hpp"
#include "robo_meta_datas.hpp"

namespace ie {

ColorStopper::ColorStopper():
colorSensor_(COLOR_SENSOR_PORT), colorJudge_(hsvConverter_) {
}

bool ColorStopper::doStop() {
    rgb_raw_t rgb;
    colorSensor_.getRawColor(rgb);
    colorid_t color = colorJudge_.getColorNumber(rgb.r, rgb.g, rgb.b);

    if (color == COLOR_BLACK || color == COLOR_WHITE) {
        return false;
    }
    return true;
}

}
