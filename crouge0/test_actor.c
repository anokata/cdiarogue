#include "actor.h"
#include <stdio.h>
#include <assert.h>

char *actor_line = "pinkporing:p:5:3:_cn_white:BehaviorSimpleAttacker:StatusLive:4:43:2:1:1:2:RoleMonster:0:1:1:0:\n";
int actor_fields_count = 19;

void test() {
    Actor a = make_actor('a', 0, 0);
    actor_free(a);

    char * in = strdup(actor_line);
    Strings s = parse_dsv_line(in, actor_fields_count);
    printf("%s \n", s[0]);
    Strings it = s;
    while (*it) {
        printf("- %s\n", *it++);
    }

    a = actor_from_strings(s);
    char *dump = actor_serialize(a);
    printf("CMP:\n%s\n%s", in, dump);
    assert(!strcmp(in, dump));
    printf("actor= %s", dump);
    free(dump);
    /* free(a->items_file); */
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
#include <limits.h>
    long long int max = -1;
    printf("%llu\n%llx\n", max,ULLONG_MAX);
}

void test_dmg() {
    // make actor
    char *player_file = "./maps/you"; // make test files
    char *player_items_file = "./maps/you.items";
    GList *lst = actors_load(player_file);
    Actor actor = lst->data;
    g_list_free(lst);
    // make weapon
    Items i = items_load(player_items_file);
    Item w = g_list_nth_data(i, 0);
    printf("atk:%d\n", actor_stat_attack(actor));
    // equip weapon
    actor_equip(actor, w);
    printf("atk:%d\n", actor_stat_attack(actor));
    printf("dis:%d\n", actor_weapon_dispersion(actor));
    printf("%d - %d\n",
        actor_stat_attack(actor) - actor_weapon_dispersion(actor),
        actor_stat_attack(actor) + actor_weapon_dispersion(actor)
    );

    actor->basestat_dexterity = 250;
    printf("dex:%f\n", actor_stat_dodge(actor));;
    // tst dmg X times
    for (int i = 0; i < 10; i++) {
        float dod = actor_calc_dodge(actor, actor);
        printf("dod: %f\n", dod);

        int dmg = actor_calc_damage(actor, actor);
        printf("dmg: %d\n", dmg);
    }
    int x = 0;
    int n = 123;
    int p = 93;
    for (int i = 0; i < n; i++) {
        if (chance(p)) x++;
        printf("dice: %d ", x);
    }
    printf("%%=%f\n", x / (1.0 * n));
    items_free(&i);
    actor_free(actor);
}

void test_take() {
    char *player_file = "./maps/you";
    char *player_items_file = "./maps/you.items";
    Actor you = NULL;
    load_player(&you, player_file);
    Items i = items_load(player_items_file);

    GList *it = you->items;
    while (it) {
        Item itm = it->data;
        printf("* %s (%d)\n", itm->name, itm->count);
        it = g_list_next(it);
    }

    Item x = g_list_nth_data(i, 0);
    item_add_stacked(&you->items, x);
    i = g_list_remove(i, x);
    Item y = g_list_nth_data(you->items, 0);
    printf("count: %d %s\n", y->count, y->name);

    it = you->items;
    while (it) {
        Item itm = it->data;
        printf("* %s (%d)\n", itm->name, itm->count);
        it = g_list_next(it);
    }

    items_free(&i);
    actor_free(you);
}

void test_clone() {
    char *player_file = "./maps/you";
    Actor you = NULL;
    load_player(&you, player_file);
    Actor clone = actor_clone(you);
    actor_free(you);
    actor_free(clone);
}

void test_save_with_items() {
    GList *as = actors_load("./maps/lvl1.actors");
    actors_save("/tmp/as", as);
    free_actors(&as);
}

int main() {
    srand(time(NULL));
    test();
    test_exp();
    printf("Test DMG\n");
    test_dmg();
    test_take();
    test_clone();
    test_save_with_items();
}
