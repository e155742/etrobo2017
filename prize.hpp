/**
 * @file prize.hpp
 * @author Akari Minowa
 */
#ifndef PRIZE_HPP
#define PRIZE_HPP

#include <SonarSensor.h>
#include "motion.hpp"
#include "util.h"
#include "robo_meta_datas.hpp"

namespace ie {

class Prize {
public:
    Prize(Motion& motion);
    void prizeCourse();

private:
    ev3api::Motor leftWheel_;
    ev3api::Motor rightWheel_;
    ev3api::SonarSensor sonarSensor_;
    Motion& motion_;
    const int distOnPrize_ = 3;  //懸賞前で止まるための距離
    const int distOnTrain_ = 35; //電車を避ける距離

};

}

#endif
