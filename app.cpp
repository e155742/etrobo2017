#include <vector>
#include "app.h"
#include "util.h"

#include "decoder.hpp"
#include "hsv_converter.hpp"
#include "file_output.hpp"

#include "control.hpp"
#include "pid_control.hpp"
#include "onoff_control.hpp"
#include "move.hpp"
#include "ports_assignment.hpp"

/**
 * 20ms間隔で呼び出される周期ハンドラ
 */
void sub_cyc(intptr_t exinf) {
    static int a = 0;
    a++;
    msg_f(a, 12);
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
    move.goStraight(*stControl, 10 * 15, 10);
    move.spin(*stControl, -360, 15);

    // move.goStraight(*stPid, 10 * -15, 20) と同じ
    ie::PIDControl* stPid = new ie::PIDControl(left.getCount() - right.getCount(), 2, 0, 0);
    int begin = move.getMileage();
    while (true) {
        if (10 * 15 < std::abs(move.getMileage() - begin)) {
            delete stPid;
            break;
        }
        move.goStraight(*stPid, -20);
    }

    while (true) {
        move.lineTrace(*ltControl, 30);
    }
    move.stop();
    delete ltControl;
    delete stControl;
}

void main_task(intptr_t unused) {
    ev3_sta_cyc(SUB_CYC);
    ioTest();
    dly_tsk(1000 * 3);
    msg_clear();
    moveTest();
    ev3_stp_cyc(SUB_CYC);
}
