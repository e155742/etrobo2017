#include "util.h"
#include <cstdio>
#include <cmath>
#include "var_entering.hpp"

// 演習用のユーティリティ

// *******************
// **    画面入力    **
// *******************
void inputInt(int& var) {
  ie::VarEntering varEntering;
  varEntering.inputInt(var);
}

void inputInt(int& var, const char *title) {
  ie::VarEntering varEntering;
  varEntering.inputInt(var, title);
}

void inputFloat(float& var) {
  ie::VarEntering varEntering;
  varEntering.inputFloat(var);
}

void inputFloat(float& var, const char *title) {
  ie::VarEntering varEntering;
  varEntering.inputFloat(var, title);
}

void inputFloat(float& var, int accuracy) {
  ie::VarEntering varEntering;
  varEntering.setAccuracy(accuracy);
  varEntering.inputFloat(var);
}

void inputFloat(float& var, int accuracy, const char *title) {
  ie::VarEntering varEntering;
  varEntering.setAccuracy(accuracy);
  varEntering.inputFloat(var, title);
}

void inputInt(std::vector<int>& vars) {
  ie::VarEntering varEntering;
  varEntering.inputInt(vars);
}

void inputInt(std::vector<int>& vars, const char *title) {
  ie::VarEntering varEntering;
  varEntering.inputInt(vars, title);
}

void inputFloat(std::vector<float>& vars) {
  ie::VarEntering varEntering;
  varEntering.inputFloat(vars);
}

void inputFloat(std::vector<float>& vars, const char *title) {
  ie::VarEntering varEntering;
  varEntering.inputFloat(vars, title);
}

void inputFloat(std::vector<float>& vars, int accuracy) {
  ie::VarEntering varEntering;
  varEntering.setAccuracy(accuracy);
  varEntering.inputFloat(vars);
}

void inputFloat(std::vector<float>& vars, int accuracy, const char *title) {
  ie::VarEntering varEntering;
  varEntering.setAccuracy(accuracy);
  varEntering.inputFloat(vars, title);
}

// *******************
// **    画面出力    **
// *******************

// 初期処理用
void init_f(const char *str) {
  // フォントの設定と0行目の表示
  ev3_lcd_set_font(EV3_FONT_MEDIUM);
  ev3_lcd_draw_string(str, 0, 0);
}

/**
 * 行単位で引数の文字列を表示
 * @param str 表示する文字列
 * @param line 10ドットごとの行番号（0から12）
 */
void msg_f(const char *str, int32_t line) {
  const int8_t line_height = 10;
  ev3_lcd_fill_rect(0, line * line_height, EV3_LCD_WIDTH, line_height, EV3_LCD_WHITE);
  ev3_lcd_draw_string(str, 0, line * line_height);
}

/**
 * 画面上の表示を全て消去
 */
void msg_clear() {
  for (int i = 0; i < 14; i++) {
    msg_f("", i);
  }
}

void msg_f(int8_t n, int32_t line) {
  msg_template(n, line, "%d");
}

void msg_f(uint8_t n, int32_t line) {
  msg_template(n, line, "%d");
}

void msg_f(char n, int32_t line) {
  msg_template(n, line, "%d");
}

void msg_f(int16_t n, int32_t line) {
  msg_template(n, line, "%d");
}

void msg_f(uint16_t n, int32_t line) {
  msg_template(n, line, "%d");
}

void msg_f(int n, int32_t line) {
  msg_template(n, line, "%d");
}

void msg_f(unsigned int n, int32_t line) {
  msg_template(n, line, "%d");
}

void msg_f(int32_t n, int32_t line) {
  msg_template(n, line, "%ld");
}

void msg_f(uint32_t n, int32_t line) {
  msg_template(n, line, "%ld");
}

void msg_f(int64_t n, int32_t line) {
  msg_template(n, line, "%lld");
}

void msg_f(uint64_t n, int32_t line) {
  msg_template(n, line, "%lld");
}

void msg_f(float n, int32_t line) {
  msg_template(n, line, "%f");
}

void msg_f(double n, int32_t line) {
  msg_template(n, line, "%lf");
}

// *******************
// **   ビープ音   d **
// *******************
void soundBeep() {
    ev3_speaker_set_volume(100);
    ev3_speaker_play_tone(NOTE_B5, 40);
}