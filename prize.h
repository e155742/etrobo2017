#include "app.h"
#include "move.hpp"
#include "util.h"
#include "SonarSensor.h"
#include "motion.hpp"

#define dist 3 //懸賞前で止まるための距離
#define dist2 35 //電車を避ける距離

using namespace ev3api;

class prize {
 public:
  void PrizeCourse();
};
