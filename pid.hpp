#ifndef PID_HPP
#define PID_HPP

#include "motion.hpp"

extern void pid(ie::Motion& motion, ie::Stopper& stopper, float target, int pwm, float kp, float ki, float kd, bool isRight);
extern void straightPid(ie::Motion& motion, ie::Stopper& stopper, float target, int speed, float gain);
extern void gentleCurvePid(ie::Motion& motion, ie::Stopper& stopper, float target, int speed, float gain);
extern void sharpCurvePid(ie::Motion& motion, ie::Stopper& stopper, float target, int speed, float gain);

#endif
