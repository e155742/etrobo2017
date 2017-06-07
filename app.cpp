#include <vector>
#include "app.h"
#include "util.h"
#include "decoder.hpp"

void main_task(intptr_t unused) {
    std::vector<int> pos = {0, 0, 0, 0, 0};
    inputInt(pos, "Enter Init position code");
    msg_clear();
    Decoder decoder(pos[4] + pos[3] * 10 + pos[2] * 100 + pos[1] * 1000 + pos[0] * 10000);
    char str[64];
    msg_f("Color block position", 1);
    sprintf(str, "Black  %d", decoder.getBlackPosition());
    msg_f(str, 3);
    sprintf(str, "Red    %d", decoder.getRedPosition());
    msg_f(str, 5);
    sprintf(str, "Yellow %d", decoder.getYellowPosition());
    msg_f(str, 7);
    sprintf(str, "Blue   %d", decoder.getBluePosition());
    msg_f(str, 9);
    while(true) {;}
}
