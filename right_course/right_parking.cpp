#include "right_parking.hpp"

#include "pid_control.hpp"
#include "onoff_control.hpp"

#include "angle_stopper.hpp"
#include "mileage_stopper.hpp"
#include "line_stopper.hpp"
#include "gray_stopper.hpp"
#include "direction_stopper.hpp"

#include "util.h"

extern ie::Localization* localization;

void RCourseParking(ie::Motion& motion, float target) {
    ie::PIDControl ltControl(target, 0.1, 0, 0.001);
    ie::OnOffControl stControl(0, 0.3, 0);
    ie::AngleStopper as;
    ie::MileageStopper ms;
    ie::LineStopper ls(target);
    ie::GrayStopper gs(500); // 550
    ie::DirectionStopper ds(*localization, degToRad(90));

    // ライントレース準備
    motion.raiseArm(5,5);
    as.setAngle(-30);
    motion.spin(as, stControl, -30);
    motion.spin(ls, stControl, 10);

    // 直角までライントレース
    ms.setTargetMileage(180);
    motion.lineTrace(ms, ltControl, 20, false);
    motion.lineTrace(gs, ltControl, 30, false); // 20
    ms.setTargetMileage(ie::OFF_SET); // +10はいらない
    motion.goStraight(ms, ltControl, 30); // 20
    localization->setDirection(0.0);
    motion.stop();

    // 駐車場を向く
    // motion.spin(ls, stControl, 10);
    motion.spin(ds, stControl, 15); // 10
    motion.stop();

    // 駐車
    ms.setTargetMileage(420); // 450
    motion.goStraight(ms, ltControl, 30); // 20 ライントレースだと暴走するから普通の直進
    motion.stop();

}
