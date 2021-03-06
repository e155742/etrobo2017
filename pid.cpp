#include "pid.hpp"
#include "pid_control.hpp"
#include "mileage_stopper.hpp"

void pid(ie::Motion& motion, ie::Stopper& stopper, float target,int pwm, float kp, float ki, float kd, bool isRight) {
    ie::PIDControl ltControl(target, kp, ki, kd);
    motion.lineTraceK(stopper, ltControl, pwm, false);
}

void straightPid(ie::Motion& motion, ie::Stopper& stopper, float target, int speed, float gain){
    // (motion, 閾値, 距離, 速度,  kp, ki, kd,   isRight);
    pid(motion, stopper, target, speed, 0.030*gain, 0.0001*gain, 0.019*gain*0.2, false); // 弱カーブP弱
}

void gentleCurvePid(ie::Motion& motion,ie::Stopper& stopper, float target, int speed, float gain){
    // (motion, 閾値, 距離, 速度,  kp, ki, kd,   isRight);
    pid(motion, stopper, target, speed, 0.038*gain, 0.0001*gain, 0.017*gain*0.2, false); // 弱カーブP弱
}

void sharpCurvePid(ie::Motion& motion,ie::Stopper& stopper, float target, int speed, float gain){
    // (motion, 閾値, 距離, 速度,  kp, ki, kd,   isRight);
    pid(motion, stopper, target, speed, 0.065 * gain, 0.0007 * gain, 0.030 * gain*0.2, false); // I弱D強
}
