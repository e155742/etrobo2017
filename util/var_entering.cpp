/**
 * @file var_entering.cpp
 * @brief 本体から数値入力。<br>
 * 左右ボタンで値の増減、vectorの場合上下ボタンで変更したい値の選択。中央ボタンで入力を確定。<br>
 * なんか似たような関数ばっかりになってしまった。クソコード。
 *
 * @author Suguru Kouchi
 */
#include "var_entering.hpp"
#include <cmath>
#include "util.h"

namespace ie {

/**
 * 本体のボタンから変数を変更。<br>
 * int型用。
 *
 * @param num 変更したい変数
 */
void VarEntering::pushButton(int& num) {
    while(true){
        if (ev3_button_is_pressed(LEFT_BUTTON)) {
            msg_f(--num, firstLine_);
            return;
        } else if (ev3_button_is_pressed(RIGHT_BUTTON)) {
            msg_f(++num, firstLine_);
            return;
        } else if (ev3_button_is_pressed(ENTER_BUTTON)) {
            doContinue_ = false;
            return;
        }
    }
}

/**
 * 本体のボタンから変数を変更。<br>
 * float型用。floatは一度int型に変換するので引数の型はint型。
 *
 * @param numInt 変更したい変数
 */
void VarEntering::pushButtonFloat(int& numInt) {
    float f;
    while(true){
        if (ev3_button_is_pressed(LEFT_BUTTON)) {
            numInt--;
            f = static_cast<float>(numInt) / accuracy_;
            msg_f(f, firstLine_);
            return;
        } else if (ev3_button_is_pressed(RIGHT_BUTTON)) {
            numInt++;
            f = static_cast<float>(numInt) / accuracy_;
            msg_f(f, firstLine_);
            return;
        } else if (ev3_button_is_pressed(ENTER_BUTTON)) {
            doContinue_ = false;
            return;
        }
    }
}

/**
 * 本体のボタンから変数を変更。<br>
 * vector<int>型用。
 *
 * @param nums          変更したいvector
 * @param maxIndex      numsの要素数
 * @param selectLineStr メッセージ"Select line"を表示させる行
 */
void VarEntering::pushButton(std::vector<int>& nums, int maxIndex, int32_t selectLineStr) {
    while(true){
        if (ev3_button_is_pressed(LEFT_BUTTON)) {
            msg_f(--nums[vectorIndex_], firstLine_ + vectorIndex_);
            return;
        } else if (ev3_button_is_pressed(RIGHT_BUTTON)) {
            msg_f(++nums[vectorIndex_], firstLine_ + vectorIndex_);
            return;
        } else if (ev3_button_is_pressed(UP_BUTTON)) {
            if (0 < vectorIndex_) {
                msg_f(--vectorIndex_ + 1, selectLineStr + 1);
            }
            return;
        } else if (ev3_button_is_pressed(DOWN_BUTTON)) {
            if (vectorIndex_ < maxIndex - 1) {
                msg_f(++vectorIndex_ + 1, selectLineStr + 1);
            }
            return;
        } else if (ev3_button_is_pressed(ENTER_BUTTON)) {
            vectorIndex_ = 0;
            doContinue_ = false;
            return;
        }
    }
}

/**
 * 本体のボタンから変数を変更。<br>
 * vector<float>型用。floatは一度int型に変換するので引数の型はvector<int>。
 *
 * @param nums          変更したいvector
 * @param maxIndex      numsの要素数
 * @param selectLineStr メッセージ"Select line"を表示させる行
 */
void VarEntering::pushButtonFloat(std::vector<int>& numInts, int maxIndex, int32_t selectLineStr) {
    float f;
    while(true){
        if (ev3_button_is_pressed(LEFT_BUTTON)) {
            numInts[vectorIndex_]--;
            f = static_cast<float>(numInts[vectorIndex_]) / accuracy_;
            msg_f(f, firstLine_ + vectorIndex_);
            return;
        } else if (ev3_button_is_pressed(RIGHT_BUTTON)) {
            numInts[vectorIndex_]++;
            f = static_cast<float>(numInts[vectorIndex_]) / accuracy_;
            msg_f(f, firstLine_ + vectorIndex_);
            return;
        } else if (ev3_button_is_pressed(UP_BUTTON)) {
            if (0 < vectorIndex_) {
                msg_f(--vectorIndex_ + 1, selectLineStr + 1);
            }
            return;
        } else if (ev3_button_is_pressed(DOWN_BUTTON)) {
            if (vectorIndex_ < maxIndex - 1) {
                msg_f(++vectorIndex_ + 1, selectLineStr + 1);
            }
            return;
        } else if (ev3_button_is_pressed(ENTER_BUTTON)) {
            vectorIndex_ = 0;
            doContinue_ = false;
            return;
        }
    }
}

/**
 * int型のボタン入力のループと画面表示。
 *
 * @param num 変更したい変数
 */
void VarEntering::inputInt(int& num) {
    msg_f(num, firstLine_);
    while (doContinue_) {
        dly_tsk(DELAY_TIME);
        pushButton(num);
    }
    doContinue_ = true;
}

/**
 * int型のボタン入力のループと画面表示。タイトル付き。
 *
 * @param num   変更したい変数
 * @param title 表示するタイトル
 */
void VarEntering::inputInt(int& num, const char* title) {
    msg_f(title, titleLine_);
    firstLine_++;
    inputInt(num);
    firstLine_--;
}

/**
 * vector<int>型のボタン入力のループと画面表示。
 *
 * @param nums 変更したいvector<int>
 */
void VarEntering::inputInt(std::vector<int>& nums) {
    const int maxIndex = nums.size();
    const int32_t selectLineStr = firstLine_ + maxIndex + 1;
    msg_f("Select line", selectLineStr);
    msg_f(vectorIndex_ + 1, selectLineStr + 1);
    for (int i = 0; i < maxIndex; i++) {
        msg_f(nums[i], firstLine_ + i);
    }
    while (doContinue_) {
        dly_tsk(DELAY_TIME);
        pushButton(nums, maxIndex, selectLineStr);
    }
    doContinue_ = true;
}

/**
 * vector<int>型のボタン入力のループと画面表示。タイトル付き。
 *
 * @param nums  変更したいvector<int>
 * @param title 表示するタイトル
 */
void VarEntering::inputInt(std::vector<int>& nums, const char* title) {
    msg_f(title, titleLine_);
    firstLine_++;
    inputInt(nums);
    firstLine_--;
}

/**
 * float型のボタン入力のループと画面表示。
 *
 * @param num 変更したい変数
 */
void VarEntering::inputFloat(float& num) {
    msg_f(num, firstLine_);
    int numInt = std::roundf(num * accuracy_);
    while (doContinue_) {
        dly_tsk(DELAY_TIME);
        pushButtonFloat(numInt);
    }
    num = static_cast<float>(numInt) / accuracy_;
    doContinue_ = true;
}

/**
 * float型のボタン入力のループと画面表示。タイトル付き。
 *
 * @param num   変更したい変数
 * @param title 表示するタイトル
 */
void VarEntering::inputFloat(float& num, const char* title) {
    msg_f(title, titleLine_);
    firstLine_++;
    inputFloat(num);
    firstLine_--;
}

/**
 * vector<float>型のボタン入力のループと画面表示。タイトル付き。
 *
 * @param nums 変更したいvector<gloat>
 */
void VarEntering::inputFloat(std::vector<float>& nums) {
    const int maxIndex = nums.size();
    const int32_t selectLineStr = firstLine_ + maxIndex + 1;
    msg_f("Select line", selectLineStr);
    msg_f(vectorIndex_ + 1, selectLineStr + 1);
    std::vector<int> numInts;
    for (int i = 0; i < maxIndex; i++) {
        msg_f(nums[i], firstLine_ + i);
        numInts.push_back(std::roundf(nums[i] * accuracy_));
    }
    while (doContinue_) {
        dly_tsk(DELAY_TIME);
        pushButtonFloat(numInts, maxIndex, selectLineStr);
    }
    for (int i = 0; i < maxIndex; i++) {
        nums[i] = static_cast<float>(numInts[i]) / accuracy_;
    }
    doContinue_ = true;
}

/**
 * vector<float>型のボタン入力のループと画面表示。タイトル付き。
 *
 * @param nums  変更したいvector<float>
 * @param title 表示するタイトル
 */
void VarEntering::inputFloat(std::vector<float>& nums, const char* title) {
    msg_f(title, titleLine_);
    firstLine_++;
    inputFloat(nums);
    firstLine_--;
}

/**
 * float型を変更する際の精度を設定。100だと1/100刻み、1000だと1/1000刻みで値を変更できる。<br>
 * 普通は10のべき乗倍を入れる。0は入れるな。
 *
 * @param accuracy float型の精度
 */
void VarEntering::setAccuracy(int accuracy) {
    if (accuracy == 0) {
        return;
    }
    accuracy_ = accuracy;
}

}
