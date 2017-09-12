#include "actor.h"
#include <stdio.h>
#include <assert.h>

int main() {
    Actor a = make_actor('a', 0, 0);
    actor_free(a);

    char * in = strdup("pink poring:p:3:3:pink:BehaviorSimpleAttacker:StatusLive:4:2:1:RoleMonster:0:\n");
    Strings s = parse_dsv_line(in, 12);
    printf("%s \n", s[0]);
    Strings it = s;
    while (*it) {
        printf("- %s\n", *it++);
    }

    a = actor_from_strings(s);
    char *dump = actor_serialize(a);
    printf("actor= %s", dump);
    free(dump);
    actor_free(a);

    free_dsv_strings(s);
    s = NULL;
    free(in);
    in = NULL;
}
