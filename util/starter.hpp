/**
 * @starter.hpp
 * @author Suguru Kouchi
 */
#ifndef STARTER_HPP
#define STARTER_HPP

#include <TouchSensor.h>

namespace ie {

class Starter{
public:
    Starter();
    void startWait();

private:
    ev3api::TouchSensor touchSensor_;

};

}

#endif
