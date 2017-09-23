/**
 * @file block_field.hpp
 * @author Suguru Kouchi
 */
#ifndef BLOCK_FIELD_HPP
#define BLOCK_FIELD_HPP

#include <vector>
#include "marker.hpp"
#include "localization.hpp"
#include "decoder.hpp"

namespace ie {

class BlockField {
public:
    BlockField(Localization& localization, Decoder& decoder, int greenPosition);
    void revisionBotPoint(int markerNum, point_t degree);
    int getTargetMarker() const;

private:
    std::vector<Marker> markerVector_;
    Localization& localization_;
    Decoder& decoder_;
    int targetMarker_ = 14; // 目標となるマーカー

};

}

#endif
