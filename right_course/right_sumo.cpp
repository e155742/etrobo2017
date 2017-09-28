#include "right_sumo.hpp"
#include "onoff_control.hpp"
#include "mileage_stopper.hpp"
#include "util.h"

extern ie::Localization* localization;

void RCourseSumo(ie::Motion& motion, ev3api::SonarSensor& sonarSensor) {
    const uint32_t waitTIme = 1500; // 電車が目の前を通ったとこの待機時間
    int16_t distance = 0;
    ie::OnOffControl stControl(0, 0.3, 0);
    ie::MileageStopper ms;

    // motion.raiseArm(15, 5);
    motion.raiseArm(45, 50); // 線路にアームが引っかからないように

    // 電車が通過するまでで待機
    motion.stop();
    while (true) {
        int16_t distance = sonarSensor.getDistance(); // 距離を測る
        if (0 < distance && distance < 60) {
            motion.wait(waitTIme); // 目の前を通っているので通り過ぎるまで待機
            break;
        }
        if (distance < 180) {
            break;
        }
    }

    // 土俵の上に移動
    ms.setTargetMileage(550);
    motion.goStraight(ms, stControl, 50);
    msg_f(distance, 11);
    motion.stop();

}
