#include "charinfo.h"
#include "app.h"
#include <stdio.h>
#include <assert.h>

int main() {
    G g = new_g("./maps/town");
    load(g);
    g_post_init(g);
    printf("%s", charinfo_print(g->player));
    free_g(g);
}
