/**
 * @file pid_control.hpp
 * @author Suguru Kouchi
 */
#ifndef PID_CONTROL_HPP
#define PID_CONTROL_HPP

#include "control.hpp"

namespace ie {

class PIDControl : public Control {
public:
    PIDControl(float target);
    PIDControl(float target, float kp, float ki, float kd);
    void setKonstant(float kp, float ki, float kd);
    void resetErrors();
    float getControlValue(float value);
    float getTarget() const;
    float getCumulativeError() const;

private:
    float target_;                 // 目標値
    float kp_ = 0.15;              // 比例定数
    float ki_ = 0.03;              // 積分定数
    float kd_ = 0.01;              // 微分定数
    float errorValues_[2] = {0,0}; // 現在と一つ前の偏差
    float cumulativeError_ = 0;    // 偏差の累積
};

}

#endif
