#include "pid.hpp"
#include "pid_control.hpp"
#include "mileage_stopper.hpp"

void pid(ie::Motion& motion, float target, int mile, int pwm, float kp, float ki, float kd, bool isRight) {
    ie::PIDControl ltControl(target, kp, ki, kd);
    ie::MileageStopper stopper(mile);
    motion.lineTrace(stopper, ltControl, pwm, false);
}

void straightPid(ie::Motion& motion, float target, int mile, int speed){
    // (motion, 閾値, 距離, 速度,  kp, ki, kd,   isRight);
    pid(motion, target, mile, speed, 0.030, 0.0001, 0.019, false); // 弱カーブP弱
}

void gentleCurvePid(ie::Motion& motion, float target, int mile, int speed){
    // (motion, 閾値, 距離, 速度,  kp, ki, kd,   isRight);
    pid(motion, target, mile, speed, 0.038, 0.0001, 0.017, false); // 弱カーブP弱
}

void sharpCurvePid(ie::Motion& motion, float target, int mile, int speed, float gain){
    // (motion, 閾値, 距離, 速度,  kp, ki, kd,   isRight);
    pid(motion, target, mile, speed, 0.065 * gain, 0.0007 * gain, 0.030 * gain, false); // I弱D強
}
