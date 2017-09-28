/**
 * @file sumo.cpp
 * @brief ET鐩告挷Neo
 *
 * @author Suguru Kouchi
 */
#include "sumo.hpp"
#include "robo_meta_datas.hpp"
#include "util.h"

namespace ie {

Sumo::Sumo(ev3api::SonarSensor& sonarSensor, Motion motion):
sonarSensor_(sonarSensor), motion_(motion) {
}

/**
 * 電車が通り過ぎるまで動きを停止
 *
 * @param n 残りの跨ぐレール
 */
void Sumo::trainWait(int n) {
    motion_.raiseArm(45, 15); // 線路にアームが引っかからないように
    motion_.stop();
    while (true) {
        int16_t distance = sonarSensor_.getDistance(); // 距離を測る
        if (0 < distance && distance < 60) {
            // 目の前を通っているので通り過ぎるまで待機
            while (sonarSensor_.getDistance() < 60) {}
            motion_.wait(TRAIN_WAIT_TIME);
            break;
        }
        if (distance < 60 * n) {
            break;
        }
    }
}

}
