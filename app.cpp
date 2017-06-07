#include "app.h"
#include "util.h"
#include "var_entering.hpp"

void main_task(intptr_t unused) {
    int n = 3;
    VarEntering entering;
    entering.inputInt(n, "VarEntering Class");
    const char* hello = "Hello";
    msg_f(hello, 1);
    msg_f("EV", 2);
    msg_f(n, 3);
    while(true) {;}
}
