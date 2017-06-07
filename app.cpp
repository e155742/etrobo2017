#include <vector>
#include "app.h"
#include "util.h"
#include "hsv_converter.hpp"

void main_task(intptr_t unused) {
    std::vector<rgb_t> rgb = {0, 0, 0};
    inputInt(rgb, "Enter RGB");
    msg_clear();
    HsvConverter hsv(rgb[0], rgb[1], rgb[2]);
    msg_f("HSV color", 1);
    char str[64];
    sprintf(str, "Hue        %f", hsv.getHue());
    msg_f(str, 3);
    sprintf(str, "Saturation %f", hsv.getSaturation());
    msg_f(str, 4);
    sprintf(str, "Value      %f", hsv.getLightness());
    msg_f(str, 5);
    while(true) {;}
}
