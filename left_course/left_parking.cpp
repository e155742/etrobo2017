#include "left_parking.hpp"
#include "localization.hpp"

#include "pid_control.hpp"
#include "onoff_control.hpp"

#include "mileage_stopper.hpp"
#include "angle_stopper.hpp"
#include "line_stopper.hpp"
#include "gray_stopper.hpp"

extern ie::Localization* localization;

/**
 * 縦列駐車
 */
void LCourseParking(ie::Motion& motion, float target) {
    ie::PIDControl ltControl(target, 0.15, 0, 0);
    ie::OnOffControl stControl(0, 0.3, 0);

    // 最終ラインを検知するまで前進。右側から侵入
    ie::LineStopper ls(80);
    motion.goStraight(ls, stControl, 30);
    // 車体がラインを跨ぐように少しだけ前進
    ie::MileageStopper ms(ie::OFF_SET + 15);
    motion.goStraight(ms, stControl, 30);

    // ライントレースをするためにラインを検知するまで回転
    ls.setTaigetThreshold(80);
    motion.spin(ls, stControl, 20);
    // 車体とラインをまっすぐにするためにライントレース
    ms.setTargetMileage(300);
    motion.lineTrace(ms, ltControl, 15, false);
    // 灰色までライントレース
    ie::GrayStopper gs(target * 1.5);
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
    motion.stop();

    // 尻尾フリフリ
    motion.wait(700);
    motion.rotateTail(60, 40);
    motion.rotateTail(-120, 40);
    motion.rotateTail(120, 40);
    motion.rotateTail(-120, 40);
    motion.rotateTail(60, 40);
}
