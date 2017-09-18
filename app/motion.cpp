/**
 * @file motion.cpp
 * @brief 移動したり止まったり
 *
 * @author Suguru Kouchi
 */

#include "motion.hpp"
#include "onoff_control.hpp"
#include "robo_meta_datas.hpp"

namespace ie {

Motion::Motion():
leftWheel_(LEFT_WHEEL_PORT), rightWheel_(RIGHT_WHEEL_PORT), steering_(leftWheel_, rightWheel_),
tail_(TAIL_MOTOR_PORT), arm_(ARM_MOTOR_PORT), colorSensor_(COLOR_SENSOR_PORT) {
    leftWheel_.reset();
    rightWheel_.reset();
    tail_.reset();
    arm_.reset();
}

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
 * 指定したパワーで左ホイールを動かす。
 *
 * @param pwm モーターのパワー
 */
void Motion::setPwmLeft(int pwm){
    leftWheel_.setPWM(pwm);
}

/**
 * 指定したパワーで左ホイールを動かす。
 *
 * @param stopper 停止判定用のStopperクラス
 * @param pwm     モーターのパワー
 */
void Motion::setPwmLeft(Stopper& stopper, int pwm){
    while (!stopper.doStop()) {
        setPwmLeft(pwm);
    }
    leftWheel_.stop();
}

/**
 * 指定したパワーで右ホイールを動かす。
 *
 * @param pwm モーターのパワー
 */
void Motion::setPwmRight(int pwm){
    rightWheel_.setPWM(pwm);
}

/**
 * 指定したパワーで右ホイールを動かす。
 *
 * @param stopper 停止判定用のStopperクラス
 * @param pwm     モーターのパワー
 */
void Motion::setPwmRight(Stopper& stopper, int pwm){
    while (!stopper.doStop()) {
        setPwmRight(pwm);
    }
    rightWheel_.stop();
}

/**
 * 指定したパワーで左ホイールと右ホイールを動かす。<br>
 * 制御は行わないので同じ値を入れても直進しません。
 *
 * @param leftPwm  左モーターのパワー
 * @param rightPwm 右モーターのパワー
 */
void Motion::setPwmBoth(int leftPwm, int lightPwm) {
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
void Motion::setPwmBoth(Stopper& stopper, int leftPwm, int lightPwm) {
    while (!stopper.doStop()) {
        setPwmBoth(leftPwm, lightPwm);
    }
    stop();
}

/**
 * ステアリング操作を行う<br>
 * Steering.setPower()を呼び出している。
 *
 * @param pwm       モーターのパワー
 * @param turnRatio ステアリングの度合い。負で左転回、正で右転回
 */
void Motion::setSteeringPower(int pwm, int turnRatio){
    steering_.setPower (pwm, turnRatio);
}

/**
 * ステアリング操作を行う<br>
 * Steering.setPower()を呼び出している。
 *
 * @param stopper   停止判定用のStopperクラス
 * @param pwm       モーターのパワー
 * @param turnRatio ステアリングの度合い。負で左転回、正で右転回
 */
 void Motion::setSteeringPower(Stopper& stopper, int pwm, int turnRatio){
    while (!stopper.doStop()) {
        setSteeringPower(pwm, turnRatio);
    }
    stop();
}

void Motion::onoffSetPwm(Control& control, int pwm) {
    if (OnOffControl* onoff = dynamic_cast<OnOffControl*>(&control)) {
        onoff->setPwm(pwm);
    }
}

void Motion::goStraightHelper(Control& control, int pwm) {
    float countDiff = leftWheel_.getCount() - rightWheel_.getCount();
    int controlValue = static_cast<int>(std::roundf(control.getControlValue(countDiff)));
    if (pwm < 0) { controlValue *= -1; }
    steering_.setPower(pwm, controlValue);
}

/**
 * 指定したパワーで直進する。距離指定なし。パワーがマイナスだと後退。<br>
 * Controlクラスのtargetは自動で指定される。(左ホイールのcount-右ホイールのcount)
 *
 * @param control 直進制御用のControlクラス
 * @param pwm     モーターのパワー
 */
void Motion::goStraight(Control& control, int pwm) {
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
    stop();
}

void Motion::spinHelper(Control& control, int pwm) {
    // ターゲット値よりも大きければ右寄り
    double countDiff = leftWheel_.getCount() + rightWheel_.getCount();
    int controlValue = static_cast<int>(std::roundf(control.getControlValue(countDiff) / 2.0));
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
void Motion::spin(Control& control, int pwm) {
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
    onoffSetPwm(control, pwm);
    control.setTarget(leftWheel_.getCount() + rightWheel_.getCount());
    while (!stopper.doStop()) {
        spinHelper(control, pwm);
    }
    stop();
}

}