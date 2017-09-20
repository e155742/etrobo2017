/**
 * @file direction_stoper.cpp
 * @brief 方位で終了判定
 *
 * @author Suguru Kouchi
 */
#include "direction_stoper.hpp"
#include <cmath>
#include "util.h"
#include "robo_meta_datas.hpp"

namespace ie {

DirectionStopper::DirectionStopper(Localization& localization):
localization_(localization), targetDirection_(0.0) {}

DirectionStopper::DirectionStopper(Localization& localization, point_t targetDirection):
localization_(localization), targetDirection_(radianNormalize(targetDirection)) {}

bool DirectionStopper::doStop() {
    point_t nowDirection = radianNormalize(localization_.getDirection());
    point_t uNowDirection = nowDirection < 0 ? nowDirection + 2 * M_PI : nowDirection;
    point_t uLastDirection = lastDirection_ < 0 ? lastDirection_ + 2 * M_PI : lastDirection_;
    point_t uTargetDirection = targetDirection_ < 0 ? targetDirection_ + 2 * M_PI : targetDirection_;

    if (nowDirection == targetDirection_) { return true; }

    // プラマイ90度以上の方位を向く場合は、方位の範囲を0 < 2*MIに正規化して判定する。
    // -179度->180度、359度->0度を起こさないため
    if (std::abs(targetDirection_) < M_PI / 2.0) {
        if (((lastDirection_ < targetDirection_ && targetDirection_ < nowDirection)
        || (nowDirection < targetDirection_ && targetDirection_ < lastDirection_))
        && (std::abs(nowDirection - lastDirection_) < M_PI)) {
            return true;
       }
    } else {
        if (((uLastDirection < uTargetDirection && uTargetDirection < uNowDirection)
        || (uNowDirection < uTargetDirection && uTargetDirection < uLastDirection))
        && (std::abs(uNowDirection - uLastDirection) < M_PI)) {
            return true;
        }
    }

    lastDirection_ = nowDirection;
    return false;
}

/**
 * 目標となる車体の方位を指定。
 *
 * @param targetDirection 向けたい方位(radian)
 */
void DirectionStopper::setTargetDirection(point_t targetDirection) {
    targetDirection_ = radianNormalize(targetDirection);
}

/**
 * 目標となる車体の方位を返す。
 *
 * @return 目標の方位
 */
point_t DirectionStopper::getTargetDirection() const {
    return targetDirection_;
}

/**
 * 現在の車体の方位を返す。<br>
 * Localization.getDirection()を呼び出し、-PI<PIに正規化しているだけ。
 *
 * @return 車体の方位
 */
point_t DirectionStopper::getDirection() const {
    return radianNormalize(localization_.getDirection());
}

}