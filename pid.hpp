#ifndef PID_HPP
#define PID_HPP

#include "motion.hpp"

extern void pid(ie::Motion& motion, float target, int mile, int pwm, float kp, float ki, float kd, bool isRight);
extern void straightPid(ie::Motion& motion, float target, int mile, int speed);
extern void gentleCurvePid(ie::Motion& motion, float target, int mile, int speed);
extern void sharpCurvePid(ie::Motion& motion, float target, int mile, int speed, float gain);

#endif
