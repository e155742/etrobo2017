#include <vector>
#include <Motor.h>
#include <SonarSensor.h>
#include "app.h"

#include "motion.hpp"
#include "localization.hpp"

#include "pid_control.hpp"
#include "onoff_control.hpp"

#include "mileage_stopper.hpp"
#include "direction_stopper.hpp"
#include "angle_stopper.hpp"

#include "file_output.hpp"
#include "calibration.hpp"
#include "starter.hpp"

#include "left_idaten.hpp"
#include "left_block.hpp"
#include "left_parking.hpp"

#include "right_prize.hpp"

#include "robo_meta_datas.hpp"
#include "util.h"


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
    msg_f("Please waite...", 1);
    ie::Calibration* calibration = new ie::Calibration();
    threshold = calibration->calibrate() * 0.47; // 普通のライントレースBest0.43
    delete calibration;
    msg_f("Threshold", 7);
    msg_f(threshold, 8);

    motion.raiseArm(0, 15);

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

void hoge(ie::Motion& motion, ie::DirectionStopper& ds, ie::Control& stControl, ie::Control& spControl, ie::point_t pointX, ie::point_t pointY) {
    int pwm = 20;
    int spinPwm = 15;
    ds.setTargetDirection(std::atan2(pointX - localization->getPointX(), pointY - localization->getPointY()));
    motion.spin(ds, spControl, spinPwm);
    motion.wait(200);
    motion.goPoint(*localization, stControl, pwm, pointX, pointY, 15);
    motion.wait(200);
}

void motionTest(ie::Motion& motion) {
    ie::AngleStopper as(360);
    ie::MileageStopper ms(400);
    ie::OnOffControl onoff(0, 0, 0.3, 0);
    // motion.spin(as, onoff, 15);
    motion.goStraight(ms, onoff, 15);
    motion.stop();
    msg_clear();
    msg_f(left.getCount(), 3);
    msg_f(right.getCount(), 4);
}

/**
 * Lコース
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
 */
void rightCourse(ie::Motion& motion, float target) {
    ev3api::SonarSensor sonarSensor(ie::SONAR_SENSOR_PORT);
    // RCourseIdaten(motion);
    RCoursePrize(motion, sonarSensor);
    // ie::Prize prize(motion);
    // prize.prizeCourse();
}

void main_task(intptr_t unused) {
    ie::Motion motion;
    msg_clear();
    float target;

    init(motion, target);

    // goPointTest(motion);
    // motionTest(motion);
    // pidTest();

    leftCourse(motion, target);
    // rightCourse(motion, target);

    del(motion);
}
