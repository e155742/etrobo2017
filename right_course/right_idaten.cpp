#include "right_idaten.hpp"
#include "localization.hpp"

#include "onoff_control.hpp"
#include "pid_control.hpp"

#include "mileage_stopper.hpp"
#include "direction_stopper.hpp"
#include "angle_stopper.hpp"
#include "line_stopper.hpp"
#include "gray_stopper.hpp"

#include "util.h"

extern ie::Localization* localization;
/*
void rtb(ie::Motion& motion) {
    motion.wait(3000);
    ie::OnOffControl stControl(0, 0.3, 0);
    msg_f("RETURN", 10);
    motion.wait(2000);
    ie::DirectionStopper ds(*localization);
    ds.setTargetDirection(std::atan2(0.0 - localization->getPointX(), 0.0 - localization->getPointY()));
    motion.spin(ds, stControl, 15);
    motion.goPoint(*localization, stControl, 50, 0.0, 0.0, 30);
    motion.wait(100000);
}
*/

/**
 * 韋駄天
 */
void RCourseIdaten(ie::Motion& motion, float target) {
    dly_tsk(1); // これがないとフリーズする。
    ie::DirectionStopper ds(*localization);
    ie::MileageStopper ms;
    ie::LineStopper ls(target); // 80
    ie::OnOffControl stControl(0, 0.3, 0);
    ie::PIDControl ltControl(target, 0.1, 0, 0.001);
    ie::AngleStopper as;
    ie::GrayStopper gs(480); // 550

    // 第2ゲートへ向けて右旋回(バック)
    ds.setTargetDirection(degToRad(59));
    // motion.setRightPwm(ds, -100);
    motion.setBothPwm(ds, -18, -100);
    // 第2ゲートを通過(バック)
    ms.setTargetMileage(-1350);
    motion.goStraight(ms, stControl, -100);

    // 第1ゲートへ向けて左旋回(バック)
    ds.setTargetDirection(degToRad(0));
    motion.setSteeringPower(ds, -100, 65);

    // 第1ゲートを通過(バック)
    int margenOne = -80;
    ms.setTargetMileage(-1400 + margenOne); // 1400 // ********** 用調整 **********
    motion.goStraight(ms, stControl, -100);
    ms.setTargetMileage(-20);
    motion.goStraight(ms, stControl, -80);
    ms.setTargetMileage(-20);
    motion.goStraight(ms, stControl, -60);
    ms.setTargetMileage(-20);
    motion.goStraight(ms, stControl, -40);
    ms.setTargetMileage(-20);
    motion.goStraight(ms, stControl, -20);
    // ms.setTargetMileage(-1550);  // ********** 用調整 **********
    // motion.goStraight(ms, stControl, -100);

    // バックストレート前のコーナまで前進
    ms.setTargetMileage(500 - margenOne); // 600
    motion.goStraight(ms, stControl, 100);
    // 最終を通過してストレートに向ける
    ds.setTargetDirection(degToRad(-88));  // ********** 用調整 **********
    motion.setSteeringPower(ds, 100, -67); // 60
    // バックストレートからゴールへ
    stControl.setCoefficient(0, 0.4, 0);
    ms.setTargetMileage(2450);     // ********** 用調整 ********** // 2500
    // motion.goPoint(*localization, stControl, 50, localization->getPointX() - 3500, localization->getPointY() - 250, 1000);
    motion.goStraight(ms, stControl, 100);
    // 終了

    // プライマリーのラインに乗せる
    motion.raiseArm(15, 5);
    motion.goStraight(ls, stControl, 100);
    ms.setTargetMileage(ie::OFF_SET + 10);
    motion.goStraight(ms, stControl, 30);
    motion.spin(ls, stControl, -10);
    motion.stop();

    // Lコースをライントレース
    ms.setTargetMileage(250);
    motion.lineTrace(ms, ltControl, 20, true);
    motion.lineTrace(gs, ltControl, 20, true); // 灰色までLコースをライントレース
    motion.stop();
    as.setAngle(-90);
    motion.spin(as, stControl, -20);           // 車体をRコースに向ける
    motion.stop();

    // Rコースをライントレース
    ls.setTaigetThreshold(target);
    motion.goStraight(ls, stControl, 20);  // Rコースまで移動
    ms.setTargetMileage(ie::OFF_SET + 10);
    motion.goStraight(ms, stControl, 20);
    ltControl.setTarget(target);
    motion.spin(ls, stControl, 20);        // ライントレースのために回転
    ms.setTargetMileage(690);              // 線路の直前まで移動 740
    motion.lineTrace(ms, ltControl, 20, false);
    motion.stop();
}