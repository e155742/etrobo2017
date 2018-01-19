#include "left_trace.hpp"
#include "pid.hpp"

#include "onoff_control.hpp"
#include "pid_control.hpp"

#include "mileage_stopper.hpp"
#include "gray_stopper.hpp"
#include "line_stopper.hpp"
#include "angle_stopper.hpp"

#include "pid_trace.hpp"

#include "util.h"

void pidRun_L(ie::Motion& motion, float target){
    //↓ Lコース

    ie::PidTrace pid(motion, target, false);
    ie::MileageStopper ms;

    int mile = 300;
    ms.setTargetMileage(mile);
    pid.setTarget(target);
    pid.pid(ms, 40, 2, 0.75);
    //sharpCurvePid(motion,ms, target, 40, 0.75); // 強カーブ7割
    soundBeep();


    mile = 2000;
    ms.setTargetMileage(mile);
    pid.pid(ms, 100, 0, 1.0);
    //straightPid(motion,ms, target, 100, 1.0); // 直線
    soundBeep();

    mile = 1300;
    ms.setTargetMileage(mile);
    double targetDev = target - 20;
    pid.setTarget(targetDev);
    pid.pid(ms, 40, 2, 0.75);
    //sharpCurvePid(motion,ms, target, 40, 0.75); // 強カーブ7割

    soundBeep();

    mile = 900;
    ms.setTargetMileage(mile);
    pid.setTarget(target);
    pid.pid(ms, 40, 2, 0.75);
    //sharpCurvePid(motion,ms, target, 40, 0.75); // 強カーブ7割
    soundBeep();

    mile = 2300;
    ms.setTargetMileage(mile);
    pid.setTarget(target);
    pid.pid(ms, 40, 2, 0.75);
    //sharpCurvePid(motion,ms, targetDev, 60, 0.75); // 強カーブ3割
    soundBeep();

    mile = 1800;
    ms.setTargetMileage(mile);
    pid.setTarget(target);
    pid.pid(ms, 40, 2, 0.85);
    sharpCurvePid(motion,ms, target,40, 0.85); // 強カーブ7割
    soundBeep();

    mile = 1240;
    ms.setTargetMileage(mile);
    pid.pid(ms, 100, 0, 1.0);
    //straightPid(motion,ms, target, 100, 1.0); // 直線
    soundBeep();
    //↑ Lコース

	ie::OnOffControl stControl(0, 0.3, 0);
//    ie::MileageStopper ms;
    ie::LineStopper ls(400);

    // 灰色分を直進
    ms.setTargetMileage(330);
    motion.goStraight(ms, stControl, 50);
    motion.stop();

    // 左エッジから右エッジに切り替え
    ie::AngleStopper as(90);
    motion.spin(as, stControl, 10);
    ms.setTargetMileage(20);
    motion.goStraight(ms, stControl, 5);
    motion.spin(ls, stControl, -10);
}
