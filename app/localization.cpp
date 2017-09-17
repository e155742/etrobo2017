 /**
 * @file robo_meta_datas.cpp
 * @brief 自己位置推定
 *
 * @author Suguru Kouchi
 */
#include "localization.hpp"
#include <cmath>
#include "util.h"

namespace ie {

Localization::Localization():
leftWheel_(LEFT_WHEEL_PORT), rightWheel_(RIGHT_WHEEL_PORT)
#ifdef OUTPUT_LOCALIZATION
, fox_("X_Point.txt"), foy_("Y_Point.txt")
#endif
{
    leftWheel_.reset();
    rightWheel_.reset();
}

#ifdef OUTPUT_LOCALIZATION
Localization::~Localization() {
    close();
}

void Localization::close() {
    fox_.close();
    foy_.close();
}
#endif

/**
 * 移動距離更新 (単位:mm)
 */
void Localization::updateDistance() {
    changeDistance_ = TIRE_OUT_DIAMETER
                    * ((rightWheel_.getCount() - lastRightCoutn_)
                    +  (leftWheel_.getCount()  - lastLeftCoutn_)) / (360.0 * 2.0);
    lastDistance_ = distance_;
    distance_ += changeDistance_;
}

/**
 * 方位更新 (単位:radian)
 */
void Localization::updateDirection() {
    point_t leftCoutn  = (leftWheel_.getCount()  - lastLeftCoutn_ );
    point_t rightCoutn = (rightWheel_.getCount() - lastRightCoutn_);
    lastLeftCoutn_  = leftWheel_.getCount();
    lastRightCoutn_ = rightWheel_.getCount();

    changeDirection_ = (leftCoutn - rightCoutn) * WHEEL_COUNT_TO_ROBOT_DIRECTION * (M_PI / 180.0);

    direction_ += changeDirection_;
    lastDirection_ = direction_;
}

/**
 * 自己位置更新 (単位:mm)
 */
void Localization::updatePoint() {
    updateDistance();
    updateDirection();
    pointX_ += changeDistance_ * std::sin(lastDirection_ + (changeDirection_ / 2.0));
    pointY_ += changeDistance_ * std::cos(lastDirection_ + (changeDirection_ / 2.0));

    #ifdef PRINT_LOCALIZATION
    msg_f("pointX_", 3);
    msg_f(pointX_, 4);
    msg_f("pointY_", 5);
    msg_f(pointY_, 6);
    msg_f("distance_",  7);
    msg_f(distance_,  8);
    msg_f("direction_", 9);
    msg_f(direction_, 10);
    #endif

    #ifdef OUTPUT_LOCALIZATION
    fox_.fileWrite(pointX_);
    foy_.fileWrite(pointY_);
    #endif
}

/**
 * 任意の座標をセット (単位:mm)
 *
 * @param pointX セットするX座標
 * @param pointY セットするY座標
 */
void Localization::setPoint(point_t pointX, point_t pointY) {
    pointX_ = pointX;
    pointY_ = pointY;
}

/**
 * 任意の自機方位をセット (単位:radian)
 *
 * @param direction セットする方位
 */
void Localization::setDirection(point_t direction) {
    direction_ = direction;
}

/**
 * 自機のX座標を取得 (単位:mm)
 *
 * @return 自機のX座標
 */
point_t Localization::getPointX() const {
    return pointX_;
}

/**
 * 自機のY座標を取得 (単位:mm)
 *
 * @return 自機のY座標
 */
point_t Localization::getPointY() const {
    return pointY_;
}

/**
 * 自機の方位を取得 (単位:radian)
 *
 * @return 自機の方位
 */
point_t Localization::getDirection() const {
    return direction_;
}

}
