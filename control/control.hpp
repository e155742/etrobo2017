/**
 * @file control.hpp
 * @author Suguru Kouchi
 */
#ifndef CONTROL_HPP
#define CONTROL_HPP

namespace ie {

class Control {
public:
    virtual ~Control(){};
    virtual void setTarget(float target) = 0;
    virtual float getControlValue(float value) = 0;
};

}

#endif
