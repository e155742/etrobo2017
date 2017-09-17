/**
 * @file localization.hpp
 * @author Suguru Kouchi
 */
#include <Motor.h>
#include "file_output.hpp"
#include "robo_meta_datas.hpp"

#ifndef LOCALIZATION_HPP
#define LOCALIZATION_HPP

// #define PRINT_LOCALIZATION  // 座標と方位と移動距離を画面出力
// #define OUTPUT_LOCALIZATION // 座標の変位をファイル出力

namespace ie {

using point_t = double; // X座標およびY座標で使用する型

class Localization {
public:
    Localization();
    #ifdef OUTPUT_LOCALIZATION
    ~Localization();
    void close();
    #endif
    void updatePoint();
    void setPoint(point_t pointX, point_t pointY);
    void setDirection(point_t direction);
    point_t getPointX() const;
    point_t getPointY() const;
    point_t getDirection() const;

private:
    void updateDistance();
    void updateDirection();
    ev3api::Motor leftWheel_;
    ev3api::Motor rightWheel_;
    #ifdef OUTPUT_LOCALIZATION
    FileOutput fox_;
    FileOutput foy_;
    #endif
    point_t pointX_    = 0; // 右がプラス
    point_t pointY_    = 0; // 前がプラス
    point_t distance_  = 0; // 移動距離
    point_t direction_ = 0; // 自機方位 (ラジアン) 時計回りに正
    point_t changeDistance_  = 0; // 移動距離変化量
    point_t changeDirection_ = 0; // 自機方位変化量
    point_t lastDistance_   = 0;
    point_t lastDirection_  = 0;
    int32_t lastLeftCoutn_  = 0;
    int32_t lastRightCoutn_ = 0;
    static constexpr double WHEEL_COUNT_TO_ROBOT_DIRECTION = TIRE_OUT_DIAMETER / (ANGLE_COEFFICIENT * ROBOT_TREAD * 2.0);
};

}

#endif
