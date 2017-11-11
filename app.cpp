#include <vector>
#include <Motor.h>
#include <SonarSensor.h>
#include <ColorSensor.h>
#include <Clock.h>
#include "app.h"

#include "motion.hpp"
#include "localization.hpp"

#include "pid_control.hpp"
#include "onoff_control.hpp"

#include "mileage_stopper.hpp"
#include "direction_stopper.hpp"
#include "angle_stopper.hpp"
#include "line_stopper.hpp"

#include "file_output.hpp"
#include "calibration.hpp"
#include "starter.hpp"

#include "left_idaten.hpp"
#include "left_block.hpp"
#include "left_parking.hpp"

#include "right_idaten.hpp"
#include "right_sumo.hpp"
#include "right_parking.hpp"
#include "right_prize.hpp"

#include "robo_meta_datas.hpp"
#include "util.h"

// #define TEST_MODE
// #define LEFT_COURSE // ブロック並べの方
// #define IDATEN // 韋駄天

ie::Localization* localization;

#ifdef TEST_MODE
ev3api::Motor left(ie::LEFT_WHEEL_PORT);
ev3api::Motor right(ie::RIGHT_WHEEL_PORT);
ev3api::Motor tail(ie::TAIL_MOTOR_PORT);
#endif

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
void init(ie::Motion& motion, float& threshold, float& threshold2) {
    motion.raiseArm(60, 15); // 動いていることがわかりやすいように
    #ifdef IDATEN
    motion.raiseArm(15, 5);
    # else
    motion.raiseArm(5, 5);
    # endif

    // キャリブレーション
    ie::Calibration* calibration = new ie::Calibration();
    threshold = calibration->calibrate() * 0.47; // 普通のライントレースBest0.43
    #ifndef LEFT_COURSE
    threshold2 = calibration->calibrate() * 0.47; // 普通のライントレースBest0.43
    #endif
    delete calibration;
    msg_f("Threshold", 7);
    msg_f(threshold, 8);

    motion.wait(1000);
    #ifdef IDATEN
    motion.raiseArm(0, 15); // フロントサスペンション
    #endif

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
    char str[64];
    sprintf(str, "%d mV", ev3_battery_voltage_mV());
    msg_f(str, 11);
    #endif
}

/**
 * ブロック並べの初期位置コードを解読
 */
void initCodeDecode(ie::Decoder d) {
    msg_clear();
    std::vector<int> initCodeVector = {0, 0, 0, 0, 0};
    inputInt(initCodeVector, "INPUT INIT CODE");
    int initCode = initCodeVector[0] * 10000;
    initCode += initCodeVector[1] * 1000;
    initCode += initCodeVector[2] * 100;
    initCode += initCodeVector[3] * 10;
    initCode += initCodeVector[4];
    msg_clear();
    msg_f(initCode ,2);
    d.setPositionFromCode(initCode);
    msg_f("Black, Red, Yellow, Blue", 4);
    char str[64];
    sprintf(str, "%d,    %d,   %d,      %d", d.getBlackPosition(), d.getRedPosition(), d.getYellowPosition(), d.getBluePosition());
    msg_f(str, 5);
}

void motionTest(ie::Motion& motion, float target) {
    ie::LineStopper ls(80);
    ie::PIDControl ltControl(target, 0.15, 0, 0);
    motion.lineTrace(ls, ltControl, 30, false);
}

/**************************************************
 * PIDトレース                                    *
 * ファイル分割するとうまく動かないのでこのままで *
 **************************************************/

void pid(ie::Motion& motion, float target, int mile, int pwm, float kp, float ki, float kd, bool isRight) {
    ie::PIDControl ltControl(target, kp, ki, kd);
    ie::MileageStopper stopper(mile);
    motion.lineTraceK(stopper, ltControl, pwm, false);
}

void beep(){
    ev3_speaker_set_volume(100);
    ev3_speaker_play_tone(NOTE_B5, 40);
}

void straightPid(ie::Motion& motion, float target, int mile, int speed){
    // pid(motion, 閾値, 距離, 速度,  kp, ki, kd,   isRight);
    // pid(motion, target, mile, speed, 0.02, 0.0, 0.014, false);//直線
    pid(motion, target, mile, speed, 0.038, 0.0001, 0.017, false);//弱カーブP弱
}

void gentleCurvePid(ie::Motion& motion, float target, int mile, int speed){
    // pid(motion, 閾値, 距離, 速度,  kp, ki, kd,   isRight);
    // pid(motion, target, mile, speed, 0.038, 0.0001, 0.019, false);//弱カーブP弱
    pid(motion, target, mile, speed, 0.038, 0.0001, 0.017, false);//弱カーブP弱
}

void sharpCurvePid(ie::Motion& motion, float target, int mile, int speed, float gain){
    // pid(motion, 閾値, 距離, 速度,  kp, ki, kd,   isRight);
    // pid(motion, target, mile, speed, 0.065*gain, 0.001*gain, 0.02*gain, false);//(8.7V Best)
    // pid(motion, target, mile, speed, 0.065*gain, 0.0007*gain, 0.02*gain, false);//I弱
    pid(motion, target, mile, speed, 0.065*gain, 0.0007*gain, 0.025*gain, false);//I弱D強
}

void pidRun_R(ie::Motion& motion, float target){
    //↓ Rコース
    int mile = 40;
    straightPid(motion, target, mile, 20);//直線
    beep();

    mile = 40;
    straightPid(motion, target, mile, 40);//直線
    beep();

    mile = 40;
    straightPid(motion, target, mile, 60);//直線
    beep();

    mile = 40;
    straightPid(motion, target, mile, 80);//直線
    beep();


    mile = 2150;
    straightPid(motion, target, mile, 100);//直線
    beep();

    double targetDev = target - 50.0;
    mile = 2800;
    gentleCurvePid(motion, targetDev, mile, 83);// 弱カーブ
    beep();

    mile = 2100;
    sharpCurvePid(motion, target, mile, 40, 0.75);//強カーブ7割
    beep();

    mile = 1480;
    targetDev = target - 10;
    sharpCurvePid(motion, targetDev, mile, 50, 0.33);//強カーブ4割
    beep();

    mile = 40;
    straightPid(motion, target, mile, 60);//直線
    beep();

    mile = 40;
    straightPid(motion, target, mile, 80);//直線
    beep();


    mile = 1540;
    straightPid(motion, target, mile, 100);//直線
    beep();
    //↑ Rコース

    //↓ Besic後
    mile = 900;
    targetDev = target + 20;
    // sharpCurvePid(motion, targetDev, mile, 20, 1.5);//強カーブ15割
    sharpCurvePid(motion, targetDev, mile, 20, 2.0);//強カーブ15割
    beep();

    // mile = 720;
    mile = 300;
    targetDev = target - 20;
    // sharpCurvePid(motion, targetDev, mile, 20, 1.5);//強カーブ15割
    sharpCurvePid(motion, targetDev, mile, 20, 2.0);//強カーブ15割
    beep();
}


void pidRun_L(ie::Motion& motion, float target){
    //↓ Lコース
    int mile = 40;
    straightPid(motion, target, mile, 20);//直線
    beep();

    mile = 40;
    straightPid(motion, target, mile, 40);//直線
    beep();

    mile = 40;
    straightPid(motion, target, mile, 60);//直線
    beep();

    mile = 40;
    straightPid(motion, target, mile, 80);//直線
    beep();

    mile = 2050;
    straightPid(motion, target, mile, 100);//直線
    beep();

    mile = 1390;
    double targetDev = target - 40;
    sharpCurvePid(motion, targetDev, mile, 70, 0.75);//強カーブ7割
    beep();

    mile = 900;
    sharpCurvePid(motion, target, mile, 40, 0.75);//強カーブ7割
    beep();

    mile = 2300;
    sharpCurvePid(motion, target, mile, 65, 0.60);//強カーブ7割
    beep();

    mile = 1800;
    sharpCurvePid(motion, target, mile, 40, 0.75);//強カーブ7割
    beep();


    mile = 40;
    straightPid(motion, target, mile, 40);//直線
    beep();

    mile = 40;
    straightPid(motion, target, mile, 60);//直線
    beep();

    mile = 40;
    straightPid(motion, target, mile, 80);//直線
    beep();

    mile = 1180;
    straightPid(motion, target, mile, 100);//直線
    beep();
    //↑ Lコース
}

/**************************************************
 * PIDトレースここまで                            *
 **************************************************/

/**
 * Lコース
 * ブロック並べのほう
 */
void leftCourse(ie::Motion& motion, float target) {
    ie::Decoder decoder;
    int greenPosition = 14; // LCourseBlock()のためのダミー ブロック並べはやらないから使わない

    #ifdef IDATEN
    LCourseIdaten(motion);
    #else
    pidRun_L(motion, target);
    #endif
    LCourseBlock(motion, target, decoder, greenPosition);
    LCourseParking(motion, target);
}

/**
 * Rコース
 * ET相撲のほう
 */
void rightCourse(ie::Motion& motion, float target, float target2, ev3api::SonarSensor& sonarSensor, ev3api::ColorSensor& colorSensor) {
    #ifdef IDATEN
    RCourseIdaten(motion, target);
    #else
    pidRun_R(motion, target);
    #endif
    RCourseSumo(motion, target, target2, sonarSensor, colorSensor);
    // RCoursePrize(motion, sonarSensor);
    RCourseParking(motion, target);
}

void main_task(intptr_t unused) {
    msg_f("Please waite...", 1);
    // バッテリー残量
    char str[64];
    sprintf(str, "%d mV", ev3_battery_voltage_mV());
    msg_f(str, 11);

    ie::Motion motion;
    #ifndef LEFT_COURSE
    ev3api::SonarSensor sonarSensor(ie::SONAR_SENSOR_PORT);
    ev3api::ColorSensor colorSensor(ie::COLOR_SENSOR_PORT);
    #endif

    msg_clear();
    // バッテリー残量
    sprintf(str, "%d mV", ev3_battery_voltage_mV());
    msg_f(str, 11);

    float target;
    float target2; // 土俵用

    init(motion, target, target2); // キャリブレーションとスタート待機

    #ifdef TEST_MODE
    motionTest(motion, target);
    #else

    #ifdef LEFT_COURSE
    leftCourse(motion, target);
    #else
    rightCourse(motion, target, target2, sonarSensor, colorSensor);
    #endif

    #endif
    del(motion);
}
