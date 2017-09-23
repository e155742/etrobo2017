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

#include "marker.hpp"

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
    stControl.setCoefficient(0, 0.25, 0);
    // ms.setTargetMileage(2500);
    motion.goPoint(*localization, stControl, 50, localization->getPointX() - 3500, localization->getPointY() - 250, 1000);
    // motion.goStraight(ms, stControl, 100);

    msg_f("RETURN", 10);
    motion.wait(2000);
    ds.setTargetDirection(0.0);
    stControl.setCoefficient(0, 0.3, 0);
    motion.spin(ds, stControl, 15);
    // ds.setTargetDirection(std::atan2(0.0 - localization->getPointX(), 0.0 - localization->getPointY()));
    // motion.spin(ds, stControl, 15);
    // stControl.setCoefficient(0, 0.3, 0);
    // motion.goPoint(*localization, stControl, 60, 0.0, 0.0, 15);
}

int LCourseBlockInit(ie::Decoder& decoder, int greenPosition, std::vector<ie::Marker> markerVector) {
    // マーカーの生成
    markerVector[0].setFields(15, -164.7, 614.7, COLOR_NONE);
    markerVector[1].setFields(10, 614.7, 614.7, COLOR_NONE);
    markerVector[2].setFields(11, 1394.1, 614.7, COLOR_NONE);
    markerVector[3].setFields(15, 2173.5, 614.7, COLOR_NONE);
    markerVector[4].setFields(9, 225, 389.7, COLOR_NONE);
    markerVector[5].setFields(5, 1004.4, 389.7, COLOR_NONE);
    markerVector[6].setFields(6, 1783.8, 389.7, COLOR_NONE);
    markerVector[7].setFields(4, 614.7, 164.7, COLOR_NONE);
    markerVector[8].setFields(6, 1394.1, 164.7, COLOR_NONE);
    markerVector[9].setFields(1, 0.0, 0.0, COLOR_NONE);
    markerVector[10].setFields(15, 2008.8, 0.0, COLOR_NONE);
    markerVector[11].setFields(2, 389.7, -225.0, COLOR_NONE);
    markerVector[12].setFields(3, 839.7, -225.0, COLOR_NONE);
    markerVector[13].setFields(5, 1169.1, -225.0, COLOR_NONE);
    markerVector[14].setFields(8, 1619.1, -225.0, COLOR_NONE);


    // 各色のブロック位置を黒ブロックの番号に合わせる

    int blackPosition = decoder.getBlackPosition();

    int redBlockPosition = decoder.getRedPosition();
    if (redBlockPosition <= 5) {
        redBlockPosition += 1;
    } else if (redBlockPosition <= 10) {
        redBlockPosition += 3;
    } else if (redBlockPosition == 11) {
        redBlockPosition += 4;
    }

    int yellowBlockPosition = decoder.getYellowPosition();
    if (yellowBlockPosition <= 2) {
        yellowBlockPosition += 0;
    } else if (yellowBlockPosition <= 3) {
        yellowBlockPosition += 1;
    } else if (yellowBlockPosition <= 10) {
        yellowBlockPosition += 2;
    } else if (yellowBlockPosition == 11) {
        yellowBlockPosition += 3;
    }

    int bluePosition = decoder.getBluePosition();
    if (bluePosition <= 1) {
        bluePosition += 0;
    } else if (bluePosition <= 2) {
        bluePosition += 1;
    } else if (bluePosition <= 6) {
        bluePosition += 2;
    } else if (bluePosition <= 8) {
        bluePosition += 3;
    } else if (bluePosition <= 11) {
        bluePosition += 4;
    }

    // ブロックを置く
    markerVector[blackPosition      ].setBlock(COLOR_BLACK);
    markerVector[redBlockPosition   ].setBlock(COLOR_RED);
    markerVector[yellowBlockPosition].setBlock(COLOR_YELLOW);
    markerVector[bluePosition       ].setBlock(COLOR_BLUE);
    markerVector[greenPosition      ].setBlock(COLOR_GREEN);

    // 目標を決める
    int targetMarker = 15;
    for (int i = 0; i < 15; i++) {
        if (markerVector[i].haveColorBlock()) {
            if (markerVector[i].getPriorityNum() < targetMarker) {
                targetMarker = markerVector[i].getPriorityNum();
            }
        }
    }

    return targetMarker;
}

void LCourseBlock(ie::Motion& motion, float target, ie::Decoder& decoder, int greenPosition) {
    std::vector<ie::Marker> markerVector(15);
    int targetMarker = LCourseBlockInit(decoder, greenPosition, markerVector);
    msg_clear();
    msg_f(targetMarker, 1);
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
