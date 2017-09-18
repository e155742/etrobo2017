/**
 * @file onoff_control.cpp
 * @brief ON/OFFコントロール
 *
 * @author Suguru Kouchi
 */
#include "onoff_control.hpp"
#include <cstdlib>
#include <cmath>

namespace ie {

/**
 * 目標値のみ指定して初期化。<br>
 * Steeringクラスで使用時に、単純な0%-100%のon/off制御になる。
 */
OnOffControl::OnOffControl(float target):
target_(target) {}

/**
 * 制御用の二次方程式の係数のみ指定して初期化。
 */
OnOffControl::OnOffControl(float a, float b, float c):
a_(a), b_(b), c_(c) {}

/**
 * 目標値と制御用の二次方程式の係数を指定して初期化。
 */
OnOffControl::OnOffControl(float target, float a, float b, float c):
target_(target), a_(a), b_(b), c_(c) {}

void OnOffControl::setTarget(float target) {
    target_ = target;
}

/**
 * 制御量を求めるの式の係数a,b,cを指定する。式は単純な二次方程式で以下のようになる。<br>
 * a * pwm^2 + b* pwm + c
 *
 * @param a pwm^2にかかる2次の係数
 * @param b pwmにかかる1次の係数
 * @param c 定数項
 */
void OnOffControl::setCoefficient(float a, float b, float c) {
    a_ = a;
    b_ = b;
    c_ = c;
}

/**
 * 制御量を取得。<br>
 * pwmの式で制御量を求めている。
 *
 * @param value 入力値
 * @return      制御量
 */
float OnOffControl::getControlValue(float value) {
    float controlValue = std::pow(pwm_, 2) * a_ + std::abs(pwm_) * b_ + c_;

    if (value < target_) {
        return -controlValue;
    // 右 < 左  (右旋回時)
    } else if (target_ < value) {
        return controlValue;
    // 右 == 左 (直進時)
    } else {
        return 0;
    }
}

/**
 * 前進や回転する際のモーターの出力と同じ値を入力する。
 *
 * @param pwm モーターのパワー
 */
void OnOffControl::setPwm(int pwm) {
    pwm_ = pwm;
}

}
