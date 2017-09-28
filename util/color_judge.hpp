/**
 * @file color_judge.hpp
 * @author Suguru Kouchi
 */
#ifndef COLOR_JUDGE_HPP
#define COLOR_JUDGE_HPP

#include "hsv_converter.hpp"
#include "ev3api.h"

namespace ie {

class ColorJudge {
public:
    ColorJudge(HsvConverter& hsvConverter);
    colorid_t getColorNumber(double red, double green, double blue);

private:
    HsvConverter hsvConverter_;

};

}

# endif

