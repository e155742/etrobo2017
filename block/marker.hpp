/**
 * @file marker.hpp
 * @author Suguru Kouchi
 */
#ifndef MARKER_HPP
#define MARKER_HPP

#include "localization.hpp"

namespace ie {

class Marker {
public:
    Marker();
    Marker(int priorityNum, point_t pointX, point_t pointY);
    Marker(int priorityNum, point_t pointX, point_t pointY, colorid_t haveBlockColor);
    void setBlock(colorid_t haveBlockColor);
    bool haveColorBlock() const;
    void setFields(int priorityNum, point_t pointX, point_t pointY, colorid_t haveBlockColor);
    point_t getPointX() const;
    point_t getPointY() const;
    int getPriorityNum() const;

private:
    int priorityNum_;
    point_t pointX_;
    point_t pointY_;
    colorid_t haveBlockColor_;
};

}

#endif
