/**
 * @file right_prize.cpp
 * @brief 検証運び
 *
 * @author Akari Minowa
 */
 #include "right_prize.hpp"
#include <Motor.h>

#include "onoff_control.hpp"
#include "mileage_stopper.hpp"
#include "angle_stopper.hpp"

void RCoursePrize(ie::Motion& motion, ev3api::SonarSensor& sonarSensor) {
    const int distOnPrize = 3;  //懸賞前で止まるための距離
    const int distOnTrain = 35; //電車を避ける距離

    ie::OnOffControl stControl(0, 0.3, 0);

    ie::MileageStopper ms(150);
    motion.goStraight(ms, stControl, 20);

    ie::AngleStopper as(-90);
    motion.spin(as, stControl, -15);
    motion.stop();

    // 懸賞に向かって直進
    while(distOnPrize < sonarSensor.getDistance() || sonarSensor.getDistance() == 0){
        motion.goStraight(stControl, 20);
    }
    motion.stop();

    // 懸賞getし後退.そして回転
    motion.raiseArm(75, 5);  //懸賞を持つ

    // 懸賞を持って後進
    ms.setTargetMileage(-200);
    motion.goStraight(ms, stControl, -20);
    motion.stop();

    // 回転
    as.setAngle(-90);
    motion.spin(as, stControl, -15);
    motion.stop();

    // 電車が通過するのを確認するまで待つ
    while(distOnTrain < sonarSensor.getDistance() || sonarSensor.getDistance() == 0){
        // 待機
    }

    // 回転して交代のままレールの上を通過．懸賞置き場へ置く
    as.setAngle(-180);
    motion.spin(as, stControl, -15);

    ms.setTargetMileage(-500);
    motion.goStraight(ms, stControl, -20);

    as.setAngle(-90);
    motion.spin(as, stControl, -15);

    ms.setTargetMileage(100);
    motion.goStraight(ms, stControl, 10);

    motion.raiseArm(7, 10);

    ms.setTargetMileage(-150);
    motion.goStraight(ms, stControl, -10);

    // バックで駐車
    as.setAngle(90);
    motion.spin(as, stControl, 10);

    ms.setTargetMileage(-350);
    motion.goStraight(ms, stControl, -10);

    as.setAngle(90);
    motion.spin(as, stControl, 10);

    ms.setTargetMileage(-400);
    motion.goStraight(ms, stControl, -10);
    motion.stop();
}
