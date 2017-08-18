/**
 * @file ports_assignment.hpp
 * @brief 各ポートに接続されているモーターやセンサーを指定
 *
 * @see https://redmine.ie.u-ryukyu.ac.jp/projects/etrobo2017-teamtwd/wiki/Port
 * @author Suguru Kouchi
 */
#ifndef PORTS_ASSIGNMENT_HPP
#define PORTS_ASSIGNMENT_HPP

#include <Port.h>

namespace ie {

static constexpr ePortM ARM_MOTOR_PORT    = PORT_A;
static constexpr ePortM RIGHT_WHEEL_PORT  = PORT_B;
static constexpr ePortM LEFT_WHEEL_PORT   = PORT_C;
static constexpr ePortM TAIL_MOTOR_PORT   = PORT_D;

static constexpr ePortS TOUCH_SENSOR_PORT = PORT_1;
static constexpr ePortS SONAR_SENSOR_PORT = PORT_2;
static constexpr ePortS COLOR_SENSOR_PORT = PORT_3;
static constexpr ePortS GYRO_SENSOR_PORT  = PORT_4;

}

#endif
