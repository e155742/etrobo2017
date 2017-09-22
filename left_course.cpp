#include "left_course.hpp"

#include "pid_control.hpp"
#include "onoff_control.hpp"

#include "mileage_stopper.hpp"
#include "angle_stopper.hpp"
#include "line_stopper.hpp"
#include "gray_stopper.hpp"

extern ie::Localization* localization;

/**
 * 韋駄天
 */
void LCourseIdaten(ie::Motion& motion) {
    ie::AngleStopper as(90);
    motion.setSteeringPower(as, -100, -50); // 第2ゲートへ向けて右旋回(バック)

    // ie::MileageStopper ms(-500);
    ie::OnOffControl stControl(0, 0.3, 0);
    // motion.goStraight(ms, stControl, -100); // 第2ゲートを通過(バック)

    as.setAngle(-90);
    motion.setSteeringPower(as, -100, 53); // 第1ゲートへ向けて左旋回(バック)

    ie::MileageStopper ms(-1460);
    motion.goStraight(ms, stControl, -100); // 第1ゲートを通過(バック)

    ms.setTargetMileage(500);
    motion.goStraight(ms, stControl, 100); // バックストレート前のコーナまで前進

    as.setAngle(-90);
    motion.setSteeringPower(as, 100, -50); // バックストレート前のコーナを通過

    stControl.setCoefficient(0, 0.4, 0);
    ms.setTargetMileage(2500);
    motion.goStraight(ms, stControl, 100); // バックストレートからゴールへ

    msg_f("RETURN", 10);
    motion.wait(2000);
    ie::DirectionStopper ds(*localization);
    ds.setTargetDirection(std::atan2(0.0 - localization->getPointX(), 0.0 - localization->getPointY()));
    motion.spin(ds, stControl, 15);
    motion.goPoint(*localization, stControl, 30, 0.0, 0.0);
}

/**
 * 縦列駐車
 */
void LCourseParking(ie::Motion& motion, float target) {
    ie::PIDControl ltControl(target, 0.15, 0, 0);
    ie::OnOffControl stControl(0, 0.3, 0);

    ie::LineStopper ls(80);
    motion.goStraight(ls, stControl, 30); // 最終ラインを検知するまで前進。右側から侵入
    ie::MileageStopper ms(80);
    motion.goStraight(ms, stControl, 30); // 車体がラインを跨ぐように少しだけ全身

    ls.setTaigetThreshold(80);
    motion.spin(ls, stControl, 15); // ライントレースをするためにラインを検知するまで回転

    ms.setTargetMileage(250);
    motion.lineTrace(ms, ltControl, 15, false); // 車体とラインをまっすぐにするためにライントレース
    ie::GrayStopper gs(500);
    motion.lineTrace(gs, ltControl, 15, false); // 灰色までライントレース

    ie::AngleStopper as(-45);
    motion.setRightPwm(as, 20); // 左旋回で車庫に侵入
    ms.setTargetMileage(200);
    motion.goStraight(ms, stControl, 20); // 少し進む
    as.setAngle(50);
    motion.setLeftPwm(as, 20); // 右旋回で車庫と水平にする
}
