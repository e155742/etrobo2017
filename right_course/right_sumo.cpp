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

void RCourseSumo(ie::Motion& motion, float target, float target2, ev3api::SonarSensor& sonarSensor, ev3api::ColorSensor& colorSensor) {
    const int crossTarget = 110;

    ie::Sumo sumo(sonarSensor, colorSensor, target2);
    ie::OnOffControl stControl(0, 0.3, 0);
    ie::PIDControl ltControl(target2, 0.15, 0, 0);
    ie::MileageStopper ms;
    ie::AngleStopper as;
    ie::LineStopper ls(target2);
    ie::DirectionStopper ds(*localization);

    sumo.trainWait(motion, 3);

    // 土俵の上に移動
    ms.setTargetMileage(400);
    motion.goStraight(ms, stControl, 50);
    motion.stop();
    motion.raiseArm(15, 15);

    // ライントレース準備
    as.setAngle(-30);
    motion.spin(as, stControl, -30);
    motion.spin(ls, stControl, 10);

    // 手前から2番目の直角まで移動
    dly_tsk(1);
    sumo.moveTocross(motion, localization, crossTarget);

    // 前半の土俵の中心まで戻る
    ms.setTargetMileage(-150);
    motion.goStraight(ms, stControl, -15);
    motion.wait(200); // 念のため停止

    motion.raiseArm(85, 15);

    // (0)のブロック
    dly_tsk(1);
    sumo.turnToBlock(motion, localization, 0);
    sumo.moveBlock(motion, 0, 138);

    // (1)のブロック
    dly_tsk(1);
    sumo.turnToBlock(motion, localization, 1);
    sumo.moveBlock(motion, 1, 138);

    // (2)のブロック
    dly_tsk(1);
    sumo.turnToBlock(motion, localization, 2);
    sumo.moveBlock(motion, 2, 118);

    // (3)のブロック
    dly_tsk(1);
    sumo.turnToBlock(motion, localization, 3);
    sumo.moveBlock(motion, 3, 118);

    // 真正面を向く
    // ds.setTargetDirection(0.0);
    // motion.spin(ds, stControl, 15);
    motion.raiseArm(15, 15);
    motion.spin(ls, stControl, 10);

    // 電車手前まで移動
    ms.setTargetMileage(30);
    motion.lineTrace(ms, ltControl, 15, false);
    ls.setTaigetThreshold(crossTarget);
    motion.lineTrace(ls, ltControl, 15, false);  // 直角までライントレース
    ms.setTargetMileage(25);
    motion.goStraight(ms, stControl, 15);        // 直角は直進
    ms.setTargetMileage(10);
    motion.lineTrace(ms, ltControl, 15, false);  // ギリギリまでライントレース
    motion.stop();
    motion.raiseArm(45, 15);                     // 念のため引っかからないようにアームを上げておく

    // (4)のブロック

    // (5)のブロック

    // (6)のブロック

    // (7)のブロック


}
