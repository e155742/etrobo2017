#include "left_trace.hpp"
#include "pid.hpp"

#include "onoff_control.hpp"
#include "pid_control.hpp"

#include "mileage_stopper.hpp"
#include "gray_stopper.hpp"
#include "line_stopper.hpp"
#include "angle_stopper.hpp"

#include "util.h"

void pidRun_L(ie::Motion& motion, float target){
    //↓ Lコース

    int mile = 300;
    sharpCurvePid(motion, target, mile, 40, 0.75); // 強カーブ7割
    soundBeep();


    mile = 2000;
    straightPid(motion, target, mile, 100); // 直線
    soundBeep();

    mile = 1300;
    double targetDev = target - 20;
    sharpCurvePid(motion, target, mile, 40, 0.75); // 強カーブ7割

    soundBeep();

    mile = 900;
    sharpCurvePid(motion, target, mile, 40, 0.75); // 強カーブ7割
    soundBeep();

    mile = 2300;
    sharpCurvePid(motion, targetDev, mile, 60, 0.75); // 強カーブ3割
    soundBeep();

    mile = 1800;
    sharpCurvePid(motion, target, mile, 40, 0.75); // 強カーブ7割
    soundBeep();

    mile = 1340;
    straightPid(motion, target, mile, 100); // 直線
    soundBeep();
    //↑ Lコース

	ie::OnOffControl stControl(0, 0.3, 0);
    ie::PIDControl ltControl(target, 0.08, 0, 0.0001);
    ie::MileageStopper ms;
    ie::GrayStopper gs(550);
    ie::LineStopper ls(400);
    // ゴール後の灰色まで直進
    // motion.lineTrace(gs, ltControl, 80, false);

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

