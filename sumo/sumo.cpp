/**
 * @file sumo.cpp
 * @brief ET相撲Neo
 *
 * @author Suguru Kouchi
 */
#include "sumo.hpp"

#include "onoff_control.hpp"
#include "pid_control.hpp"

#include "angle_stopper.hpp"
#include "mileage_stopper.hpp"
#include "line_stopper.hpp"

#include "robo_meta_datas.hpp"
#include "util.h"

namespace ie {

Sumo::Sumo(ev3api::SonarSensor& sonarSensor, float target):
sonarSensor_(sonarSensor), target_(target) {
}

/**
 * 電車が通り過ぎるまで動きを停止
 *
 * @param n 残りの跨ぐレール
 */
void Sumo::trainWait(Motion& motion, int n) {
    motion.stop();
    motion.raiseArm(45, 15); // 線路にアームが引っかからないように
    while (true) {
        int16_t distance = sonarSensor_.getDistance(); // 距離を測る
        if (0 < distance && distance < 60) {
            // 目の前を通っているので通り過ぎるまで待機
            while (sonarSensor_.getDistance() < 60) {}
            motion.wait(TRAIN_WAIT_TIME);
            break;
        }
        if (distance < 60 * n) {
            break;
        }
    }
}

/**
 * 直角検知<br>
 * ライントレース中から直角検知まで
 *
 * `parama lineThreshold 直角検知の閾値
 */
void Sumo::moveTocross(Motion& motion, double lineThreshold) {
    ie::OnOffControl stControl(0, 0.3, 0);
    ie::PIDControl ltControl(target_, 0.15, 0, 0);

    ie::LineStopper ls(lineThreshold);
    ie::AngleStopper as;
    ie::MileageStopper ms;

    ms.setTargetMileage(50);
    motion.lineTrace(ms, ltControl, 15, false);  // 車体をまっすぐにする
    motion.lineTrace(ls, ltControl, 20, false);  // 直角までライントレース 少し早いくらいが車体がブレない
    ms.setTargetMileage(ie::OFF_SET + 10);
    motion.goStraight(ms, ltControl, 10);        // トレッド軸にラインが来るように移動
    motion.wait(200); // 念のため停止
}

}
