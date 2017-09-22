#ifndef LEFT_COURSE_HPP
#define LEFT_COURSE_HPP

#include "motion.hpp"
#include "localization.hpp"

extern void LCourseIdaten(ie::Motion& motion);
extern void LCourseParking(ie::Motion& motion, float target);

#endif
