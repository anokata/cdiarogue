#include "app.h"

int main(int argc, char *argv[]) {
    if (argc > 1) {
        start(argv[1]);
    } else {
        start(0);
    }
    return 0;
}

