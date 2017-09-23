#include "left_idaten.hpp"
#include "localization.hpp"

#include "onoff_control.hpp"
#include "mileage_stopper.hpp"
#include "direction_stopper.hpp"
// #include "angle_stopper.hpp"

#include "util.h"

extern ie::Localization* localization;

/**
 * 韋駄天
 */
void LCourseIdaten(ie::Motion& motion) {
    // 第2ゲートへ向けて右旋回(バック)
    dly_tsk(1); // これがないとフリーズする。
    ie::DirectionStopper ds(*localization);
    ds.setTargetDirection(degToRad(58));
    motion.setSteeringPower(ds, -100, -55);
    // 第2ゲートを通過(バック)
    ie::MileageStopper ms(-800);
    ie::OnOffControl stControl(0, 0.3, 0);
    motion.goStraight(ms, stControl, -100);

    // 第1ゲートへ向けて左旋回(バック)
    ds.setTargetDirection(degToRad(0));
    motion.setSteeringPower(ds, -100, 58);
    // 第1ゲートを通過(バック)
    ms.setTargetMileage(-1420);
    motion.goStraight(ms, stControl, -100);

    // バックストレート前のコーナまで前進
    ms.setTargetMileage(500);
    motion.goStraight(ms, stControl, 100);
    // バックストレート前のコーナを通過
    ds.setTargetDirection(degToRad(-75));
    motion.setSteeringPower(ds, 100, -50);
    // バックストレートからゴールへ
    // stControl.setCoefficient(0, 0.3, 0);
    ms.setTargetMileage(2500);
    // motion.goPoint(*localization, stControl, 50, localization->getPointX() - 3500, localization->getPointY() - 300, 1000);
    motion.goStraight(ms, stControl, 100);
}
