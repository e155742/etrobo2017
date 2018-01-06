#include "pid.hpp"
#include "right_trace.hpp"
#include "util.h"

void pidRun_R(ie::Motion& motion, float target){
    //↓ Rコース

    float gain = 0.5;
    int mile = 300;
    sharpCurvePid(motion, target, mile, 40, 0.75); // 強カーブ7割
    soundBeep();

    mile = 1950;
    straightPid(motion, target, mile, 100, 1.0); // 直線
    soundBeep();

//    double targetDev = target - 50.0;
	double targetDev = target - 10.0;
    mile = 2750;
    sharpCurvePid(motion, targetDev, mile, 80, 0.70); // 強カーブ7割
    soundBeep();

    mile = 2300;
    sharpCurvePid(motion, target, mile, 40, 0.75); // 強カーブ7割
    soundBeep();

    mile = 1280;
    targetDev = target - 10;
    sharpCurvePid(motion, targetDev, mile, 50, 0.30); // 強カーブ4割
    soundBeep();

    mile = 1700;
    straightPid(motion, target, mile, 100, 1.0); // 直線
    soundBeep();
    //↑ Rコース

    //↓ Besic後
    mile = 890;
    targetDev = target + 40;
    sharpCurvePid(motion, targetDev, mile, 20, 10.0); // 強カーブ15割
    soundBeep();

    // mile = 720;
    mile = 300 + 150;
    targetDev = target - 20;
    sharpCurvePid(motion, targetDev, mile, 20, 4.0); // 強カーブ15割
    soundBeep();
}
