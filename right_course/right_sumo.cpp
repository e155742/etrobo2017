#include "right_sumo.hpp"

#include "onoff_control.hpp"
#include "pid_control.hpp"

#include "mileage_stopper.hpp"
#include "angle_stopper.hpp"
#include "line_stopper.hpp"
#include "direction_stopper.hpp"

#include "sumo.hpp"

#include "robo_meta_datas.hpp"
#include "util.h"

extern ie::Localization* localization;

void RCourseSumo(ie::Motion& motion, float target, ev3api::SonarSensor& sonarSensor) {
    ie::Sumo sumo(sonarSensor, motion);
    ie::OnOffControl stControl(0, 0.3, 0);
    ie::PIDControl ltControl(target, 0.15, 0, 0);
    ie::MileageStopper ms;
    ie::AngleStopper as;
    ie::LineStopper ls(110);
    ie::DirectionStopper ds(*localization);

    sumo.trainWait(3);

    // 土俵の上に移動
    ms.setTargetMileage(400);
    motion.goStraight(ms, stControl, 50);
    motion.stop();
    motion.raiseArm(15, 15);

    // ライントレース準備
    as.setAngle(-45);
    motion.spin(as, stControl, -30);
    motion.spin(ls, stControl, 15);

    // 2番目の直角までライントレース
    ms.setTargetMileage(50);
    motion.lineTrace(ms, ltControl, 15, false);  // 車体をまっすぐにする
    // localization->setDirection(0);   // 方位を0に
    motion.lineTrace(ls, ltControl, 20, false);  // 直角までライントレース 少し早いくらいが車体がブレない
    // ds.setTargetDirection(0.0);
    // motion.spin(ds, stControl, 5);
    ms.setTargetMileage(ie::OFF_SET + 10);
    motion.goStraight(ms, ltControl, 10);        // トレッド軸にラインが来るように移動
    motion.wait(200); // 念のため停止

    // 前半の土俵の中心まで戻る
    ms.setTargetMileage(-150);
    motion.goStraight(ms, ltControl, -15);
    motion.wait(200); // 念のため停止

    // 最初のブロック
    // ds.setTargetDirection(std::atan2(230, 300));
    // motion.spin(as, stControl, 20);
    // motion.stop();
    // ms.setTargetMileage(150);
    // motion.goStraight(ms, ltControl, 15);
    // motion.stop();
}
