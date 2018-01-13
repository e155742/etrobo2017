#include "pid.hpp"
#include "right_trace.hpp"
#include "util.h"

#include "onoff_control.hpp"

#include "mileage_stopper.hpp"
#include "gray_stopper.hpp"
#include "line_stopper.hpp"
#include "angle_stopper.hpp"

void pidRun_R(ie::Motion& motion, float target){
    //↓ Rコース

    ie::MileageStopper ms;
    ie::GrayStopper gs(target*1.5);
    ie::AngleStopper as(90);
    ie::OnOffControl stControl(0, 0.3, 0);

    int mile = 300;
    ms.setTargetMileage(mile);
    sharpCurvePid(motion,ms, target, 40, 0.75); // 強カーブ7割
    soundBeep();

    mile = 1950;
    ms.setTargetMileage(mile);
    straightPid(motion,ms, target, 100, 1.0); // 直線
    soundBeep();

//    double targetDev = target - 50.0;
	double targetDev = target - 10.0;
    mile = 2750;
    ms.setTargetMileage(mile);
    sharpCurvePid(motion,ms, targetDev, 80, 0.70); // 強カーブ7割
    soundBeep();

    mile = 2300;
    ms.setTargetMileage(mile);
    sharpCurvePid(motion,ms, target, 40, 0.75); // 強カーブ7割
    soundBeep();

    mile = 1280;
    ms.setTargetMileage(mile);
    targetDev = target - 10;
    sharpCurvePid(motion,ms, targetDev, 50, 0.30); // 強カーブ4割
    soundBeep();

    mile = 1700;
    ms.setTargetMileage(mile);
    straightPid(motion,ms, target, 100, 1.0); // 直線
    soundBeep();
    //↑ Rコース

    //↓ 踏切おろし
    //Basic後灰色一旦通り過ぎる
    targetDev = target + 40;
    mile = 800;
    ms.setTargetMileage(mile);
    sharpCurvePid(motion,ms, targetDev, 20, 10.0); // 強カーブ15割
    soundBeep();

    //180度右へ
    motion.spin(as, stControl, 10);
    ie::LineStopper ls(target*1.5);
    motion.spin(ls, stControl, 10);

    //反転後灰色直前まで進む
    targetDev = target + 40;
    mile = 600;
    ms.setTargetMileage(mile);
    sharpCurvePid(motion,ms, targetDev, 20, 10.0); // 強カーブ15割
    soundBeep();

    //灰色検知
    sharpCurvePid(motion,gs, target, 20, 10.0); // 強カーブ15割
    soundBeep();

    mile = 800;
    ms.setTargetMileage(mile);
    sharpCurvePid(motion,ms, target, 20, 10.0); // 強カーブ15割
    soundBeep();


    //90度右へ
    motion.spin(as, stControl, 10);
    soundBeep();

    motion.stop();



    //↑ 踏切おろし

    //↓ Besic後
    targetDev = target + 40;
    mile = 890;
    ms.setTargetMileage(mile);
    sharpCurvePid(motion,ms, targetDev, 20, 10.0); // 強カーブ15割
    soundBeep();

    // mile = 720;
    targetDev = target - 20;
    mile = 300 + 150;
    ms.setTargetMileage(mile);
    sharpCurvePid(motion,ms, targetDev, 20, 4.0); // 強カーブ15割
    soundBeep();
}
