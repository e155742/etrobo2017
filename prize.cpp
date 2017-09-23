#include "prize.h"
#include "Clock.h"
#include "Motor.h"

#include "control.hpp"
#include "pid_control.hpp"
#include "onoff_control.hpp"

//変数の初期設定スペース

ev3api::Motor Left(ie::LEFT_WHEEL_PORT);
ev3api::Motor Right(ie::RIGHT_WHEEL_PORT);

#define dist 3 //懸賞前で止まるための距離
#define dist2 35 //電車を避ける距離


void prize::PrizeCourse() {
  
  //センサー等の初期設定
  SonarSensor sonarSensor(PORT_2);
  Clock clock;

  int16_t distance = sonarSensor.getDistance(); //距離格納用変数
  int roop = 0;
  int flag = 0;

  Motor leftWheel(PORT_C);
  Motor rightWheel(PORT_B);
  ie::Move move;
  ie::Motion motion;
  ie::OnOffControl* stControl = new ie::OnOffControl(0,0,0.3,0);
  ie::PIDControl* stPid = new ie::PIDControl(Left.getCount() - Right.getCount(), 2, 0, 0);

  move.goStraight(*stControl, 10 * 15, 20);
  move.spin(*stControl, -90, 100/6);

  leftWheel.setPWM(20);
  rightWheel.setPWM(20);


  //----懸賞に向かって直進
  while(1){
    distance = sonarSensor.getDistance();  //距離を格納している
    if(distance == dist && distance != 0){
      move.stop();
      break;
    }
  }
  
  //---懸賞getし後退.そして回転
  move.raiseArm(75, 5);  //懸賞を持つ
  move.goStraight(*stControl, 10 * -20, 20); //(int 距離, int PWM) ※距離は単位がミリなので10かける
  move.spin(*stControl, -90, 100/6);

  //---電車が通過するのを確認するまで待つ
  while(1){
    distance = sonarSensor.getDistance();
    if(distance <= dist2 && distance != 0){
      break;
    }
  }

  //---回転して交代のままレールの上を通過．懸賞置き場へ置く
  move.spin(*stControl, -180, 100/6);
  move.goStraight(*stControl, 10 * -50, 20);
  move.spin(*stControl, -90, 100/6);
  move.goStraight(*stControl, 10 * 10, 10);
  move.raiseArm(7, 10);
  move.goStraight(*stControl, 10 * -15, 10);

  //バックで駐車
  move.spin(*stControl, 90, 10);
  move.goStraight(*stControl, 10 * -35, 10);
  move.spin(*stControl, 90, 10);
  move.goStraight(*stControl, 10 * -40, 10);
}
