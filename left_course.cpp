#include "left_course.hpp"

#include <vector>
#include <cmath>

#include "pid_control.hpp"
#include "onoff_control.hpp"

#include "mileage_stopper.hpp"
#include "angle_stopper.hpp"
#include "line_stopper.hpp"
#include "gray_stopper.hpp"
#include "direction_stopper.hpp"

#include "decoder.hpp"
#include "block_field.hpp"

#include "robo_meta_datas.hpp"

extern ie::Localization* localization;

/**
 * 韋駄天
 */
void LCourseIdaten(ie::Motion& motion) {
    // 第2ゲートへ向けて右旋回(バック)
    dly_tsk(1); // これがないとフリーズする。
    ie::DirectionStopper ds(*localization);
    ds.setTargetDirection(58.0 * M_PI / 180.0);
    motion.setSteeringPower(ds, -100, -55);
    // 第2ゲートを通過(バック)
    ie::MileageStopper ms(-800);
    ie::OnOffControl stControl(0, 0.3, 0);
    motion.goStraight(ms, stControl, -100);

    // 第1ゲートへ向けて左旋回(バック)
    ds.setTargetDirection(0.0 * M_PI / 180.0);
    motion.setSteeringPower(ds, -100, 58);
    // 第1ゲートを通過(バック)
    ms.setTargetMileage(-1420);
    motion.goStraight(ms, stControl, -100);

    // バックストレート前のコーナまで前進
    ms.setTargetMileage(500);
    motion.goStraight(ms, stControl, 100);
    // バックストレート前のコーナを通過
    ds.setTargetDirection(-90.0 * M_PI / 180.0);
    motion.setSteeringPower(ds, 100, -50);
    // バックストレートからゴールへ
    // stControl.setCoefficient(0, 0.3, 0);
    ms.setTargetMileage(2500);
    // motion.goPoint(*localization, stControl, 50, localization->getPointX() - 3500, localization->getPointY() - 300, 1000);
    motion.goStraight(ms, stControl, 100);

    msg_f("RETURN", 10);
    motion.wait(2000);
    ds.setTargetDirection(0.0);
    stControl.setCoefficient(0, 0.3, 0);
    // motion.spin(ds, stControl, 15);
    ds.setTargetDirection(std::atan2(0.0 - localization->getPointX(), 0.0 - localization->getPointY()));
    motion.spin(ds, stControl, 15);
    stControl.setCoefficient(0, 0.3, 0);
    motion.goPoint(*localization, stControl, 60, 0.0, 0.0, 15);
}

void LCourseBlock(ie::Motion& motion, float target, ie::Decoder& decoder, int greenPosition) {
    dly_tsk(1); // 念のため
    ie::BlockField bf(*localization, decoder, greenPosition);
    int targetMarker = bf.getTargetMarker();
    msg_clear();
    msg_f(targetMarker, 1);

    bf.revisionBotPoint(9, 30);
}

/**
 * 縦列駐車
 */
void LCourseParking(ie::Motion& motion, float target) {
    ie::PIDControl ltControl(target, 0.15, 0, 0);
    ie::OnOffControl stControl(0, 0.3, 0);

    // 最終ラインを検知するまで前進。右側から侵入
    ie::LineStopper ls(80);
    motion.goStraight(ls, stControl, 30);
    // 車体がラインを跨ぐように少しだけ全身
    ie::MileageStopper ms(80);
    motion.goStraight(ms, stControl, 30);

    // ライントレースをするためにラインを検知するまで回転
    ls.setTaigetThreshold(80);
    motion.spin(ls, stControl, 15);
    // 車体とラインをまっすぐにするためにライントレース
    ms.setTargetMileage(250);
    motion.lineTrace(ms, ltControl, 15, false);
    // 灰色までライントレース
    ie::GrayStopper gs(500);
    motion.lineTrace(gs, ltControl, 15, false);

    // 左旋回で車庫に侵入
    ie::AngleStopper as(-45);
    motion.setRightPwm(as, 20);
    // 少し進む
    ms.setTargetMileage(200);
    motion.goStraight(ms, stControl, 20);
    // 右旋回で車庫と水平にする
    as.setAngle(50);
    motion.setLeftPwm(as, 20);
}
