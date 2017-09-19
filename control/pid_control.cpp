/**
 * @file pid_control.cpp
 * @brief PIDコントロール
 *
 * @see http://www.picfun.com/motor05.html
 * @see http://monoist.atmarkit.co.jp/mn/articles/1007/26/news083.html
 * @author Suguru Kouchi
 */
#include "pid_control.hpp"
#include "robo_meta_datas.hpp"

namespace ie {

/**
 * 目標値のみを設定。
 *
 * @param target 目標値
 */
PIDControl::PIDControl(float target):
target_(target) {}

/**
 * 各種定数のみ設定。
 *
 * @param kp     比例定数
 * @param ki     積分定数
 * @param kd     微分定数
 */
PIDControl::PIDControl(float kp, float ki, float kd):
kp_(kp), ki_(ki), kd_(kd) {}

/**
 * 目標値と各種定数を設定。
 *
 * @param target 目標値
 * @param kp     比例定数
 * @param ki     積分定数
 * @param kd     微分定数
 */
PIDControl::PIDControl(float target, float kp, float ki, float kd):
target_(target), kp_(kp), ki_(ki), kd_(kd) {}

/**
 * 各種定数を変更します。Konstantはドイツ語で定数。
 *
 * @param kp 比例定数
 * @param ki 積分定数
 * @param kd 微分定数
 */
void PIDControl::setKonstant(float kp, float ki, float kd) {
    kp_ = kp;
    ki_ = ki;
    kd_ = kd;
}

/**
 * 前回と今回の偏差とこれまでの偏差の累積を0にする。
 */
void PIDControl::resetErrors() {
    errorValues_[0]  = 0;
    errorValues_[1]  = 0;
    cumulativeError_ = 0;
}

void PIDControl::setTarget(float target) {
    target_ = target;
}

/**
 * 制御量を取得。ここでPIDの計算を行なっている。
 *
 * @param value 入力値
 * @return      制御量
 */
float PIDControl::getControlValue(float value) {
    errorValues_[1] = errorValues_[0];                                         // 前回の偏差
    errorValues_[0] = value - target_;                                         // 今回の偏差
    cumulativeError_ += (errorValues_[0] + errorValues_[1]) / 2.0 * DELTA_T; // 偏差の累積

    float controlValue = kp_ * errorValues_[0];                          // 比例
    controlValue += ki_ * cumulativeError_;                              // 積分
    controlValue += kd_ * (errorValues_[0] - errorValues_[1]) / DELTA_T; // 微分

    return controlValue;
}

/**
 * 目標値を取得。
 *
 * @return 目標値
 */
float PIDControl::getTarget() const {
    return target_;
}

/**
 * 偏差の累計を取得。
 *
 * @return 偏差の累計
 */
float PIDControl::getCumulativeError() const {
    return cumulativeError_;
}

}
