#include "app.h"
#include "util.h"

void main_task(intptr_t unused) {
    const char* hello = "Hello";
    msg_f(hello, 1);
    msg_f("EV", 2);
    msg_f(3, 3);
    while(true) {;}
}
