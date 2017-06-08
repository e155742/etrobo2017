/**
 * @file hsv_converter.hpp
 * @author Suguru Kouchi
 */
#ifndef HSV_CONVERTER_HPP
#define HSV_CONVERTER_HPP

namespace ie {

// 下のエイリアスは少なくともどちらか一方を浮動小数点数型にすること
// 両方整数型だと色相が計算できない
using rgb_t = int;   // RGB値で使用する型
using hsv_t = float; // HSV値で使用する型

class HsvConverter{
public:
    HsvConverter();
    HsvConverter(rgb_t red, rgb_t green, rgb_t blue);
    void convertToHsv(rgb_t red, rgb_t green, rgb_t blue);
    void getHsv(hsv_t& hue, hsv_t& saturation, hsv_t& lightness) const;
    hsv_t getHue() const;
    hsv_t getSaturation() const;
    hsv_t getLightness() const;

private:
    hsv_t hue_;
    hsv_t saturation_;
    hsv_t lightness_;  // 本当はvalueにしたいが他と被りそうなので
    int searchMaxAndMin(rgb_t red, rgb_t green, rgb_t blue, rgb_t& max, rgb_t& min) const;
};

}

#endif
