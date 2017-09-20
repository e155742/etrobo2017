/**
 * @color_judge.cpp
 * @brief HSVから色を確定する
 *
 * @author Suguru Kouchi
 */
#include "color_judge.hpp"
#include "robo_meta_datas.hpp"

namespace ie {

ColorJudge::ColorJudge(HsvConverter& hsvConverter):
hsvConverter_(hsvConverter) {
}

/**
 * RGB値から色(colorid_t)を取得する。<br>
 * 赤、青、黄、緑、黒、不明の6パターン<br>
 * RGBの正規化も同時に行うので、センサーの生の値を入力すること。
 *
 * @param red   RGBの赤の値
 * @param green RGBの緑の値
 * @param blue  RGBの青の値
 * @return 判定した色(colorid_t)
 */
colorid_t ColorJudge::getColorNumber(double red, double green, double blue) {
    red   *= RED_COEFFICIENT;
    green *= GREEN_COEFFICIENT;
    blue  *= BLUE_COEFFICIENT;

    hsvConverter_.convertToHsv(red, green, blue);
    hsv_t h;
    hsv_t s;
    hsv_t v;
    hsvConverter_.getHsv(h, s, v);

    if (v < 50.0 && s < 150.0) {
        return COLOR_BLACK;    // 黒
    } else if (50.0 <= v && s < 150.0) {
        return COLOR_WHITE;    // 白
    } else if (v < 5.0) {
        return COLOR_BLACK;    // 黒
    } else if (350 < h || h < 15) {
        return COLOR_RED;      // 赤
    } else if (190 < h && h < 250) {
        return COLOR_BLUE;     // 青
    } else if (20 < h && h < 70) {
        return COLOR_YELLOW;   // 黄
    } else if (100 < h && h < 160) {
        return COLOR_GREEN;    // 緑
    }

    return COLOR_NONE;
}

}

