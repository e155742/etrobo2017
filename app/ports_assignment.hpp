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

ePortM ARM_MOTOR_PORT    = PORT_A;
ePortM RIGHT_WHEEL_PORT  = PORT_B;
ePortM LEFT_WHEEL_PORT   = PORT_C;
ePortM TAIL_MOTOR_PORT   = PORT_D;

ePortS TOUCH_SENSOR_PORT = PORT_1;
ePortS SONAR_SENSOR_PORT = PORT_2;
ePortS COLOR_SENSOR_PORT = PORT_3;
ePortS GYRO_SENSOR_PORT  = PORT_4;

#endif
