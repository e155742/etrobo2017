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
#include "direction_stopper.hpp"

#include "hsv_converter.hpp"
#include "color_judge.hpp"

#include "localization.hpp"
#include "robo_meta_datas.hpp"
#include "util.h"

namespace ie {


// ブロックの並びは手前の土俵から時計回りの順番で
// (8)---(5)
//     |
// (7)---(6)
//     |
// (4)---(1)
//     |
// (3)---(2)

Sumo::Sumo(ev3api::SonarSensor& sonarSensor, ev3api::ColorSensor& colorSensor, float target):
sonarSensor_(sonarSensor), colorSensor_(colorSensor), target_(target) {
    int index = -1;

    markers_[++index] = COLOR_YELLOW; // 黄
    markers_[++index] = COLOR_BLUE;   // 青
    markers_[++index] = COLOR_RED;    // 赤
    markers_[++index] = COLOR_GREEN;  // 緑

    markers_[++index] = COLOR_BLUE;   // 青
    markers_[++index] = COLOR_YELLOW; // 黄
    markers_[++index] = COLOR_GREEN;  // 緑
    markers_[++index] = COLOR_RED;    // 赤
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
void Sumo::moveTocross(Motion& motion, Localization* localization, double lineThreshold) {
    ie::OnOffControl stControl(0, 0.3, 0);
    ie::PIDControl ltControl(target_, 0.15, 0, 0);

    ie::LineStopper ls(lineThreshold);
    ie::AngleStopper as;
    ie::MileageStopper ms;

    ms.setTargetMileage(50);
    motion.lineTrace(ms, ltControl, 15, false);  // 車体をまっすぐにする
    motion.lineTrace(ls, ltControl, 25, false);  // 直角までライントレース 少し早いくらいが車体がブレない
    ms.setTargetMileage(ie::OFF_SET + 10);
    localization->setDirection(0.0);             // 方位を0に
    motion.goStraight(ms, stControl, 10);        // トレッド軸にラインが来るように移動
    motion.wait(200); // 念のため停止
}

void Sumo::turnFirstBlock(Motion& motion, Localization* localization) {
    ie::DirectionStopper ds(*localization);
    ie::OnOffControl stControl(0, 0.3, 0);

    ds.setTargetDirection(std::atan2(230, 150));
    motion.spin(ds, stControl, 20);
    motion.stop();
}

void Sumo::moveBlock(Motion& motion, int markerNum) {
    ie::OnOffControl stControl(0, 0.3, 0);
    ie::MileageStopper ms;
    HsvConverter hsvConverter;
    ColorJudge colorJudge(hsvConverter);
    rgb_raw_t rgb;

    colorSensor_.getRawColor(rgb);

    int pwm = 15;
    if (markers_[markerNum] == colorJudge.getColorNumber(rgb.r, rgb.g, rgb.b)) {
        // 寄り切り
        ms.setTargetMileage(KIRI_DISTANCE);
        motion.goStraight(ms, stControl, pwm);
        motion.wait(200);
        ms.setTargetMileage(-KIRI_DISTANCE);
        motion.goStraight(ms, stControl, -pwm);
        motion.stop();
    } else {
        // 押し出し
        ms.setTargetMileage(DASHI_DISTANCE);
        motion.goStraight(ms, stControl, pwm);
        motion.wait(200);
        ms.setTargetMileage(-DASHI_DISTANCE);
        motion.goStraight(ms, stControl, -pwm);
        motion.stop();
    }
}

}
