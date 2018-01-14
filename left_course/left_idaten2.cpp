#include "left_idaten.hpp"
#include "localization.hpp"

#include "onoff_control.hpp"
#include "mileage_stopper.hpp"
#include "direction_stopper.hpp"
// #include "angle_stopper.hpp"
#include "line_stopper.hpp"

#include "util.h"

extern ie::Localization* localization;

/**
 * 韋駄天(白のみ)
 */
void LCourseIdaten2(ie::Motion& motion) {
    dly_tsk(1); // これがないとフリーズする。
    ie::OnOffControl stControl(0, 0.4, 0);
    ie::DirectionStopper ds(*localization);
    ie::MileageStopper ms;

    // 第1ゲートへ向けて右旋回(バック)
    ds.setTargetDirection(degToRad(20));
    motion.setBothPwm(ds, -50, -100);

    // 第1ゲートを通過(バック)
    int margenOne = 10;
    ms.setTargetMileage(-2280 - margenOne);
    motion.goStraight(ms, stControl, -100);
    ms.setTargetMileage(-20);
    motion.goStraight(ms, stControl, -80);
    ms.setTargetMileage(-20);
    motion.goStraight(ms, stControl, -60);
    ms.setTargetMileage(-20);
    motion.goStraight(ms, stControl, -40);
    ms.setTargetMileage(-20);
    motion.goStraight(ms, stControl, -20);
    motion.wait(100);

    // 第2ゲートへ向けて直進
    ms.setTargetMileage(1520 + margenOne);
    motion.goStraight(ms, stControl, 100);

    // 第2ゲートを通過
    int curveRate = 50;
    ds.setTargetDirection(degToRad(180));
    motion.setBothPwm(ds, curveRate, 100);

    // ストレートに向ける
    ds.setTargetDirection(degToRad(-90));
    motion.setBothPwm(ds, 100, curveRate + 5);

    // バックストレートからゴールへ
    // ms.setTargetMileage(1700);
    // motion.goStraight(ms, stControl, 100);
    while (localization->getPointX() > -3740.0) {
        motion.goStraight(stControl, 100);
    }

    motion.wait(1000 * 1000);

    // 終了
    motion.wait(100);
    motion.raiseArm(5, 5);
    // ブロック並べのためにLコースのラインに乗せる
    ds.setTargetDirection(0);
    motion.spin(ds, stControl, 20);
    ie::LineStopper ls(80);
    motion.goStraight(ls, stControl, 25);
    ms.setTargetMileage(ie::OFF_SET + 10);
    motion.goStraight(ms, stControl, 20);
    motion.spin(ls, stControl, -15);
    motion.stop();
}
