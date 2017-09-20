/**
 * @file angle_stopper.cpp
 * @brief 旋回角度で終了判定
 *
 * @author Suguru Kouchi
 */
#include "angle_stopper.hpp"
#include "util.h"
#include "robo_meta_datas.hpp"

namespace ie {

AngleStopper::AngleStopper():
leftWheel_(LEFT_WHEEL_PORT), rightWheel_(RIGHT_WHEEL_PORT) {
    setAngle(0.0);
}

AngleStopper::AngleStopper(double targetAngle):
leftWheel_(LEFT_WHEEL_PORT), rightWheel_(RIGHT_WHEEL_PORT) {
    setAngle(targetAngle);
}

bool AngleStopper::doStop() {
    //  (左ホイール回転量 - 右ホイール回転量) / 2
    int32_t diffCount = roundInt32_t((leftWheel_.getCount() - beginLeftCount_ + beginRightCount_ - rightWheel_.getCount()) / 2.0);
    if (0 < targetWheelAngle_) {
        if (targetWheelAngle_ < diffCount) {
            return true;
        }
    } else {
        if (diffCount < targetWheelAngle_) {
            return true;
        }
    }
    return false;
}

/**
 * 目標となる車体の旋回角度を指定。
 *
 * @param targetAngle 旋回させたい角度
 */
void AngleStopper::setAngle(double targetAngle) {
    beginLeftCount_  = leftWheel_.getCount();
    beginRightCount_ = rightWheel_.getCount();

    // 回転するホイールの角度 = (車体を回転させたい角度)*(車体が一回転する円の外周/車輪の外周)
    targetWheelAngle_ = roundInt32_t(ANGLE_COEFFICIENT * targetAngle * (ROBOT_TREAD / TIRE_OUT_DIAMETER));
}

}