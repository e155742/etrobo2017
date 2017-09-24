/**
 * @file motion.cpp
 * @brief 移動したり止まったり
 *
 * @author Suguru Kouchi
 */
#include "motion.hpp"
#include "onoff_control.hpp"
#include "direction_stopper.hpp"
#include "robo_meta_datas.hpp"
#include "util.h"

#include "mileage_stopper.hpp"

#define ISHIZUKA_LINETRACE // KaitoIshizukaによるライントレース実装

namespace ie {

Motion::Motion():
leftWheel_(LEFT_WHEEL_PORT), rightWheel_(RIGHT_WHEEL_PORT),
tail_(TAIL_MOTOR_PORT), arm_(ARM_MOTOR_PORT), colorSensor_(COLOR_SENSOR_PORT)
#ifdef OUTPUT_LINETRACE
, fo_("PID_control_value.txt")
#endif
{
    leftWheel_.reset();
    rightWheel_.reset();
    tail_.reset();
    arm_.reset();
}

#ifdef OUTPUT_LINETRACE
Motion::~Motion() {
    close();
}

void Motion::close() {
    fo_.close();
}
#endif

/**
 * 尻尾を指定した角度だけ回転させる。<br>
 * 正の角度で後ろから見て時計回りにまわり、負の値で反時計回りになります。<br>
 * パワーが負の値でも反転します。
 *
 * @param degree 回転したい角度
 * @param pwm    モーターのパワー
 */
 void Motion::rotateTail(int degree, int pwm) {
    tail_.reset();
    plusMinusNormalize(degree, pwm);
    tail_.setPWM(pwm);
    while (std::abs(tail_.getCount() < degree)) {}
    tail_.stop();
}

/**
 * アームを指定した角度まで動かす。相対角度ではなく絶対角度です。<br>
 * モーターの出力は0以上。腕の範囲は0~90度まで。低いほうが0度で高いほうが90度。<br>
 * appを実行した時点の高さが基準の0度になります。手で限界まで下げた状態で実行するのがオススメです。
 *
 * @param degree 回転したい角度(0~90までの絶対角度)
 * @param pwm    モーターのパワー(0以上)
 */
void Motion::raiseArm(int degree, int pwm) {
    if (degree < 0) {
        degree = 0;
    } else if (90 < degree) {
        degree = 90;
    }
    if (pwm < 0) { pwm = 0; }

    // 上げる
    if (arm_.getCount() < degree) {
        arm_.setPWM(pwm);
        while (arm_.getCount() < degree) {}
    }
    // 下げる
    if (degree < arm_.getCount()) {
        arm_.setPWM(-pwm);
        while (degree < arm_.getCount()) {}
    }

    arm_.stop();
    return;
}

/**
 * 両方の車輪を停止させる。
 */
void Motion::stop() {
    leftWheel_.stop();
    rightWheel_.stop();
}

/**
 * 両方の車輪を指定時間停止させる。
 *
 * @param time 停止する時間(ms)
 */
 void Motion::wait(uint32_t time) {
    leftWheel_.stop();
    rightWheel_.stop();
    dly_tsk(time);
}

/**
 * 指定したパワーで左ホイールを動かす。
 *
 * @param pwm モーターのパワー
 */
inline void Motion::setLeftPwm(int pwm){
    leftWheel_.setPWM(pwm);
    rightWheel_.stop();
}

/**
 * 指定したパワーで左ホイールを動かす。
 *
 * @param stopper 停止判定用のStopperクラス
 * @param pwm     モーターのパワー
 */
void Motion::setLeftPwm(Stopper& stopper, int pwm){
    while (!stopper.doStop()) {
        setLeftPwm(pwm);
    }
}

/**
 * 指定したパワーで右ホイールを動かす。
 *
 * @param pwm モーターのパワー
 */
inline void Motion::setRightPwm(int pwm){
    leftWheel_.stop();
    rightWheel_.setPWM(pwm);
}

/**
 * 指定したパワーで右ホイールを動かす。
 *
 * @param stopper 停止判定用のStopperクラス
 * @param pwm     モーターのパワー
 */
void Motion::setRightPwm(Stopper& stopper, int pwm){
    while (!stopper.doStop()) {
        setRightPwm(pwm);
    }
}

/**
 * 指定したパワーで左ホイールと右ホイールを動かす。<br>
 * 制御は行わないので同じ値を入れても直進しません。
 *
 * @param leftPwm  左モーターのパワー
 * @param rightPwm 右モーターのパワー
 */
inline void Motion::setBothPwm(int leftPwm, int lightPwm) {
    leftWheel_.setPWM(leftPwm);
    rightWheel_.setPWM(lightPwm);
}

/**
 * 指定したパワーで左ホイールと右ホイールを動かす。<br>
 * 制御は行わないので同じ値を入れても直進しません。
 *
 * @param stopper  停止判定用のStopperクラス
 * @param leftPwm  左モーターのパワー
 * @param rightPwm 右モーターのパワー
 */
void Motion::setBothPwm(Stopper& stopper, int leftPwm, int lightPwm) {
    while (!stopper.doStop()) {
        setBothPwm(leftPwm, lightPwm);
    }
}

inline int Motion::setSteeringLeftPower(int pwm, double turnRatio) {
    if (0 < turnRatio) { return pwm; }

    if (turnRatio < -100) { turnRatio = -100; }
    return roundInt(pwm * ((100.0 + turnRatio) / 100.0));
}

inline int Motion::setSteeringRightPower(int pwm, double turnRatio) {
    if (turnRatio < 0) { return pwm; }

    if (100 < turnRatio) { turnRatio = 100; }
    return roundInt(pwm * ((100.0 - turnRatio) / 100.0));
}

/**
 * ステアリング操作を行う<br>
 * Steering.setPower()を使わない独自実装
 *
 * @param pwm       モーターのパワー
 * @param turnRatio ステアリングの度合い。負で左転回、正で右転回
 */
inline void Motion::setSteeringPower(int pwm, int turnRatio){
    int leftPwm = setSteeringLeftPower(pwm, turnRatio);
    int rightPwm = setSteeringRightPower(pwm, turnRatio);
    setBothPwm(leftPwm, rightPwm);
}

/**
 * ステアリング操作を行う<br>
 * Steering.setPower()を使わない独自実装
 *
 * @param stopper   停止判定用のStopperクラス
 * @param pwm       モーターのパワー
 * @param turnRatio ステアリングの度合い。負で左転回、正で右転回
 */
 void Motion::setSteeringPower(Stopper& stopper, int pwm, int turnRatio){
    int leftPwm = setSteeringLeftPower(pwm, turnRatio);
    int rightPwm = setSteeringRightPower(pwm, turnRatio);
    while (!stopper.doStop()) {
        setBothPwm(leftPwm, rightPwm);
    }
}

inline void Motion::onoffSetPwm(Control& control, int pwm) {
    if (OnOffControl* onoff = dynamic_cast<OnOffControl*>(&control)) {
        onoff->setPwm(pwm);
    }
}

inline void Motion::goStraightHelper(Control& control, int pwm) {
    float countDiff = leftWheel_.getCount() - rightWheel_.getCount();
    double controlValue = control.getControlValue(countDiff);
    if (0 < pwm) { controlValue *= -1; }
    setSteeringPower(pwm, controlValue);
}

/**
 * 指定したパワーで直進する。距離指定なし。パワーがマイナスだと後退。<br>
 * Controlクラスのtargetは自動で指定される。(左ホイールのcount-右ホイールのcount)
 *
 * @param control 直進制御用のControlクラス
 * @param pwm     モーターのパワー
 */
inline void Motion::goStraight(Control& control, int pwm) {
    onoffSetPwm(control, pwm);
    control.setTarget(leftWheel_.getCount() - rightWheel_.getCount());
    goStraightHelper(control, pwm);
}

/**
 * 指定したパワーで直進する。距離指定なし。パワーがマイナスだと後退。<br>
 * Controlクラスのtargetは自動で指定される。(左ホイールのcount-右ホイールのcount)
 *
 * @param stopper 停止判定用のStopperクラス
 * @param control 直進制御用のControlクラス
 * @param pwm     モーターのパワー
 */
void Motion::goStraight(Stopper& stopper, Control& control, int pwm) {
    onoffSetPwm(control, pwm);
    control.setTarget(leftWheel_.getCount() - rightWheel_.getCount());
    while (!stopper.doStop()) {
        goStraightHelper(control, pwm);
    }
}

inline void Motion::spinHelper(Control& control, int pwm) {
    // ターゲット値よりも大きければ右寄り
    double countDiff = leftWheel_.getCount() + rightWheel_.getCount();
    int controlValue = roundInt(control.getControlValue(countDiff) / 2.0);
    leftWheel_.setPWM(pwm - controlValue);
    rightWheel_.setPWM(-(pwm + controlValue));
}

/**
 * 正のpwmで時計回り、負のpwmで反時計回りに回転する。<br>
 * Controlクラスのtargetは自動で指定される。(左ホイールのcount+右ホイールのcount)
 *
 * @param control 回転制御用のControlクラス
 * @param pwm     モーターのパワー
 */
inline void Motion::spin(Control& control, int pwm) {
    onoffSetPwm(control, pwm);
    control.setTarget(leftWheel_.getCount() + rightWheel_.getCount());
    spinHelper(control, pwm);
}

/**
 * 正のpwmで時計回り、負のpwmで反時計回りに回転する。<br>
 * Controlクラスのtargetは自動で指定される。(左ホイールのcount+右ホイールのcount)
 *
 * @param stopper 停止判定用のStopperクラス
 * @param control 回転制御用のControlクラス
 * @param pwm     モーターのパワー
 */
void Motion::spin(Stopper& stopper, Control& control, int pwm) {
    // 方位停止の場合、pwmの正負を無視して近い方を回転方向とする。
    if (DirectionStopper* ds = dynamic_cast<DirectionStopper*>(&stopper)) {
        point_t diffDirection = ds->getTargetDirection() - ds->getDirection();
        if (0 < diffDirection) {
            pwm = std::abs(pwm);
        } else {
            pwm = std::abs(pwm) * -1;
        }
    }
    onoffSetPwm(control, pwm);
    control.setTarget(leftWheel_.getCount() + rightWheel_.getCount());
    while (!stopper.doStop()) {
        spinHelper(control, pwm);
    }
}

/**
 * 指定の座標に移動する。Steeringクラスを使用。<br>
 * Controlクラスのtargetは自動で指定される。(0)<br>
 * pwmが大きいとmarginも大きくする必要がある。
 *
 * @param l Localizationクラス
 * @param control Steering制御用のControlクラス
 * @param pwm モーターのパワー
 * @param pointX 目標となるX座標(mm)
 * @param pointY 目標となるY座標(mm)
 * @param margin 目標からどれだけ手前で止まるか(mm)
 */
void Motion::goPoint(Localization& l, Control& control, int pwm, point_t pointX, point_t pointY, int margen) {
    onoffSetPwm(control, pwm);
    control.setTarget(0);
    while(margen <= std::sqrt(std::pow(pointX - l.getPointX(), 2) + std::pow(pointY - l.getPointY(), 2))) {
        // 正だと左旋回
        point_t diffRadian = std::atan2(pointX - l.getPointX(), pointY - l.getPointY())
                   - l.getDirection();
        double controlValue = control.getControlValue(radianNormalize(diffRadian));
        setSteeringPower(pwm, controlValue);
    }
}

inline void Motion::lineTraceHelper(Control& control, int pwm, bool isRightSide) {
    colorSensor_.getRawColor(rgb_);
    float value = static_cast<float>(rgb_.r + rgb_.g + rgb_.b);
    double controlValue = control.getControlValue(value);

    if (!isRightSide) { controlValue *= -1;}
    if (controlValue < -pwm) {controlValue = -pwm;}
    if (pwm < controlValue) {controlValue = pwm;}

    #ifdef ISHIZUKA_LINETRACE
    // KaitoIshizukaによる謎実装
    controlValue *= 0.02; // 謎の値
	int pwm_L = roundInt(pwm * (1.0 - controlValue));
	int pwm_R = roundInt(pwm * (1.0 + controlValue));
    setBothPwm(pwm_L, pwm_R);
    #else
    // 一般的なの実装
    if (controlValue < 0) {
        // 右旋回
        setBothPwm(pwm, pwm + controlValue);
    } else {
        // 左旋回
        setBothPwm(pwm - controlValue, pwm);
    }
    #endif

    #ifdef OUTPUT_LINETRACE
    fo_.fileWrite(controlValue);
    #endif
}

/**
 * 指定したパワーでライントレースする。<br>
 *
 * @param control     ライントレース制御用のControlクラス
 * @param pwm         モーターのパワー
 * @param isRightSide ラインの右側走るならtrue、左を走るならfalse
 */
inline void Motion::lineTrace(Control& control, int pwm, bool isRightSide) {
    onoffSetPwm(control, pwm);
    lineTraceHelper(control, pwm, isRightSide);
}

/**
 * 指定したパワーでライントレースする。<br>
 *
 * @param stopper 停止判定用のStopperクラス
 * @param control     ライントレース制御用のControlクラス
 * @param pwm         モーターのパワー
 * @param isRightSide ラインの右側走るならtrue、左を走るならfalse
 */
void Motion::lineTrace(Stopper& stopper, Control& control, int pwm, bool isRightSide) {
    onoffSetPwm(control, pwm);
    while (!stopper.doStop()) {
        lineTraceHelper(control, pwm, isRightSide);
    }
}

}
