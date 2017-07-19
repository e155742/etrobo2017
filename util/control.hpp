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
    virtual float getControlValue(float value) = 0;
};

}

#endif
