/**
 * @file calibration.cpp
 * @brief キャリブレーション<br>
 * 二つの反射光の平均値を求めます。
 *
 * @author Suguru Kouchi
 */
#include "calibration.hpp"
#include <cmath>
#include <Clock.h>
#include "util.h"
#include "robo_meta_datas.hpp"

namespace ie {

Calibration::Calibration():colorSensor_(COLOR_SENSOR_PORT), touchSensor_(TOUCH_SENSOR_PORT) {
}

/**
 * 二回反射光を図り、その"合計値"を返す。<br>
 * 平均値ではないので注意。<br>
 * 反射光を図る際はタッチボタンを押す。
 *
 * @return 2回の測定値の反射光の合計
 */
int Calibration::calibrate() {
    msg_f("First brightness", 1);
    int firstBrightness = pushTouchButton(2);
    msg_f(firstBrightness, 2);

    dly_tsk(300); // 少し止めないとちょん押しで次のやつまで入力される

    msg_f("Second brightness", 4);
    int secondBrightness = pushTouchButton(5);
    msg_f(secondBrightness, 5);

    return firstBrightness + secondBrightness;
}

/**
 * float calibrate()と同じ。参照か戻り値かの違い。<br>
 * 二回反射光を図り、その"合計値"を返す。<br>
 * 平均値ではないので注意。<br>
 * 反射光を図る際はタッチボタンを押す。
 *
 * @param target 2回の測定値の反射光の平均を収納する変数
 */
void Calibration::calibrate(int& target) {
    target = calibrate();
}

/**
 * 中央のボタンを押した瞬間のRGB値合計を返す。
 *
 * @param line 取得値を表示する行
 * @return RGB値合計
 */
int Calibration::pushCenterButton(int line) {
    ev3api::Clock clock;
    rgb_raw_t rgb;
    while (true) {
        if (500 < clock.now()) {
            colorSensor_.getRawColor(rgb);
            msg_f(rgb.r + rgb.g + rgb.b, line);
            clock.reset();
        }
        if (ev3_button_is_pressed(ENTER_BUTTON)) {
            colorSensor_.getRawColor(rgb);
            return rgb.r + rgb.g + rgb.b;
        }
    }
}

/**
 * タッチボタンを押した瞬間のRGB値合計を返す。
 *
 * @param line 取得値を表示する行
 * @return RGB値合計
 */
int Calibration::pushTouchButton(int line) {
    ev3api::Clock clock;
    rgb_raw_t rgb;
    while (true) {
        if (500 < clock.now()) {
            colorSensor_.getRawColor(rgb);
            msg_f(rgb.r + rgb.g + rgb.b, line);
            clock.reset();
        }
        if (touchSensor_.isPressed()) {
            colorSensor_.getRawColor(rgb);
            return rgb.r + rgb.g + rgb.b;
        }
    }
}

}
