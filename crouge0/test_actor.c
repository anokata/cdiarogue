#include "actor.h"
#include <stdio.h>
#include <assert.h>

void test() {
    Actor a = make_actor('a', 0, 0);
    actor_free(a);

    char * in = strdup("pink poring:p:3:3:_cn_white:BehaviorSimpleAttacker:StatusLive:4:2:1:RoleMonster:0:1:1\n");
    Strings s = parse_dsv_line(in, 14);
    printf("%s \n", s[0]);
    Strings it = s;
    while (*it) {
        printf("- %s\n", *it++);
    }

    a = actor_from_strings(s);
    char *dump = actor_serialize(a);
    printf("%s%s", in, dump);
    assert(!strcmp(in, dump));
    printf("actor= %s", dump);
    free(dump);
    actor_free(a);

    free_dsv_strings(s);
    s = NULL;
    free(in);
    in = NULL;

    extern char *RoleNames[];
    extern char *StatusNames[];
    extern char *BehaviorNames[];
    printf("role: %d\n", role_from_str("RoleMonster"));
    assert(RoleMonster == role_from_str("RoleMonster"));
    assert(RoleNPC == role_from_str("RoleNPC"));
    printf("Rolestr: %s\n", RoleNames[RoleMonster]);
    printf("Rolestr: %s\n", RoleNames[RolePlayer]);
    assert(role_from_str(RoleNames[RoleNPC]) == role_from_str("RoleNPC"));

    printf("\nStatus from str: %d\n", status_from_str("StatusLive"));
    printf("Status from str: %d\n", status_from_str("StatusDead"));
    assert(StatusLive == status_from_str("StatusLive"));
    assert(StatusDead == status_from_str("StatusDead"));
    assert(status_from_str(StatusNames[StatusSleep]) == status_from_str("StatusSleep"));

    assert(BehaviorSimpleAttacker == behavior_from_str("BehaviorSimpleAttacker"));
    assert(BehaviorSimpleDirect == behavior_from_str("BehaviorSimpleDirect"));
    assert(behavior_from_str(BehaviorNames[BehaviorRandom]) == behavior_from_str("BehaviorRandom"));

    GList *as = actors_load("./maps/map_1_1.actors");
    actors_save("/tmp/as", as);
    GList *ita = as;
    while (ita) {
        Actor a = ita->data;
        printf("actor loaded: %d \n", a->stat_hp); // name error
        ita = g_list_next(ita);
    }
    free_actors(&as);
    printf("actor %ld\n", sizeof(struct Actor)); 
}

int main() {
    test();
}
