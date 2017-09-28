#include <vector>
#include <Motor.h>
#include <SonarSensor.h>
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
#include "right_prize.hpp"

#include "robo_meta_datas.hpp"
#include "util.h"

// #define TEST_MODE
// #define LEFT_COURSE // ブロック並べの方

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
    motion.raiseArm(60, 15); // 動いていることがわかりやすいように
    motion.raiseArm(15, 5);

    // キャリブレーション
    ie::Calibration* calibration = new ie::Calibration();
    threshold = calibration->calibrate() * 0.47; // 普通のライントレースBest0.43
    delete calibration;
    msg_f("Threshold", 7);
    msg_f(threshold, 8);

    motion.wait(1000);
    motion.raiseArm(0, 15); // フロントサスペンション

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
    // msg_f("END...", 10);
    // char str[64];
    // sprintf(str, "%d mV", ev3_battery_voltage_mV());
    // msg_f(str, 11);
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
    // ev3api::SonarSensor sonarSensor(ie::SONAR_SENSOR_PORT);
    // ev3api::Clock clock;
    // msg_clear();
    // int lastdistance = sonarSensor.getDistance();
    // while (true) {
    //     int distance = sonarSensor.getDistance();
    //     if (lastdistance != distance) {
    //         msg_f(distance, 8);
    //     }
    //     lastdistance = distance;
    // }
    ie::LineStopper ls(80);
    ie::PIDControl ltControl(target, 0.15, 0, 0);
    motion.lineTrace(ls, ltControl, 30, false);
}

/**
 * Lコース
 * ブロック並べのほう
 */
void leftCourse(ie::Motion& motion, float target) {
    ie::Decoder decoder;
    int greenPosition = 14; // LCourseBlock()のためのダミー ブロック並べはやらないから使わない

    LCourseIdaten(motion);
    LCourseBlock(motion, target, decoder, greenPosition);
    LCourseParking(motion, target);
}

/**
 * Rコース
 * ET相撲のほう
 */
void rightCourse(ie::Motion& motion, float target, ev3api::SonarSensor sonarSensor) {
    // RCourseIdaten(motion);
    RCourseSumo(motion, target, sonarSensor);
    // RCoursePrize(motion, sonarSensor);
    // ie::Prize prize(motion);
    // prize.prizeCourse();
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
    #endif

    msg_clear();
    // バッテリー残量
    sprintf(str, "%d mV", ev3_battery_voltage_mV());
    msg_f(str, 11);

    float target;

    init(motion, target); // キャリブレーションとスタート待機

    #ifdef TEST_MODE
    motionTest(motion, target);
    #else

    #ifdef LEFT_COURSE
    leftCourse(motion, target);
    #else
    rightCourse(motion, target, sonarSensor);
    #endif

    #endif
    del(motion);
}

