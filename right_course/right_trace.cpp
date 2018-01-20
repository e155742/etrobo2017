#include "pid.hpp"
#include "right_trace.hpp"
#include "util.h"

#include "localization.hpp"

#include "pid_control.hpp"
#include "onoff_control.hpp"

#include "mileage_stopper.hpp"
#include "gray_stopper.hpp"
#include "line_stopper.hpp"
#include "angle_stopper.hpp"

#include "pid_trace.hpp"

extern ie::Localization* localization;

void pidRun_R(ie::Motion& motion, float target){
    //↓ Rコース

    ie::MileageStopper ms;
    ie::GrayStopper gs(target*1.5);
    ie::AngleStopper as(90);
    ie::OnOffControl stControl(0, 0.3, 0);
    ie::PidTrace pid(motion, target, false);

    int mile = 300;
    pid.setTarget(target);
    ms.setTargetMileage(mile);
    pid.pid(ms, 40, 2, 0.75);
    //sharpCurvePid(motion,ms, target, 40, 0.75); // 強カーブ7割
    soundBeep();

    mile = 1950;
	  double targetDev = target - 10.0;
    pid.setTarget(targetDev);
    ms.setTargetMileage(mile);
    pid.pid(ms, 100, 2, 1.0);//curve0で動かないから1にした。
    //Pid(motion,ms, target, 100, 1.0); // 直線
    soundBeep();

//    double targetDev = target - 50.0;
	  targetDev = target - 10.0;
    mile = 2750;
    pid.setTarget(targetDev);
    ms.setTargetMileage(mile);
    pid.pid(ms, 70, 2, 0.7);//80だと曲がりきれない可能性70へ変更
    //sharpCurvePid(motion,ms, targetDev, 80, 0.70); // 強カーブ7割
    soundBeep();

    mile = 2300;
    pid.setTarget(target);
    ms.setTargetMileage(mile);
    pid.pid(ms, 40, 2, 0.75);
    //sharpCurvePid(motion,ms, target, 40, 0.75); // 強カーブ7割
    soundBeep();

    mile = 1280;
    ms.setTargetMileage(mile);
    targetDev = target - 10;
    pid.setTarget(targetDev);
    pid.pid(ms, 50, 2, 0.30);
    //sharpCurvePid(motion,ms, targetDev, 50, 0.30); // 強カーブ4割
    soundBeep();

    mile = 1700;
    ms.setTargetMileage(mile);
    targetDev = target - 20;
    pid.setTarget(targetDev);

    pid.pid(ms, 100, 2, 1.0);//curve0で動かないから1にした。
    // ie::PIDControl pidControl(target, 0.065, 0.0007, 0.030);
    // motion.lineTraceK(pidControl, 100, false);
    // while (localization->getPointX() > -3780.0) {
    //     // loop
    // }
    // ev3_stp_cyc(LINE_TRACE_CYC);

    //pid.pid(ms, 100, 0, 1.0);
    //0Pid(motion,ms, target, 100, 1.0); // 直線
    soundBeep();


    //↑ Rコース

    /*
    //↓ 踏切おろし
    motion.wait(2000);
    mile = 80;
    ms.setTargetMileage(mile);
    pid.setTarget(target);
    pid.pid(ms, 100, 0, 1.0);
    //0Pid(motion,ms, target, 100, 1.0); // 直線
    soundBeep();
    //灰色を過ぎたところからスタート
    //180度右へ
    as.setAngle(90);
    motion.spin(as, stControl, 10);
    soundBeep();

    as.setAngle(90);
    motion.spin(as, stControl, 10);
    soundBeep();
    //ie::LineStopper ls(80);
    //motion.spin(ls, stControl, 10);

    // 距離少しpid。灰色検知安定のため
    mile = 50;
    ms.setTargetMileage(mile);
    targetDev=target+30;
    pid.setTarget(targetDev);
    pid.setEdge(true);
    pid.pid(ms, 20, 2, 10.0);

    // 回転後、灰色検知走行
    pid.setTarget(target);
    pid.setEdge(true);
    pid.pid(gs, 20, 2, 10.0);
    soundBeep();
    motion.wait(1000);

    // 灰色検知後、黒検知
    targetDev = target + 110;
    pid.setTarget(targetDev);
    ie::LineStopper ls(target*0.45);//testの値
    pid.pid(ls, 20, 2, 10.0);
    soundBeep();

    //灰色から駅まで
    mile = 450;
    ms.setTargetMileage(mile);
    pid.setTarget(target);
    pid.setEdge(true);
    pid.pid(ms, 20, 2, 10.0);
    //sharpCurvePid(motion,ms, target, 20, 10.0); // 強カーブ15割
    soundBeep();


    //90度右へ
    as.setAngle(90);
    motion.spin(as, stControl, 10);
    soundBeep();

    while(1){
    motion.stop();
    }



    //↑ 踏切おろし
    */

    //↓ Besic後
    targetDev = target + 40;
    mile = 890;
    pid.setTarget(targetDev);
    ms.setTargetMileage(mile);
    pid.pid(ms, 20, 2, 10.0+10.0);//10.0 から20.0に引き上げ
    //sharpCurvePid(motion,ms, targetDev, 20, 10.0); // 強カーブ15割
    soundBeep();

    // mile = 720;
    targetDev = target - 20;
    mile = 300;
    pid.setTarget(targetDev);
    ms.setTargetMileage(mile);
    pid.pid(ms, 20, 2, 10.0);
    //sharpCurvePid(motion,ms, targetDev, 20, 4.0); // 強カーブ15割
    soundBeep();
}
