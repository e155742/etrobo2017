/**
 * @file mileage_stopper.cpp
 * @brief 走行距離で終了判定
 *
 * @author Suguru Kouchi
 */
#include "mileage_stopper.hpp"
#include "robo_meta_datas.hpp"

namespace ie {

MileageStopper::MileageStopper():
leftWheel_(LEFT_WHEEL_PORT), rightWheel_(RIGHT_WHEEL_PORT),
beginMileage_(getMileage()), targetMileage_(0.0) {
}

MileageStopper::MileageStopper(double targetMileage):
leftWheel_(LEFT_WHEEL_PORT), rightWheel_(RIGHT_WHEEL_PORT),
beginMileage_(getMileage()), targetMileage_(targetMileage) {
}

bool MileageStopper::doStop() const {
    if (0 < targetMileage_) {
        if (getMileage() - beginMileage_ < targetMileage_) {
            return false;
        } else {
            return true;
        }
    } else {
        if (targetMileage_ < getMileage() - beginMileage_) {
            return false;
        } else {
            return true;
        }
    }
}

void MileageStopper::setMileage(double targetMileage) {
    beginMileage_ = getMileage();
    targetMileage_ = targetMileage;
}

/**
 * 走行距離を取得する。<br>
 * 左右のタイヤの走行距離の平均を取っている。
 *
 * @return 走行距離
 */
double MileageStopper::getMileage() const {
    return TIRE_OUT_DIAMETER * (leftWheel_.getCount() + rightWheel_.getCount()) / (360.0 * 2.0);
}

}