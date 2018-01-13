/**
 * @file pid_trace.cpp
 * @brief PIDによるライントレース
 *
 * @author Suguru Kouchi
 */

#include "pid_trace.hpp"
#include <cassert>
#include "pid_control.hpp"

namespace ie {

PidTrace::PidTrace(Motion& motion, float target, bool isRight):
motion_(motion), target_(target), isRight_(isRight){}

PidTrace::PidTrace(Motion& motion):
motion_(motion), target_(0.0), isRight_(false){}

void PidTrace::pid(Stopper& stopper, int pwm, unsigned int curve, float gain) {
    assert(curve < (sizeof(k_) / sizeof(k_[0])));
    ie::PIDControl ltControl(target_, k_[curve][0] * gain, k_[curve][1] * gain, k_[curve][2] * gain * 0.2);
    motion_.lineTraceK(stopper, ltControl, pwm, isRight_);
}

void PidTrace::setTarget(float target) {
    target_ = target;
}

void PidTrace::setEdge(bool isRight) {
    isRight_ = isRight;
}

}