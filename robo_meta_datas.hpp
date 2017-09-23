/**
 * @file robo_meta_datas.hpp
 * @brief モーターやセンサーの接続ポートやロボットの寸法
 *
 * @see https://redmine.ie.u-ryukyu.ac.jp/projects/etrobo2017-teamtwd/wiki/Port
 * @see https://redmine.ie.u-ryukyu.ac.jp/projects/etrobo2017-teamtwd/wiki/Spec
 * @see https://redmine.ie.u-ryukyu.ac.jp/projects/etrobo2017-teamtwd/wiki/Color
 * @author Suguru Kouchi
 */
#ifndef ROBO_META_DATAS_HPP
#define ROBO_META_DATAS_HPP

// #define TWO_UNIT // 二号機用

#include <Port.h>
#include <cmath>

namespace ie {

static constexpr ePortM ARM_MOTOR_PORT    = PORT_A;
static constexpr ePortM RIGHT_WHEEL_PORT  = PORT_B;
static constexpr ePortM LEFT_WHEEL_PORT   = PORT_C;
static constexpr ePortM TAIL_MOTOR_PORT   = PORT_D;

static constexpr ePortS TOUCH_SENSOR_PORT = PORT_1;
static constexpr ePortS SONAR_SENSOR_PORT = PORT_2;
static constexpr ePortS COLOR_SENSOR_PORT = PORT_3;
static constexpr ePortS GYRO_SENSOR_PORT  = PORT_4;

static constexpr double TIRE_OUT_DIAMETER = 1243.5 / 5;   // 360*5度回した走行距離が1243.5mm
static constexpr double ROBOT_TREAD       = M_PI * 126.0; // 右タイヤの中心から左タイヤの中心まで
static constexpr double ANGLE_COEFFICIENT = 1.050;//1.043        // 角度係数 スピン時の論理値と実測の差を埋める

static constexpr double OFF_SET           = 34.0;         // カラーセンサーから車体中心までの距離(mm)

#ifndef TWO_UNIT
// 一号機
static constexpr double RED_COEFFICIENT   = 255.0 / 377.0;
static constexpr double GREEN_COEFFICIENT = 255.0 / 368.0;
static constexpr double BLUE_COEFFICIENT  = 255.0 / 225.0;
#else
// 二号機
static constexpr double RED_COEFFICIENT   = 255.0 / 350.0;
static constexpr double GREEN_COEFFICIENT = 255.0 / 285.0;
static constexpr double BLUE_COEFFICIENT  = 255.0 / 155.0;
#endif

static constexpr double DELTA_T = 0.00078456312;// 0.004;

}

#endif
