/**
 * @file stopper.hpp
 * @author Suguru Kouchi
 */
#ifndef STOPPER_HPP
#define STOPPER_HPP

namespace ie {

class Stopper {
public:
    virtual bool doStop() = 0;
};

}

#endif
