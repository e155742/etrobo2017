#include <vector>
#include "app.h"
#include "util.h"
#include "decoder.hpp"
#include "hsv_converter.hpp"
#include "file_output.hpp"

void main_task(intptr_t unused) {
    std::vector<ie::rgb_t> rgb = {0, 0, 0};
    inputInt(rgb, "Enter RGB");
    msg_clear();
    ie::HsvConverter hsv(rgb[0], rgb[1], rgb[2]);
    msg_f("HSV color", 1);
    char str[64];
    sprintf(str, "Hue        %f", hsv.getHue());
    msg_f(str, 3);
    sprintf(str, "Saturation %f", hsv.getSaturation());
    msg_f(str, 4);
    sprintf(str, "Value      %f", hsv.getLightness());
    msg_f(str, 5);

    ie::Decoder d(12008);
    msg_f("Black, Red, Yellow, Blue", 7);
    sprintf(str, "%d,    %d,   %d,      %d", d.getBlackPosition(), d.getRedPosition(), d.getYellowPosition(), d.getBluePosition());
    msg_f(str, 8);

    ie::FileOutput fo("test.txt");
    for (double i = 10.12345678900; i < 10.12345678915; i+=0.00000000001) {
        fo.fileWrite(i, "%2.13f");
    }
    fo.close();
}
