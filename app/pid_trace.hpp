/**
 * @file pid_trace.hpp
 * @author Suguru Kouchi
 */
#ifndef PID_TRACE_HPP
#define PID_TRACE_HPP

#include "motion.hpp"
#include "stopper.hpp"

namespace ie {

class PidTrace {
public:
    PidTrace(Motion& motion, float target, bool isRight);
    PidTrace(Motion& motion);
    void pid(Stopper& stopper, int pwm, unsigned int curve, float gain);
    void setTarget(float target);
    void setEdge(bool isRight);

private:
    Motion& motion_;
    float target_ = 0.0;
    float k_[3][3] = {{0.030, 0.0001, 0.019},
                              {0.038, 0.0001, 0.017},
                              {0.065, 0.0007, 0.030}};
    bool isRight_ = false;
};

}

#endif
