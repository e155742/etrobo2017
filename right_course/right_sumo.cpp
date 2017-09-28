#include "right_sumo.hpp"

#include "onoff_control.hpp"
#include "pid_control.hpp"

#include "mileage_stopper.hpp"
#include "angle_stopper.hpp"
#include "line_stopper.hpp"
#include "direction_stopper.hpp"

#include "robo_meta_datas.hpp"
#include "util.h"

extern ie::Localization* localization;

void RCourseSumo(ie::Motion& motion, float target, ev3api::SonarSensor& sonarSensor) {
    const uint32_t waitTIme = 1000; // 電車が目の前を通ったとこの待機時間
    int16_t distance = 0;
    ie::OnOffControl stControl(0, 0.3, 0);
    ie::PIDControl ltControl(target, 0.15, 0, 0);
    ie::MileageStopper ms;
    ie::AngleStopper as;
    ie::LineStopper ls(110);
    ie::DirectionStopper ds(*localization);

    // motion.raiseArm(15, 5);
    motion.raiseArm(45, 50); // 線路にアームが引っかからないように

    // 電車が通過するまでで待機
    motion.stop();
    while (true) {
        int16_t distance = sonarSensor.getDistance(); // 距離を測る
        if (0 < distance && distance < 60) {
            // 目の前を通っているので通り過ぎるまで待機
            while (sonarSensor.getDistance() < 60) {}
            motion.wait(waitTIme);
            break;
        }
        if (distance < 180) {
            break;
        }
    }

    // 土俵の上に移動
    ms.setTargetMileage(400);
    motion.goStraight(ms, stControl, 50);
    msg_f(distance, 11);
    motion.stop();
    motion.raiseArm(15, 15);

    // ライントレース準備
    as.setAngle(-45);
    motion.spin(as, stControl, -30);
    motion.spin(ls, stControl, 15);

    // 2番目の直角までライントレース
    ms.setTargetMileage(50);
    motion.lineTrace(ms, ltControl, 15, false);  // 車体をまっすぐにする
    // localization->setDirection(0);   // 方位を0に
    motion.lineTrace(ls, ltControl, 20, false); // 直角までライントレース 少し早いくらいが車体がブレない
    // ds.setTargetDirection(0.0);
    // motion.spin(ds, stControl, 5);
    ms.setTargetMileage(ie::OFF_SET + 10);
    motion.goStraight(ms, ltControl, 10); // トレッド軸にラインが来るように移動
    motion.wait(200); // 念のため停止

}
