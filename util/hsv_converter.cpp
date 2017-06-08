/**
 * @file hsv_converter.cpp
 * @brief RGBをHSVに変換
 *
 * @see http://www.peko-step.com/tool/hsvrgb.html
 * @author Suguru Kouchi
 */
#include "hsv_converter.hpp"

namespace ie {

/**
 * 引数なしで初期化するとRGB(0,0,0)をHSVに変換する。要は黒。
 */
HsvConverter::HsvConverter() {
    convertToHsv(0, 0, 0);
}

/**
 * RGBを指定してHSVに変換。
 *
 * @param red   RGBの赤
 * @param green RGBの緑
 * @param blue  RGBの青
 */
HsvConverter::HsvConverter(rgb_t red, rgb_t green, rgb_t blue) {
    convertToHsv(red, green, blue);
}

/**
 * RGBの各値から最大値と最小値を求める。<br>
 * 色相を求める際にどの色が最大値か知っている必要があるのでこれも求める。
 *
 * @rapam red   RGBの赤
 * @rapam green RGBの緑
 * @rapam blue  RGBの青
 * @rapam max   RGBの各値の中から最大値を格納する変数
 * @rapam min   RGBの各値の中から最小値を格納する変数
 * @return      最大値だった色 0:赤 1:緑 2:青
 */
int HsvConverter::searchMaxAndMin(rgb_t red, rgb_t green, rgb_t blue, rgb_t& max, rgb_t& min) const {
    int maxIndex = 0;
    rgb_t rgb[3] = {red, green, blue};
    max = rgb[0];
    min = rgb[0];
    for (int i = 1; i < 3; i++) {
        if (max < rgb[i % 3]) {
            max = rgb[i % 3];
            maxIndex = i;
        } else if (rgb[i % 3] < min) {
            min = rgb[i % 3];
        }
    }
    return maxIndex;
}

/**
 * RGB値をHSV値に変換する。HSVの値はメンバ変数のhue_(色相)、saturation_(彩度)、lightness_(明度)に代入される。
 *
 * @param red   変換したいRGB値の赤
 * @param green 変換したいRGB値の緑
 * @param blue  変換したいRGB値の青
 */
void HsvConverter::convertToHsv(rgb_t red, rgb_t green, rgb_t blue) {
    rgb_t max;
    rgb_t min;
    int maxIndex = searchMaxAndMin(red, green, blue, max, min);
    hsv_t rgb[3] = {(hsv_t)red, (hsv_t)green, (hsv_t)blue};
    hue_ = (red == green && green == blue)
            ? 0
            : 60.0 * ((rgb[(maxIndex + 1) % 3] - rgb[(maxIndex + 2) % 3]) / (max - min)) + (120 * maxIndex);
    if (hue_ < 0) {
        hue_ += 360;
    }
    saturation_ = max == 0
            ? 0                          // ゼロ除算しないため
            : 255.0 * (max - min) / max;
    lightness_ = max;
}

/**
 * HSV値を一気に取得。
 *
 * @param hue        色相を格納する変数
 * @param saturation 彩度を格納する変数
 * @param lightness  明度を格納する変数
 */
void HsvConverter::getHsv(hsv_t& hue, hsv_t& saturation, hsv_t& lightness) const {
    hue = hue_;
    saturation = saturation_;
    lightness = lightness_;
}

/**
 * 色相を取得
 *
 * @return 色相
 */
hsv_t HsvConverter::getHue() const {
    return hue_;
}

/**
 * 彩度を取得
 *
 * @return 彩度
 */
hsv_t HsvConverter::getSaturation() const {
    return saturation_;
}

/**
 * 明度を取得
 *
 * @return 明度
 */
hsv_t HsvConverter::getLightness() const {
    return lightness_;
}

}