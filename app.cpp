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
void init(ie::Motion& motion, float& threshold, float targetGain) {
    motion.raiseArm(15, 5);

    // キャリブレーション
    msg_f("Please waite...", 1);
    ie::Calibration* calibration = new ie::Calibration();
    threshold = calibration->calibrate() * targetGain; // 普通のライントレースBest0.43
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

void pid(ie::Motion& motion, float target, int mile, int pwm, float kp, float ki, float kd, bool isRight) {
    // PIDの各種定数

	/*
	// 直線用
    const float kp = 0.003; // 比例定数
    const float ki = 0.00; // 積分定数
    const float kd = 0.001; // 微分定数
	*/

	// ゆるい曲線用

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


    ie::PIDControl ltControl(target, kp, ki, kd);
    ie::MileageStopper stopper(mile);
    motion.lineTrace(stopper, ltControl, pwm, false);
}

void beep(){
  ev3_speaker_set_volume(100);
  ev3_speaker_play_tone(NOTE_B5, 40);
}

void main_task(intptr_t unused) {
    ie::Motion motion;
    float target;
    float targetGain = 0.43;//暫定Best 0.43
    init(motion, target, targetGain);
	int mile = 0;

	//↓ Rコース
	mile = 2200;
	//pid(motion, 閾値, 距離, 速度,  kp, ki, kd,   isRight);
    pid(motion, target, mile, 100, 0.02, 0.0, 0.02, false);//直線
	beep();

	mile = 3200;
    pid(motion, target, mile, 80, 0.040, 0.0001, 0.025, false);//弱カーブ
	beep();

	mile = 3400;
    pid(motion, target, mile, 70, 0.06, 0.001, 0.05, false);//強カーブ
	beep();

	mile = 1600;
    pid(motion, target, mile, 100, 0.02, 0.000, 0.02, false);//直線
	beep();
	//↑ Rコース

	/*
	//↓ Lコース
	mile = 2200;
    pid(motion, target, mile, 100, 0.02, 0.0, 0.02, false);//直線
	beep();

	mile = 2100;
    pid(motion, target, mile, 70, 0.06, 0.0001, 0.05, false);//強カーブ
	beep();


	mile = 2200;
    pid(motion, target, mile, 80, 0.040, 0.0001, 0.04, false);//弱カーブ
	beep();

	mile = 2000;
    pid(motion, target, mile, 70, 0.06, 0.001, 0.05, false);//強カーブ
	beep();

	mile = 1600;
    pid(motion, target, mile, 100, 0.02, 0.000, 0.02, false);//直線;
	beep();
	//↑ Lコース
	*/

    del(motion);
}
