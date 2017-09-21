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

Calibration::Calibration():colorSensor_(COLOR_SENSOR_PORT) {
}

/**
 * 二回反射光を図り、その"合計値"を返す。<br>
 * 平均値ではないので注意。<br>
 * 反射光を図る際は中央のボタンを押す。
 *
 * @return 2回の測定値の反射光の合計
 */
int Calibration::calibrate() {
    msg_f("First brightness", 1);
    int firstBrightness = pushCenterButton();
    msg_f("First brightness value", 4);
    msg_f(firstBrightness, 5);

    dly_tsk(300); // 少し止めないとちょん押しで次のやつまで入力される

    msg_f("Second brightness", 1);
    int secondBrightness = pushCenterButton();
    msg_f("Second brightness value", 7);
    msg_f(secondBrightness, 8);

    return firstBrightness + secondBrightness;
}

/**
 * float calibrate()と同じ。参照か戻り値かの違い。<br>
 * 二回反射光を図り、その平均値を返す。<br>
 * 反射光を図る際は中央のボタンを押す。
 *
 * @param target 2回の測定値の反射光の平均を収納する変数
 */
void Calibration::calibrate(int& target) {
    target = calibrate();
}

/**
 * 中央のボタンを押した瞬間の反射光を返す。
 *
 * @return 反射光
 */
int Calibration::pushCenterButton() {
    ev3api::Clock clock;
    rgb_raw_t rgb;
    while (true) {
        if (500 < clock.now()) {
            colorSensor_.getRawColor(rgb);
            msg_f(rgb.r + rgb.g + rgb.b, 2);
            clock.reset();
        }
        if (ev3_button_is_pressed(ENTER_BUTTON)) {
            colorSensor_.getRawColor(rgb);
            return rgb.r + rgb.g + rgb.b;
        }
    }
}

}
