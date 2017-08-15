#include <glib.h>
#include "debug.h"
#include "debug.h"

int main() {
    printf("Glib log test\n");
    g_message("g_message\n"); // to stdio
    debug_file_log("msg #1\n");
}
