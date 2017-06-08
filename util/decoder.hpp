/**
 * @file decoder.hpp
 * @author Suguru Kouchi
 */
#ifndef DECODER_HPP
#define DECODER_HPP

namespace ie {

class Decoder{
public:
    Decoder();
    Decoder(int initPosition);
    void setPositionFromCode(int initPosition);
    int getPosition(int n) const;
    void getPosition(int& black, int& red, int& yellow, int& blue) const;
    int getBlackPosition() const;
    int getRedPosition() const;
    int getYellowPosition() const;
    int getBluePosition() const;

private:
    int blockPositionsMinusOne_[4]; // 0:黒 1:赤 2:黄 3:青
};

}

#endif
