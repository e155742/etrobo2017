#include <vector>
#include <cmath>
#include "app.h"
#include "util.h"

#include <ColorSensor.h>
#include "robo_meta_datas.hpp"

#include "decoder.hpp"
#include "hsv_converter.hpp"
#include "file_output.hpp"
#include "calibration.hpp"
#include "starter.hpp"

#include "control.hpp"
#include "pid_control.hpp"
#include "onoff_control.hpp"

#include "localization.hpp"
#include "move.hpp"
#include "motion.hpp"

#include "mileage_stopper.hpp"
#include "angle_stopper.hpp"
#include "direction_stopper.hpp"
#include "color_stopper.hpp"
#include "line_stopper.hpp"
#include "gray_stopper.hpp"

#include "left_course.cpp"

ie::Localization* localization;
ev3api::Motor left(ie::LEFT_WHEEL_PORT);
ev3api::Motor right(ie::RIGHT_WHEEL_PORT);

/**
 * 20ms間隔で呼び出される周期ハンドラ<br>
 * 自己位置推定を行っている
 */
void sub_cyc(intptr_t exinf) {
    static bool f = true;
    if (f) {
        // global領域で初期化するとなぜか動かない
        localization = new ie::Localization();
        f = false;
    }

    localization->updatePoint();
}

/**
 * スタート前に呼び出す関数。
 * アームの初期化とキャリブレーションと自己位置推定開始
 */
void init(ie::Motion& motion, float& threshold) {
    motion.raiseArm(15, 5);

    // キャリブレーション
    msg_f("Please waite...", 1);
    ie::Calibration* calibration = new ie::Calibration();
    threshold = calibration->calibrate() * 0.40; // 普通のライントレース
    delete calibration;
    msg_f("Threshold", 7);
    msg_f(threshold, 8);

    // タッチセンサーを押すとスタート
    ie::Starter* starter = new ie::Starter();
    msg_f("PUSH TOUCH SENSOR!", 10);
    starter->startWait();
    msg_f("START!", 10);
    delete starter;
    ev3_sta_cyc(SUB_CYC);
}

/**
 * 終了時に呼び出す関数。
 */
void del(ie::Motion& motion) {
    motion.stop(); // 念のため
    ev3_stp_cyc(SUB_CYC);
    delete localization;
    #ifndef PRINT_LOCALIZATION
    msg_f("END...", 10);
    #endif
}

void pidCurve(ie::Motion& motion, float target, int mile) {
    // PIDの各種定数
	
	/*
	// 直線用
    const float kp = 0.003; // 比例定数
    const float ki = 0.00; // 積分定数
    const float kd = 0.001; // 微分定数
	*/

	// ゆるい曲線用
	
    const float kp = 0.040; // 比例定数
    const float ki = 0.00; // 積分定数
    const float kd = 0.004; // 微分定数
	

	// きつい曲線用
	/*
    const float kp = 0.003; // 比例定数
    const float ki = 0.00; // 積分定数
    const float kd = 0.001; // 微分定数
	*/



    // ボタン入力
    // inputFloat(kp, 10, "kp X.*");
    // inputFloat(kp, "kp X.X*");
    // inputFloat(kp, 1000, "kp X.XX*");

    const int pwm = 70;

    ie::PIDControl ltControl(target, kp, ki, kd);
    ie::MileageStopper stopper(mile);
    motion.lineTrace(stopper, ltControl, pwm, false);
}

void pidStraight(ie::Motion& motion, float target, int mile) {
    // PIDの各種定数
	
	// 直線用ベストパラメータ
	/*
    const float kp = 0.003; // 比例定数
    const float ki = 0.00; // 積分定数
    const float kd = 0.001; // 微分定数
	*/

	// 直線用
    float kp = 0.027; // 比例定数
    const float ki = 0.00; // 積分定数
    const float kd = 0.003; // 微分定数

    // ボタン入力
    // inputFloat(kp, 100, "kp X.*");
    // inputFloat(kp, "kp X.X*");
    // inputFloat(kp, 1000, "kp X.XX*");

    const int pwm = 100;

    ie::PIDControl ltControl(target, kp, ki, kd);
    ie::MileageStopper stopper(mile);
    motion.lineTrace(stopper, ltControl, pwm, false);
}

void main_task(intptr_t unused) {
    ie::Motion motion;
    float target;
    init(motion, target);
	int mile = 0;

	mile = 2200;
    pidStraight(motion, target, mile);
	mile = 4000;
    pidCurve(motion, target, mile);

    del(motion);
}
