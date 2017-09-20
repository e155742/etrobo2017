/**
 * @file direction_stoper.hpp
 * @author Suguru Kouchi
 */
#ifndef DIRECTION_STOPPER_HPP
#define DIRECTION_STOPPER_HPP

#include "stopper.hpp"
#include "localization.hpp"
#include "robo_meta_datas.hpp"

namespace ie {

class DirectionStopper : public Stopper {
public:
    DirectionStopper(Localization& localization);
    DirectionStopper(Localization& localization, point_t targetDirection);
    bool doStop();
    void setTargetDirection(point_t targetDirection);
    point_t getTargetDirection() const;
    point_t getDirection() const;

private:
    Localization& localization_;
    point_t targetDirection_;
    point_t lastDirection_ ;

};

}

#endif
