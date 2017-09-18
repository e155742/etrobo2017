#ifndef MY_UNTL_H_
#define MY_UNTL_H_

#include "ev3api.h"
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

template <class X, class Y, class Z>
void msg_template(X n, Y line, Z format) {
  char str[64];
  sprintf(str, format, n);
  msg_f(str, line);
}

double radianNormalize(double radian);

/**
 * valueが負の場合、これを正にしてsubValueの正負を反転させます<br>
 * pwmや距離、角度等を正規化する際に使用します<br>
 * 符号付きの整数型まはた浮動小数型で使うこと。
 *
 * @param value    こいつを正にする
 * @param subValue valueが負ならこいつの符号が逆転する
 */
template <class X, class Y>
void plusMinusNormalize(X& value, Y& subValue) {
  if (value < 0) {
    value *= -1;
    subValue *= -1;
  }
}

#endif  // MY_UNTL_H_
