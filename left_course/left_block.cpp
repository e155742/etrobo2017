#include "left_block.hpp"
#include "localization.hpp"

#include "block_field.hpp"

#include "pid_control.hpp"
#include "onoff_control.hpp"

#include "mileage_stopper.hpp"
#include "direction_stopper.hpp"
#include "line_stopper.hpp"
#include "color_stopper.hpp"

#include "gray_stopper.hpp"
#include "angle_stopper.hpp"

#include "util.h"

extern ie::Localization* localization;

/**
 * ブロック並べ
 * 無視して駐車場まで移動する
 */
void LCourseBlock(ie::Motion& motion, float target, ie::Decoder& decoder, int greenPosition) {
    dly_tsk(1); // 念のため
    ie::BlockField bf(*localization, decoder, greenPosition);
    // 以下の変数はブロック並べを行わないため使用しない。
    // int targetMarker = bf.getTargetMarker();

    // // ライントレースに移るために旋回
    ie::OnOffControl stControl(0, 0.3, 0);
    ie::PIDControl ltControl(target, 0.08, 0, 0.0001);
    ie::GrayStopper gs(550);
    ie::LineStopper ls(400);
    ie::MileageStopper ms;

    // バックストレートで自機方位修正
    // ブロック置き場での修正は誤差が出る可能性が高い
    ms.setTargetMileage(200);
    motion.lineTrace(ms, ltControl, 15, true);
    localization->setDirection(degToRad(-90));

    // ブロック置き場までライントレース
    ltControl.setKonstant(0.5, 0, 0.015);
    ms.setTargetMileage(700);
    motion.lineTrace(ms, ltControl, 20, true);
    ie::ColorStopper cs;
    motion.lineTrace(cs, ltControl, 20, true);
    motion.wait(100);

    // バックして白色地帯に乗せる
    ms.setTargetMileage(-350); // 320
    motion.goStraight(ms, stControl, -15);
    motion.wait(100);

    // ブロック並べフィールドの右端を向く
    ie::DirectionStopper ds(*localization, degToRad(87)); // 90
    motion.spin(ds, stControl, 15);
    motion.wait(100);

    // 裏ストレートを直進
    ms.setTargetMileage(1850);
    motion.goStraight(ms, stControl, 30);
    motion.stop();

    // 島を避ける
    ds.setTargetDirection(degToRad(60));
    motion.setRightPwm(ds, 30);
    ms.setTargetMileage(380); // 470
    motion.goStraight(ms, stControl, 30);
    ds.setTargetDirection(degToRad(87));
    motion.setLeftPwm(ds, 30);

    // 駐車場ラインに乗せる
    ms.setTargetMileage(150);
    motion.goStraight(ms, stControl, 30);
    ds.setTargetDirection(degToRad(0));
    motion.spin(ds, stControl, 20);
    motion.stop();
}
