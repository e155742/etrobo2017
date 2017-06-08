/**
 * @file var_entering.hpp
 * @author Suguru Kouchi
 */
#ifndef VAR_ENTERING_HPP
#define VAR_ENTERING_HPP

#include <vector>
#include <Clock.h>

namespace ie {

class VarEntering {
public:
    void inputInt(int& num);
    void inputInt(int& num, const char* title);
    void inputInt(std::vector<int>& nums);
    void inputInt(std::vector<int>& nums, const char* title);
    void inputFloat(float& num);
    void inputFloat(float& num, const char* title);
    void inputFloat(std::vector<float>& nums);
    void inputFloat(std::vector<float>& nums, const char* title);
    void setAccuracy(int accuracy);

private:
    const uint32_t DELAY_TIME = 200; // 長押しにならないように停止させておく
    int accuracy_ = 100;             // float型の精度 1/accuracy_ になる。 100とか1000とか10のべき乗倍にしろ
    int32_t titleLine_ = 1;          // タイトルの行
    int32_t firstLine_ = titleLine_; // 数値の行
    int vectorIndex_ = 0;
    bool doContinue_ = true;
    void pushButton(int& num);
    void pushButtonFloat(int& numInt);
    void pushButton(std::vector<int>& nums, int maxIndex, int32_t selectLineStr);
    void pushButtonFloat(std::vector<int>& numInts, int maxIndex, int32_t selectLineStr);
};

}

#endif
