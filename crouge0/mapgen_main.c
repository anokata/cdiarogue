#include "map_gen.h"
// TODO map util for convert?

int main() {
    string file = "/tmp/test.map";
    out_map(file, 6, 3);
    Map m = load_map(file);
    print_map(m);

    free_map(m);
}

