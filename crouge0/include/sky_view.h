#include <stdlib.h>

// Escapes
#define RED "\033[0;31;44m" // 7 chars len
#define BLUE "\033[0;34m"
#define DEFAULT "\033[0;37m"
#define BGBLUE "\033[0;44m"
#define BGBLUEB "\033[0;46m"
#define BGBLUED "\033[48;2;0;0;90m \033[48;2;0;0;130m \033[48;2;0;0;160m "
#define BGBLUE_END "\033[48;2;0;0;160m \033[48;2;0;0;130m \033[48;2;0;0;90m "

enum Weather { WTHR_SUNNY, WTHR_FOG, WTHR_RAIN };

#define SV_SUNRISE 6
#define SV_SUNDOWN 22

typedef struct Sky {
    int hour;
    enum Weather weather;
    int width;
} *Sky;


Sky make_sky();
void free_sky(Sky s);

char *sky_draw_string(Sky s);
char *sky_draw_cc(Sky s, int x, int y);
