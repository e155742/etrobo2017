/**
 * @file move.cpp
 * @brief 移動したり止まったり
 *
 * @author Suguru Kouchi
 */
#include "move.hpp"
#include "util.h"
#include "onoff_control.hpp"
#include "robo_meta_datas.hpp"

namespace ie {

/**
 * 各Motorクラスのインスタンスをreset()してます。
 */
Move::Move():
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
void Move::rotateTail(int degree, int pwm) {
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
void Move::raiseArm(int degree, int pwm) {
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
 * 止まる
 */
void Move::stop() {
    leftWheel_.stop();
    rightWheel_.stop();
}

/**
 * 指定したパワーで左ホイールを動かす
 *
 * @param pwm モーターのパワー
 */
void Move::left(int pwm){
    leftWheel_.setPWM(pwm);
}

/**
 * 指定したパワーで右ホイールを動かす
 *
 * @param pwm モーターのパワー
 */
void Move::right(int pwm){
    rightWheel_.setPWM(pwm);
}

/**
 * ステアリング操作を行う<br>
 * Steering.setPower()を呼び出している。
 *
 * @param pwm モーターのパワー
 * @param turnRatio ステアリングの度合い。マイナスの値は左への転回、プラスの値は右への転回
 */
void Move::steering(int power, int turnRatio){
    steering_.setPower (power, turnRatio);
}

/**
 * 指定したパワーで直進する。距離指定あり。パワーがマイナスだと後退。<br>
 * パワーの範囲は-100~100だが、これ未満の値や超過する値入れても自動でカンストするから平気。<br>
 * Controlクラスのtargetは0を指定すること。
 *
 * @param control  直進制御用のControlクラス
 * @param distance 走行する距離(mm)
 * @param pwm      モーターのパワー
 */
void Move::goStraight(Control& control, float distance, int pwm) {
    int beginLeftCount  = leftWheel_.getCount();
    int beginRightCount = rightWheel_.getCount();
    float beginDistance = getMileage();
    plusMinusNormalize(distance, pwm);
    if (OnOffControl* onoff = dynamic_cast<OnOffControl*>(&control)) {
        onoff->setPwm(pwm);
    }

    while(std::abs(beginDistance - getMileage()) < distance) {
        // 左ホイールの回転角 - 右ホイールの回転角度  正だと左旋回
        float countDiff = std::abs(leftWheel_.getCount() - beginLeftCount)
                        - std::abs(rightWheel_.getCount() - beginRightCount);
        int controlValue = static_cast<int>(std::roundf(control.getControlValue(countDiff)));
        steering_.setPower(pwm, controlValue);
    }
    stop();
}

/**
 * 指定したパワーで直進する。距離指定なし。パワーがマイナスだと後退。<br>
 * Controlクラスのtargetは(左ホイールのcount-右ホイールのcount)を指定すること。
 *
 * @param control  直進制御用のControlクラス
 * @param pwm      モーターのパワー
 */
void Move::goStraight(Control& control, int pwm) {
    if (OnOffControl* onoff = dynamic_cast<OnOffControl*>(&control)) {
        onoff->setPwm(pwm);
    }

    float countDiff = leftWheel_.getCount() - rightWheel_.getCount();
    int controlValue = static_cast<int>(std::roundf(control.getControlValue(countDiff)));
    if (pwm < 0) { controlValue *= -1; }
    steering_.setPower(pwm, controlValue);
}

/**
 * 指定した角度だけその場で回転する。正の角度が時計回り、負の角度が反時計回り。パワーが負になっても逆になる<br>
 * パワーの範囲は-100~100だが、これ未満の値や超過する値入れても自動でカンストするから平気。<br>
 * <br>
 * 回転するホイールの角度 = (車体を回転させたい角度)*(車体が一回転する円の外周/車輪の外周)
 *
 * @param control 回転制御用のControlクラス
 * @param degree  回転したい角度
 * @param pwm     モーターのパワー
 */
void Move::spin(Control& control, int degree, int pwm) {
    int32_t beginLeftCount  = leftWheel_.getCount();
    int32_t beginRightCount = rightWheel_.getCount();
    plusMinusNormalize(degree, pwm);
    if (OnOffControl* onoff = dynamic_cast<OnOffControl*>(&control)) {
        onoff->setPwm(pwm);
    }
    double targetWheelAngle = ANGLE_COEFFICIENT * static_cast<double>(degree)
                            * (ROBOT_TREAD / TIRE_OUT_DIAMETER);

    // 左右のホイールの平均回転角度 < 目標回転角度
    while (std::abs((leftWheel_.getCount() - beginLeftCount + beginRightCount - rightWheel_.getCount()) / 2)
         < targetWheelAngle) {
        // 正だと左寄り
        float countDiff = std::abs(leftWheel_.getCount() - beginLeftCount)
                        - std::abs(rightWheel_.getCount() - beginRightCount);
        int controlValue = static_cast<int>(std::roundf(control.getControlValue(countDiff) / 2));
        if (pwm < 0) { controlValue *= -1; }

        leftWheel_.setPWM(pwm - controlValue);
        rightWheel_.setPWM(-(pwm + controlValue));
    }
    stop();
}

/**
 * 角度を -PI < radian <= PI に正規化
 *
 * @param   radian 正規化したい角度
 * @preturn radian 正規化された角度
 */
double Move::radianNormalize(double radian) {
    if (radian <= -M_PI || M_PI < radian) {
        radian = fmod(radian, 2.0 * M_PI);
        if (radian <= -M_PI) {
            radian +=  2 * M_PI;
        } else if (M_PI < radian) {
            radian -= 2 * M_PI;
        }
    }
    return radian;
}

/**
 * 指定の座標に移動
 */
void Move::goPoint(Control& control, Control& spinControl, Localization& l,
                   point_t pointX, point_t pointY, int pwm, int spinPwm) {
    // 回転させる
    point_t diffRadian = std::atan2(pointX - l.getPointX(), pointY - l.getPointY())
                       - l.getDirection();
    diffRadian = radianNormalize(diffRadian);
    int32_t beginLeftCount  = leftWheel_.getCount();
    int32_t beginRightCount = rightWheel_.getCount();
    if (OnOffControl* onoff = dynamic_cast<OnOffControl*>(&spinControl)) {
        onoff->setPwm(spinPwm);
    }

    // 右回転か左回転かを決める
    if (0 < diffRadian) {
        while (0 < diffRadian) {
            spinForGoPoint(spinControl, l, pointX, pointY, diffRadian,
                           beginLeftCount, beginRightCount, spinPwm);
        }
    } else {
        spinPwm *= -1;
        while (diffRadian < 0) {
            spinForGoPoint(spinControl, l, pointX, pointY, diffRadian,
                           beginLeftCount, beginRightCount, spinPwm);
        }
    }
    // 回転ここまで

    dly_tsk(100); // そのままだと機体が揺れて誤差が出る気がするので少し止めておく
    // 移動させる
    if (OnOffControl* onoff = dynamic_cast<OnOffControl*>(&control)) {
        onoff->setPwm(pwm);
    }

    while(30 <= std::sqrt(std::pow(pointX - l.getPointX(), 2) + std::pow(pointY - l.getPointY(), 2))) {
        // 正だと左旋回
        diffRadian = std::atan2(pointX - l.getPointX(), pointY - l.getPointY())
                   - l.getDirection();
        diffRadian = radianNormalize(diffRadian);
        int controlValue = static_cast<int>(std::roundf(control.getControlValue(diffRadian)));
        steering_.setPower(pwm, -controlValue);
    }
    stop();
    // 移動ここまで

}

void Move::spinForGoPoint(Control& control, Localization& l, point_t pointX, point_t pointY, point_t& diffRadian,
                          int32_t beginLeftCount, int32_t beginRightCount, int spinPwm) {

    float countDiff = std::abs(leftWheel_.getCount() - beginLeftCount)
                    - std::abs(rightWheel_.getCount() - beginRightCount);
    int controlValue = static_cast<int>(std::roundf(control.getControlValue(countDiff) / 2));
    if (spinPwm < 0) { controlValue *= -1; }

    leftWheel_.setPWM(spinPwm - controlValue);
    rightWheel_.setPWM(-(spinPwm + controlValue));
    diffRadian = std::atan2(pointX - l.getPointX(), pointY - l.getPointY())
               - l.getDirection();
    diffRadian = radianNormalize(diffRadian);
}

void Move::showControlData(int brightness, int controlValue) const {
    const int max = 200;
    static int a = max;
    if (max < a++) {
        msg_f("brightness", 1);
        msg_f(brightness, 2);
        msg_f("control value", 4);
        msg_f(controlValue, 5);
        a = 0;
    }
}

/**
 * 指定したパワーでライントレースする。<br>
 * Steeringクラスを使用した実装。
 *
 * @param control     ライントレース制御用のControlクラス
 * @param pwm         モーターのパワー
 * @param isRightSide ラインの右側走るならtrue、左を走るならfalse
 */
void Move::lineTrace(Control& control, int pwm, bool isRightSide) {
    if (OnOffControl* onoff = dynamic_cast<OnOffControl*>(&control)) {
        onoff->setPwm(pwm);
    }
    float value = static_cast<float>(colorSensor_.getBrightness());
    int controlValue = static_cast<int>(std::roundf(control.getControlValue(value)));
    if (!isRightSide) {
        controlValue *= -1;
    }
    steering_.setPower(pwm, controlValue);
    showControlData(value, controlValue);
}

// 従来のSteeringクラスを使用しない実装
void Move::oldLineTrace(Control& control, float threshold, int pwm, bool isRightSide) {
    if (OnOffControl* onoff = dynamic_cast<OnOffControl*>(&control)) {
        onoff->setPwm(pwm);
    }
    float value = static_cast<float>(colorSensor_.getBrightness());
    int controlValue = static_cast<int>(std::roundf(control.getControlValue(value)));

    showControlData(value, controlValue);

    if (!isRightSide) {
        controlValue *= -1;
    }
    if(value > threshold) {
        leftWheel_.setPWM(pwm - controlValue);
        rightWheel_.setPWM(pwm);
    } else {
        leftWheel_.setPWM(pwm);
        rightWheel_.setPWM(pwm + controlValue);
    }
}

/**
 * 走行距離を取得する。<br>
 * 左右のタイヤの走行距離の平均を取っている。
 *
 * @return 走行距離
 */
float Move::getMileage() const {
    return TIRE_OUT_DIAMETER * (leftWheel_.getCount() + rightWheel_.getCount()) / (360.0 * 2.0);
}

}
