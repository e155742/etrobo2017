#include <vector>
#include <cmath>
#include "app.h"
#include "util.h"

#include <ColorSensor.h>
#include "robo_meta_datas.hpp"

#include "decoder.hpp"
#include "hsv_converter.hpp"
#include "file_output.hpp"

#include "control.hpp"
#include "pid_control.hpp"
#include "onoff_control.hpp"

#include "localization.hpp"
#include "move.hpp"
#include "motion.hpp"

#include "mileage_stopper.hpp"
#include "angle_stopper.hpp"

ie::Localization* localization;
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
 * 入出力のテスト<br>
 * RGB-HSV変換とブロック並べのデコード<br>
 * for文によるファイル出力テスト
 */
void ioTest() {
    std::vector<ie::rgb_t> rgb = {0, 0, 0};
    inputInt(rgb, "Enter RGB");
    msg_clear();
    ie::HsvConverter hsv(rgb[0], rgb[1], rgb[2]);
    msg_f("HSV color", 1);
    char str[64];
    sprintf(str, "Hue        %f", hsv.getHue());
    msg_f(str, 3);
    sprintf(str, "Saturation %f", hsv.getSaturation());
    msg_f(str, 4);
    sprintf(str, "Value      %f", hsv.getLightness());
    msg_f(str, 5);

    ie::Decoder d(12008);
    msg_f("Black, Red, Yellow, Blue", 7);
    sprintf(str, "%d,    %d,   %d,      %d", d.getBlackPosition(), d.getRedPosition(), d.getYellowPosition(), d.getBluePosition());
    msg_f(str, 8);

    ie::FileOutput fo("test.txt");
    for (double i = 10.12345678900; i < 10.12345678915; i+=0.00000000001) {
        fo.fileWrite(i, "%2.13f");
    }
    fo.close();
}

/**
 * 実際に動かして見る
 */
void moveTest() {
    // PIDの各種定数
    const float kp = 3.0; // 比例定数
    const float ki = 0.0; // 積分定数
    const float kd = 0.0; // 微分定数

    const float threshold = 36.5;

    ev3api::Motor left(ie::LEFT_WHEEL_PORT);
    ev3api::Motor right(ie::RIGHT_WHEEL_PORT);
    ie::PIDControl* ltControl = new ie::PIDControl(threshold, kp, ki, kd);
    ie::OnOffControl* stControl = new ie::OnOffControl(0, 0, 0.3, 0);
    ie::Move move;

    move.raiseArm(60, 10);
    move.raiseArm(15, 5);
    move.rotateTail(360, 10);
    move.goStraight(*stControl, 15 * 10, 10);
    move.spin(*stControl, -360, 15);

    // move.goStraight(*stPid, -15 * 10, 20) と同じ
    ie::PIDControl* stPid = new ie::PIDControl(left.getCount() - right.getCount(), 2, 0, 0);
    int begin = move.getMileage();
    while (true) {
        if (15 * 10 < std::abs(move.getMileage() - begin)) {
            delete stPid;
            break;
        }
        move.goStraight(*stPid, -20);
    }

    while (move.getMileage() < 3.5 * 1000) {
        move.lineTrace(*ltControl, 30, true);
    }

    move.stop();
    delete ltControl;
    delete stControl;
}

/*
 * ブロック並べフィールドを移動。入口からスタート。
 * 黒マーカーでいうところの以下の番号を通る。
 * 10 -> 5 -> 1 -> 13 -> 2 -> 10
 */
void goPointTest() {
    ie::OnOffControl* stControl = new ie::OnOffControl(0, 0, 2, 0);
    ie::OnOffControl* spControl = new ie::OnOffControl(0, 0, 0.3, 0);
    ie::Move move;

    localization->setDirection(30 * M_PI/180.0); // 入口のラインの角度が30度
                                                    // X座標 Y座標 出力 回転出力
    move.goPoint(*stControl, *spControl, *localization, 225, 389.7, 20, 15);
    move.goPoint(*stControl, *spControl, *localization, -164.7, 614.7, 20, 15);
    move.goPoint(*stControl, *spControl, *localization, 839.7, -225, 20, 15);
    move.goPoint(*stControl, *spControl, *localization, 614.7, 614.7, 20, 15);
    move.goPoint(*stControl, *spControl, *localization, 0, 0, 20, 15);

    move.stop();
    delete stControl;
    delete spControl;
}

void motionTest() {
    // PIDの各種定数
    const float kp = 3.0; // 比例定数
    const float ki = 0.0;   // 積分定数
    const float kd = 0.00; // 微分定数
    const float threshold = (30 + 708) * 0.47;
    ie::PIDControl ltControl(threshold, kp, ki, kd);
    // ie::OnOffControl ltControl(threshold, 0, 0, 100);
    ie::Motion motion;

    motion.raiseArm(15, 5);
    dly_tsk(2000);

    ie::MileageStopper ms(2000);
    motion.lineTrace(ms, ltControl, 100, true);
}

void main_task(intptr_t unused) {
    // ioTest();
    // dly_tsk(3 * 1000);
    // msg_clear();

    ev3_sta_cyc(SUB_CYC);
    // moveTest();
    // goPointTest();
    motionTest();
    ev3_stp_cyc(SUB_CYC);
    delete localization;
}
