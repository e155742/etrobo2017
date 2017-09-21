/**
 * @starter.cpp
 * @brief タッチボタンを押すとスタートさせる。
 *
 * @author Suguru Kouchi
 */
#include "starter.hpp"
#include "robo_meta_datas.hpp"

namespace ie {

Starter::Starter():
touchSensor_(TOUCH_SENSOR_PORT) {
}

void Starter::startWait() {
    dly_tsk(300); // 少し止めないとちょん押しで前やつが入力される。
    while (true) {
        if (touchSensor_.isPressed()) {
            return;
        }
    }
}

}
