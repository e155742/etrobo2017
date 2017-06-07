/**
 * @file decoder.cpp
 * @brief 初期位置コードをデコードし、各色ブロックの位置を割り出すクラス。
 *
 * @author Suguru Kouchi
 */
#include "decoder.hpp"
#include <cmath>

/**
 * 引数なしで初期化すると初期位置コード0でデコードされる。
 */
Decoder::Decoder() {
    setPositionFromCode(0);
}

/**
 * 初期位置コードを指定してデコード。
 *
 * @param initPosition 初期位置コード
 */
Decoder::Decoder(int initPosition) {
    setPositionFromCode(initPosition);
}

/**
 * 初期位置コードから各ブロックの位置(-1)を割り出す。<br>
 * 割り出された値はそれぞれメンバ変数 blockPositionsMinusOne_[n] に代入される。<br>
 * n が色に対応する。0:黒 1:赤 2:黄 3:青
 *
 * @param initPosition 初期位置コード
 */
void Decoder::setPositionFromCode(int initPosition) {
    static int count = 0;
    blockPositionsMinusOne_[count] = initPosition / std::pow(11, 3 - count);
    if (count++ < 3) {
        setPositionFromCode(initPosition % static_cast<int>(std::pow(11, 4 - count)));
    }
    count = 0;
}

/**
 * 指定した色のブロックの位置を取得
 *
 * @param n 色を指定 0:黒 1:赤 2:黄 3:青
 * @return  指定した色ブロックの位置
 */
int Decoder::getPosition(int n) const {
    if (n > 3) {
        n = 3;
    }
    return blockPositionsMinusOne_[n] + 1;
}

/**
 * 全ての色ブロックの位置を一気に取得
 *
 * @param black  黒ブロックの位置を収納する変数
 * @param red    赤ブロックの位置を収納する変数
 * @param yellow 黄ブロックの位置を収納する変数
 * @param blue   青ブロックの位置を収納する変数
 */
void Decoder::getPosition(int& black, int& red, int& yellow, int& blue) const {
    black  = getBlackPosition();
    red    = getRedPosition();
    yellow = getYellowPosition();
    blue   = getBluePosition();
}

/**
 * 黒ブロックの位置を取得
 *
 * @return 黒ブロックの位置
 */
int Decoder::getBlackPosition() const {
    return blockPositionsMinusOne_[0] + 1;
}

/**
 * 赤ブロックの位置を取得
 *
 * @return 赤ブロックの位置
 */
int Decoder::getRedPosition() const {
    return blockPositionsMinusOne_[1] + 1;
}

/**
 * 黄ブロックの位置を取得
 *
 * @return 黄ブロックの位置
 */
int Decoder::getYellowPosition() const {
    return blockPositionsMinusOne_[2] + 1;
}

/**
 * 青ブロックの位置を取得
 *
 * @return 青ブロックの位置
 */
int Decoder::getBluePosition() const {
    return blockPositionsMinusOne_[3] + 1;
}
