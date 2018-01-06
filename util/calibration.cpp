/**
 * @file calibration.cpp
 * @brief キャリブレーション<br>
 * 二つの反射光の平均値を求めます。
 *
 * @author Suguru Kouchi
 */
#include "calibration.hpp"
#include <algorithm>
#include <cmath>
#include <vector>
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
float Calibration::calibrate() {
    dly_tsk(300); // 少し止めないとちょん押しで次のやつまで入力される
    msg_f("First brightness", 1);
    float firstBrightness = pushTouchButton(2);
    soundBeep();
    msg_f(firstBrightness, 2);

    dly_tsk(300);
    msg_f("Second brightness", 4);
    float secondBrightness = pushTouchButton(5);
    soundBeep();
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
void Calibration::calibrate(float& target) {
    target = calibrate();
}

/**
 * タッチボタンか中央ボタンを押すとRGB値合計を返す。<br>
 * 押した瞬間からSAMPLE_NUM回値を取得し、その中央値を返す。
 *
 * @param line 取得値を表示する行
 * @return RGB値合計
 */
float Calibration::pushTouchButton(int line) {
    ev3api::Clock clock;
    rgb_raw_t rgb;
    while (true) {
        if (500 < clock.now()) {
            colorSensor_.getRawColor(rgb);
            msg_f(rgb.r + rgb.g + rgb.b, line);
            clock.reset();
        }
        if (touchSensor_.isPressed() || ev3_button_is_pressed(ENTER_BUTTON)) {
            std::vector<int> reflection_colors = {};
            for (int i = 0; i < SAMPLE_NUM; i++) {
                colorSensor_.getRawColor(rgb);
                reflection_colors.push_back(rgb.r + rgb.g + rgb.b);
                dly_tsk(DLY_TIME);
            }
            std::sort(reflection_colors.begin(), reflection_colors.end());
            if (SAMPLE_NUM % 2 == 1) {
                return reflection_colors[(SAMPLE_NUM - 1) / 2];
            } else {
                return (reflection_colors[(SAMPLE_NUM / 2) - 1] + reflection_colors[SAMPLE_NUM / 2]) / 2;
            }
        }
    }
}

}
