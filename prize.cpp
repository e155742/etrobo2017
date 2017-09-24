/**
 * @file prize.cpp
 * @brief 検証運び
 *
 * @author Akari Minowa
 */
#include "prize.hpp"
#include <Motor.h>

#include "onoff_control.hpp"
#include "mileage_stopper.hpp"
#include "angle_stopper.hpp"

namespace ie {

Prize::Prize(Motion& motion):
leftWheel_(LEFT_WHEEL_PORT), rightWheel_(RIGHT_WHEEL_PORT),
sonarSensor_(SONAR_SENSOR_PORT), motion_(motion) {}

void Prize::prizeCourse() {
    // int16_t distance = 0; //距離格納用変数
    ie::OnOffControl stControl(0, 0.3, 0);

    MileageStopper ms(150);
    motion_.goStraight(ms, stControl, 20);

    AngleStopper as(-90);
    motion_.spin(as, stControl, -15);
    motion_.stop();

    // 懸賞に向かって直進
    while(distOnPrize_ < sonarSensor_.getDistance() || sonarSensor_.getDistance() == 0){
        motion_.goStraight(stControl, 20);
    }
    motion_.stop();

    // 懸賞getし後退.そして回転
    motion_.raiseArm(75, 5);  //懸賞を持つ

    // 懸賞を持って後進
    ms.setTargetMileage(-200);
    motion_.goStraight(ms, stControl, -20);
    motion_.stop();

    // 回転
    as.setAngle(-90);
    motion_.spin(as, stControl, -15);
    motion_.stop();

    // 電車が通過するのを確認するまで待つ
    while(distOnTrain_ < sonarSensor_.getDistance() || sonarSensor_.getDistance() == 0){
        // 待機
    }

    // 回転して交代のままレールの上を通過．懸賞置き場へ置く
    as.setAngle(-180);
    motion_.spin(as, stControl, -15);

    ms.setTargetMileage(-500);
    motion_.goStraight(ms, stControl, -20);

    as.setAngle(-90);
    motion_.spin(as, stControl, -15);

    ms.setTargetMileage(100);
    motion_.goStraight(ms, stControl, 10);

    motion_.raiseArm(7, 10);

    ms.setTargetMileage(-150);
    motion_.goStraight(ms, stControl, -10);


    // バックで駐車
    as.setAngle(90);
    motion_.spin(as, stControl, 10);

    ms.setTargetMileage(-350);
    motion_.goStraight(ms, stControl, -10);

    as.setAngle(90);
    motion_.spin(as, stControl, 10);

    ms.setTargetMileage(-400);
    motion_.goStraight(ms, stControl, -10);
    motion_.stop();
}

}
