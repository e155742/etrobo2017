/**
 * @file block_field.cpp
 * @brief ブロック並べのフィールドに関係するクラス
 *
 * @author Suguru Kouchi
 */
#include "block_field.hpp"
#include "robo_meta_datas.hpp"
#include "util.h"

namespace ie {

BlockField::BlockField(Localization& localization, Decoder& decoder, int greenPosition):
markerVector_(15), localization_(localization), decoder_(decoder) {
    // マーカーの生成
    markerVector_[0].setFields(15, -164.7, 614.7, COLOR_NONE);
    markerVector_[1].setFields(10, 614.7, 614.7, COLOR_NONE);
    markerVector_[2].setFields(11, 1394.1, 614.7, COLOR_NONE);
    markerVector_[3].setFields(15, 2173.5, 614.7, COLOR_NONE);
    markerVector_[4].setFields(9, 225, 389.7, COLOR_NONE);
    markerVector_[5].setFields(5, 1004.4, 389.7, COLOR_NONE);
    markerVector_[6].setFields(6, 1783.8, 389.7, COLOR_NONE);
    markerVector_[7].setFields(4, 614.7, 164.7, COLOR_NONE);
    markerVector_[8].setFields(6, 1394.1, 164.7, COLOR_NONE);
    markerVector_[9].setFields(1, 0.0, 0.0, COLOR_NONE);
    markerVector_[10].setFields(15, 2008.8, 0.0, COLOR_NONE);
    markerVector_[11].setFields(2, 389.7, -225.0, COLOR_NONE);
    markerVector_[12].setFields(3, 839.7, -225.0, COLOR_NONE);
    markerVector_[13].setFields(5, 1169.1, -225.0, COLOR_NONE);
    markerVector_[14].setFields(8, 1619.1, -225.0, COLOR_NONE);

    // 各色のブロック位置を黒ブロックの番号に合わせる

    int blackPosition = decoder.getBlackPosition();

    int redBlockPosition = decoder.getRedPosition();
    if (redBlockPosition <= 5) {
        redBlockPosition += 1;
    } else if (redBlockPosition <= 10) {
        redBlockPosition += 3;
    } else if (redBlockPosition == 11) {
        redBlockPosition += 4;
    }

    int yellowBlockPosition = decoder.getYellowPosition();
    if (yellowBlockPosition <= 2) {
        yellowBlockPosition += 0;
    } else if (yellowBlockPosition <= 3) {
        yellowBlockPosition += 1;
    } else if (yellowBlockPosition <= 10) {
        yellowBlockPosition += 2;
    } else if (yellowBlockPosition == 11) {
        yellowBlockPosition += 3;
    }

    int bluePosition = decoder.getBluePosition();
    if (bluePosition <= 1) {
        bluePosition += 0;
    } else if (bluePosition <= 2) {
        bluePosition += 1;
    } else if (bluePosition <= 6) {
        bluePosition += 2;
    } else if (bluePosition <= 8) {
        bluePosition += 3;
    } else if (bluePosition <= 11) {
        bluePosition += 4;
    }

    // ブロックを置く
    markerVector_[blackPosition       - 1].setBlock(COLOR_BLACK);
    markerVector_[redBlockPosition    - 1].setBlock(COLOR_RED);
    markerVector_[yellowBlockPosition - 1].setBlock(COLOR_YELLOW);
    markerVector_[bluePosition        - 1].setBlock(COLOR_BLUE);
    markerVector_[greenPosition       - 1].setBlock(COLOR_GREEN);

    // 目標を決める
    int mini = 15;
    for (int i = 0; i < 15; i++) {
        if (markerVector_[i].haveColorBlock()) {
            if (markerVector_[i].getPriorityNum() < mini) {
                mini = markerVector_[i].getPriorityNum();
                targetMarker_ = i;
            }
        }
    }
}

/**
 * マーカーと角度を指定すると自己位置推定の座標を修正する。
 *
 * @param markerNum マーカーの番号
 * @param degree    乗っているラインの角度
 */
void BlockField::revisionBotPoint(int markerNum, point_t degree) {
    point_t revisionX = markerVector_[markerNum].getPointX() - polarToRectX(OFF_SET, degToRad(degree));
    point_t revisionY = markerVector_[markerNum].getPointY() - polarToRectY(OFF_SET, degToRad(degree));
    localization_.setPoint(revisionX, revisionY);
}

int BlockField::getTargetMarker() const {
    return targetMarker_;
}

}

