/**
 * @file masker.cpp
 * @brief ブロック置き場
 *
 * @author Suguru Kouchi
 */
#include "marker.hpp"

namespace ie {

Marker::Marker():
priorityNum_(0), pointX_(0), pointY_(0), haveBlockColor_(COLOR_NONE) {
}

Marker::Marker(int priorityNum, point_t pointX, point_t pointY):
priorityNum_(priorityNum), pointX_(pointX), pointY_(pointY), haveBlockColor_(COLOR_NONE) {
}

Marker::Marker(int priorityNum, point_t pointX, point_t pointY, colorid_t haveBlockColor):
priorityNum_(priorityNum), pointX_(pointX), pointY_(pointY), haveBlockColor_(haveBlockColor) {
}

void Marker::setBlock(colorid_t haveBlockColor) {
    haveBlockColor_ = haveBlockColor;
}

bool Marker::haveColorBlock() const {
    if (haveBlockColor_ == COLOR_NONE || haveBlockColor_ == COLOR_BLACK) {
        return false;
    } else {
        return true;
    }
}

void Marker::setFields(int priorityNum, point_t pointX, point_t pointY, colorid_t haveBlockColor) {
    priorityNum_ = priorityNum;
    pointX_ = pointX;
    pointY_ = pointY;
    haveBlockColor_ = haveBlockColor;
}

point_t Marker::getPointX() const {
    return pointX_;
}

point_t Marker::getPointY() const {
    return pointY_;
}

int Marker::getPriorityNum() const {
    return priorityNum_;
}

}