#ifndef MY_UNTL_H_
#define MY_UNTL_H_

#include "ev3api.h"
#include <cmath>
#include <vector>

extern void inputInt(int& var);
extern void inputInt(int& var, const char *str);
extern void inputFloat(float& var);
extern void inputFloat(float& var, const char *str);
extern void inputFloat(float& var, int accuracy);
extern void inputFloat(float& var, int accuracy, const char *str);

extern void inputInt(std::vector<int>& vars);
extern void inputInt(std::vector<int>& vars, const char *str);
extern void inputFloat(std::vector<float>& vars);
extern void inputFloat(std::vector<float>& vars, const char *str);
extern void inputFloat(std::vector<float>& vars, int accuracy);
extern void inputFloat(std::vector<float>& vars, int accuracy, const char *str);

extern void init_f(const char *str);
extern void msg_f(const char *str, int32_t line);
extern void msg_clear();

extern void msg_f(int8_t n, int32_t line);
extern void msg_f(uint8_t n, int32_t line);
extern void msg_f(char n, int32_t line);

extern void msg_f(int16_t n, int32_t line);
extern void msg_f(uint16_t n, int32_t line);

extern void msg_f(int n, int32_t line);
extern void msg_f(unsigned int n, int32_t line);

extern void msg_f(int32_t n, int32_t line);
extern void msg_f(uint32_t n, int32_t line);

extern void msg_f(int64_t n, int32_t line);
extern void msg_f(uint64_t n, int32_t line);

extern void msg_f(float n, int32_t line);
extern void msg_f(double n, int32_t line);

extern void soundBeep();

template <class X, class Y, class Z>
void msg_template(X n, Y line, Z format) {
  char str[64];
  sprintf(str, format, n);
  msg_f(str, line);
}

inline static int roundInt(float n) {
  return static_cast<int>(std::roundf(n));
}

inline static int roundInt(double n) {
  return static_cast<int>(std::round(n));
}

inline static int32_t roundInt32_t(float n) {
  return static_cast<int32_t>(std::roundf(n));
}

inline static int32_t roundInt32_t(double n) {
  return static_cast<int32_t>(std::round(n));
}

// *******************
// **      計算      **
// *******************

/**
 * 角度を -PI < radian <= PI に正規化
 *
 * @param   radian 正規化したい角度
 * @preturn radian 正規化された角度
 */
inline static double radianNormalize(double radian) {
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
 * valueが負の場合、これを正にしてsubValueの正負を反転させます<br>
 * pwmや距離、角度等を正規化する際に使用します<br>
 * 符号付きの整数型まはた浮動小数型で使うこと。
 *
 * @param value    こいつを正にする
 * @param subValue valueが負ならこいつの符号が逆転する
 */
template <class X, class Y>
inline extern void plusMinusNormalize(X& value, Y& subValue) {
  if (value < 0) {
    value *= -1;
    subValue *= -1;
  }
}

/**
 * 度数法を弧度法(ラジアン)に変換
 *
 * @param degree 度数法での角度
 * @return       弧度法での角度
 */
inline extern double degToRad(double degree) {
  return degree * (M_PI / 180.0);
}

/**
 * 弧度法(ラジアン)を度数法に変換
 *
 * @param radian 弧度法での角度
 * @return       度数法での角度
 */
inline extern double RadToDeg(double radian) {
  return radian * (180.0 / M_PI);
}

/**
 * 極座標から直交座標のX座標を求める
 *
 * @param r     中心と座標の距離
 * @param theta 中心と座標の偏角
 * @return      X座標
 */
inline extern double polarToRectX(double r, double theta) {
  return r * std::cos(theta);
}

/**
 * 極座標から直交座標のY座標を求める
 *
 * @param r     中心と座標の距離
 * @param theta 中心と座標の偏角
 * @return      Y座標
 */
inline extern double polarToRectY(double r, double theta) {
  return r * std::sin(theta);
}

#endif  // MY_UNTL_H_
