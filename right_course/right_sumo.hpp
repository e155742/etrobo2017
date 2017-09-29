#ifndef RIGHT_SUMO_HPP
#define RIGHT_SUMO_HPP

#include <SonarSensor.h>
#include "motion.hpp"

extern void RCourseSumo(ie::Motion& motion, float target, float target2, ev3api::SonarSensor& sonarSensor, ev3api::ColorSensor& colorSensor);

#endif
