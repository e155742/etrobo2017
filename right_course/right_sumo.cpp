#include "right_sumo.hpp"

#include "onoff_control.hpp"
#include "pid_control.hpp"

#include "mileage_stopper.hpp"
#include "angle_stopper.hpp"
#include "line_stopper.hpp"
#include "gray_stopper.hpp"
#include "direction_stopper.hpp"

#include "sumo.hpp"

#include "robo_meta_datas.hpp"
#include "util.h"

extern ie::Localization* localization;

void RCourseSumo(ie::Motion& motion, float target, float target2, ev3api::SonarSensor& sonarSensor, ev3api::ColorSensor& colorSensor) {
    const int crossTarget = target2 * 0.45; // 100

    ie::Sumo sumo(sonarSensor, colorSensor, target2);
    ie::OnOffControl stControl(0, 0.3, 0);
    ie::PIDControl ltControl(target, 0.1, 0, 0.001);
    ie::MileageStopper ms;
    ie::AngleStopper as;
    ie::GrayStopper gs(550);
    ie::LineStopper ls;
    ie::DirectionStopper ds(*localization);

    motion.raiseArm(15, 5);
    motion.stop();

    // 1枚目の土俵
    sumo.trainWait(motion, 2);
    ltControl.setTarget(target2); // ライントレースの閾値を土俵用に

    // 土俵の上に移動
    ms.setTargetMileage(460); // 400
    motion.goStraight(ms, stControl, 30);
    motion.stop();
    motion.raiseArm(15, 15);

    // ライントレース準備の回転
    as.setAngle(-30);
    motion.spin(as, stControl, -30);
    ls.setTaigetThreshold(target2);
    motion.spin(ls, stControl, 10);

    // 手前から2番目の直角まで移動
    dly_tsk(1);
    sumo.moveTocross(motion, localization, crossTarget);

    // 前半の土俵の中心まで戻る
    ms.setTargetMileage(-150);
    motion.goStraight(ms, stControl, -15);
    motion.wait(200); // 念のため停止

    // 色検知のためアームをあげる
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
    motion.raiseArm(15, 15);
    ls.setTaigetThreshold(target2);
    motion.spin(ls, stControl, 10);

    // 電車手前まで移動
    ms.setTargetMileage(40);
    motion.lineTrace(ms, ltControl, 15, false);
    ls.setTaigetThreshold(crossTarget);
    motion.lineTrace(ls, ltControl, 15, false);  // 直角までライントレース
    ms.setTargetMileage(ie::OFF_SET + 10);
    motion.goStraight(ms, stControl, 15);        // 直角は直進
    motion.stop();


    // 2枚目の土俵に移動
	// 超音波の検知のため少し退がる
    motion.wait(100);
    int back = 100;
    ms.setTargetMileage(-back);
    motion.goStraight(ms, stControl, -15);
	sumo.trainWait(motion, 2);
    ms.setTargetMileage(350+back);
    motion.goStraight(ms, stControl, 30);
    motion.stop();
    motion.raiseArm(15, 15);
    motion.stop();

    // ライントレース準備の回転
    as.setAngle(-30);
    motion.spin(as, stControl, -30);
    ls.setTaigetThreshold(target2);
    motion.spin(ls, stControl, 10);

    // 手前から4番目の直角まで移動
    dly_tsk(1);
    sumo.moveTocross(motion, localization, crossTarget);

    // 後半の土俵の中心まで戻る
    ms.setTargetMileage(-150);
    motion.goStraight(ms, stControl, -15);
    motion.wait(200); // 念のため停止

    // 色検知のためアームをあげる
    motion.raiseArm(85, 15);

    // (4)のブロック
    dly_tsk(1);
    sumo.turnToBlock(motion, localization, 0);
    sumo.moveBlock(motion, 4, 138);

    // (5)のブロック
    dly_tsk(1);
    sumo.turnToBlock(motion, localization, 1);
    sumo.moveBlock(motion, 5, 138);

    // (6)のブロック
    dly_tsk(1);
    sumo.turnToBlock(motion, localization, 2);
    sumo.moveBlock(motion, 6, 118);

    // (7)のブロック
    dly_tsk(1);
    sumo.turnToBlock(motion, localization, 3);
    sumo.moveBlock(motion, 7, 118);

    // 正面を向く
    motion.raiseArm(15, 15);
    ls.setTaigetThreshold(target2);
    motion.spin(ls, stControl, 10);

    // 電車手前まで移動
    ms.setTargetMileage(30);
    motion.lineTrace(ms, ltControl, 15, false);
    ls.setTaigetThreshold(crossTarget);
    motion.lineTrace(ls, ltControl, 15, false);  // 直角までライントレース
    ms.setTargetMileage(ie::OFF_SET + 10);
    motion.goStraight(ms, stControl, 15);        // 直角は直進
    motion.wait(100);
    back = 100;
    ms.setTargetMileage(-back);
    motion.goStraight(ms, stControl, -15);
    motion.stop();


    // 土俵から降りる
    sumo.trainWait(motion, 1);
    ms.setTargetMileage(330 + back);
    motion.goStraight(ms, ltControl, 30);
    motion.raiseArm(15, 15);
    motion.stop();
}
