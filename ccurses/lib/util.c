#include "util.h"

void debug_file_log(char *msg) {
    char log_filename[] = "/tmp/clog";
    FILE *fout = fopen(log_filename, "a+");
    fwrite(msg, strlen(msg), 1, fout);
    fclose(fout);
}


