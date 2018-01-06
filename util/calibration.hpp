/**
 * @file calibration.hpp
 * @author Suguru Kouchi
 */
#ifndef CALIBRATION_HPP
#define CALIBRATION_HPP

#include <ColorSensor.h>
#include <TouchSensor.h>

namespace ie {

class Calibration {
public:
    Calibration();
    float calibrate();
    void calibrate(float& target);

private:
    ev3api::ColorSensor colorSensor_;
    ev3api::TouchSensor touchSensor_;
    static constexpr int SAMPLE_NUM = 10; // 中央値を得るさいの反射光のデータ数
    static constexpr int DLY_TIME = 200; // 反射光を取る際の時間差
    float pushTouchButton(int line);

};

}

#endif
