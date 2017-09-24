#include "left_idaten.hpp"
#include "localization.hpp"

#include "onoff_control.hpp"
#include "mileage_stopper.hpp"
#include "direction_stopper.hpp"
// #include "angle_stopper.hpp"
#include "line_stopper.hpp"

#include "util.h"

extern ie::Localization* localization;

void rtb(ie::Motion& motion) {
    motion.wait(3000);
    ie::OnOffControl stControl(0, 0.3, 0);
    msg_f("RETURN", 10);
    motion.wait(2000);
    ie::DirectionStopper ds(*localization);
    ds.setTargetDirection(std::atan2(0.0 - localization->getPointX(), 0.0 - localization->getPointY()));
    motion.spin(ds, stControl, 15);
    motion.goPoint(*localization, stControl, 30, 0.0, 0.0, 15);
    motion.wait(100000);
}

/**
 * 韋駄天
 */
void LCourseIdaten(ie::Motion& motion) {
    dly_tsk(1); // これがないとフリーズする。
    ie::DirectionStopper ds(*localization);
    ie::MileageStopper ms;

    // 第2ゲートへ向けて右旋回(バック)
    ds.setTargetDirection(degToRad(58));
    motion.setSteeringPower(ds, -100, -55);
    // 第2ゲートを通過(バック)
    ms.setTargetMileage(-800);
    ie::OnOffControl stControl(0, 0.3, 0);
    motion.goStraight(ms, stControl, -100);

    // 第1ゲートへ向けて左旋回(バック)
    ds.setTargetDirection(degToRad(0));
    motion.setSteeringPower(ds, -100, 58);
    // 第1ゲートを通過(バック)
    ms.setTargetMileage(-1420);
    motion.goStraight(ms, stControl, -100);
    ms.setTargetMileage(-22);
    motion.goStraight(ms, stControl, -80);
    ms.setTargetMileage(-22);
    motion.goStraight(ms, stControl, -60);
    ms.setTargetMileage(-22);
    motion.goStraight(ms, stControl, -40);
    ms.setTargetMileage(-22);
    motion.goStraight(ms, stControl, -20);

    // バックストレート前のコーナまで前進
    ms.setTargetMileage(600);
    motion.goStraight(ms, stControl, 100);
    // バックストレート前のコーナを通過してストレートに向ける
    ds.setTargetDirection(degToRad(-88));
    motion.setSteeringPower(ds, 100, -60);
    // バックストレートからゴールへ
    stControl.setCoefficient(0, 0.4, 0);
    ms.setTargetMileage(2580);
    // motion.goPoint(*localization, stControl, 50, localization->getPointX() - 3500, localization->getPointY() - 250, 1000);
    motion.goStraight(ms, stControl, 100);

    // 終了
    motion.wait(100);

    // ブロック並べのためにLコースのラインに乗せる
    ds.setTargetDirection(0);
    motion.spin(ds, stControl, 20);
    ie::LineStopper ls(80);
    motion.goStraight(ls, stControl, 25);
    ms.setTargetMileage(ie::OFF_SET + 10);
    motion.goStraight(ms, stControl, 20);
    motion.stop();

}
