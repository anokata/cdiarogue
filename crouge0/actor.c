#include "actor.h"
extern EError global_error;
extern char *ColorNames[];

const char actor_file_header[] = "name:char:x:y:color:behavior:status:hp:con:str:dex:int:role:items_file:exp:lvl:sp:\n";
const char actor_file_type[] = "# vi: filetype=sh\n";
const char actor_dump_format[] = "%s:%c:%d:%d:%s:%s:%s:%d:%d:%d:%d:%d:%s:0:%ld:%d:%d:\n";

char *RoleNames[] = {
    FOREACH_ROLE(MAKE_STRING)
    NULL
};

Role role_from_str(char *str) {
    char **it = RoleNames;
    Role role = 0;
    while (*it) {
        if (!strcmp(*it, str)) return role;
        role++;
        it++;
    }
    return RoleLength;
}

char *BehaviorNames[] = {
    FOREACH_BEHAVIOR(MAKE_STRING)
    NULL
};

char *StatusNames[] = {
    FOREACH_STATUS(MAKE_STRING)
    NULL
};

Behavior behavior_from_str(char *str) {
    ENUM_FROMSTR(Behavior)
}

Status status_from_str(char *str) {
    ENUM_FROMSTR(Status)
}


Actor make_actor(char c, int x, int y) {
    Actor actor = malloc(sizeof(struct Actor));
    actor->c = c;
    actor->x = x;
    actor->y = y;
    actor->directed.x = x;
    actor->directed.y = y;
    actor->behavior = BehaviorRandom;
    actor->color = cn_white;
    actor->role = RoleMonster;
    actor->name = NULL;
    actor->status = StatusLive;
    actor->items = NULL;
    actor->basestat_constitution = 1;
    actor->basestat_strength = 1;
    actor->stat_hp = actor_stat_maxhp(actor);
    actor->equiped_right_hand = NULL;
    actor->equiped_head = NULL;
    actor->equiped_foot = NULL;
    actor->equiped_legs = NULL;
    actor->equiped_arms = NULL;
    actor->equiped_body = NULL;
    actor->exp = 1;
    actor->lvl = 1;
    actor->stat_points = 2;
    return actor;
}

void actor_free(Actor actor) {
    if (!actor) return;
    items_free(&actor->items);
    if (actor->name) free(actor->name);
    free(actor);
}

Actor actor_clone(Actor actor) {
    Actor clone = make_actor(actor->c, actor->x , actor->y);
    memcpy(clone, actor, sizeof(struct Actor));
    clone->name = strdup(actor->name);
    return clone;
}

void free_actors(GList **actors) {
    GList *it = *actors;
    while (it) {
        Actor actor = it->data;
        actor_free(actor);
        it = g_list_next(it);
    }
    g_list_free(*actors);
    *actors = NULL;
}

bool _actor_isat_directed_place(Actor actor) { 
    return (actor->x == actor->directed.x) && (actor->y == actor->directed.y);
}

int _actor_direct_diffx(Actor actor) {
    int diff = (actor->directed.x - actor->x);
    return diff == 0 ? 0 : (diff > 0 ? 1 : -1);
}

int _actor_direct_diffy(Actor actor) {
    int diff = (actor->directed.y - actor->y);
    return diff == 0 ? 0 : (diff > 0 ? 1 : -1);
}

CharPoint actor_as_charpoint_cast(Actor actor) {
    return (struct CharPoint *) actor;
}

int actor_stat_maxhp(Actor actor) {
    return actor->basestat_constitution * 10;
}

int actor_stat_attack(Actor actor) {
    int atk = actor->basestat_strength * 2;
    if (actor->equiped_right_hand) atk += actor->equiped_right_hand->value;
    return atk;
}

float actor_stat_dodge(Actor actor) {
    return actor->basestat_dexterity * 0.9;
}

int actor_attack_dispersion(Actor actor) {
    int dis = 0;
    dis += actor_weapon_dispersion(actor);
    return dis * 2 + 1;
}

int actor_weapon_dispersion(Actor actor) {
    int dis = 0;
    if (actor->equiped_right_hand) dis += actor->equiped_right_hand->dispersion;
    return dis;
}

int actor_stat_defence(Actor actor) {
    int def = 0;
    if (actor->equiped_head) def += actor->equiped_head->value;
    if (actor->equiped_foot) def += actor->equiped_foot->value;
    if (actor->equiped_legs) def += actor->equiped_legs->value;
    if (actor->equiped_arms) def += actor->equiped_arms->value;
    if (actor->equiped_body) def += actor->equiped_body->value;
    return def;
}

int actor_stat_regen(Actor actor) {
    return actor->basestat_constitution / 4 - 1;
}

float actor_calc_dodge(Actor attacker, Actor defender) {
    float dodge = actor_stat_dodge(defender);
    int lvl_delta = defender->lvl - attacker->lvl;
    dodge = dodge * (1 + lvl_delta / 10.0);
    return dodge;
}

int actor_calc_damage(Actor attacker, Actor defender) {
    if (chance(actor_calc_dodge(attacker, defender))) return 0;

    int atk = actor_stat_attack(attacker);
    int dis = actor_attack_dispersion(attacker);
    int dispersion_value = 0;
    if (dis) {
        dispersion_value = (rand() % dis) - dis/2;
    }
    int def = actor_stat_defence(defender);
    int dif = (atk + dispersion_value) - def;
    return MAX(dif, 0);
}

void actor_heal(Actor actor, int value) {
    int hp = actor->stat_hp + value;
    int maxhp = actor_stat_maxhp(actor);
    if (hp > maxhp) {
        actor->stat_hp = maxhp;
    } else {
        actor->stat_hp = hp; 
    }
}

bool actor_equip(Actor actor, Item item) {
    switch (item->cls) {
        case ItemWeaponCls:
            if (actor->equiped_right_hand) {
                global_error = Error_Actor_SlotBusy;
                return false;
            }
            actor->equiped_right_hand = item;
            return true;
            break;
        case ItemHeadEquipCls:
            if (actor->equiped_head) {
                global_error = Error_Actor_SlotBusy;
                return false;
            }
            actor->equiped_head = item;
            return true;
            break;
        default:
            global_error = Error_Actor_NotEquptable;
            return false;
            break;
    }
    return false;
}

bool actor_item_is_equiped(Actor actor, Item item) {
    if (actor->equiped_right_hand == item) return true;
    if (actor->equiped_head == item) return true;
    if (actor->equiped_foot == item) return true;
    if (actor->equiped_legs == item) return true;
    if (actor->equiped_arms == item) return true;
    if (actor->equiped_body == item) return true;
    /* TODO slots to array with enum? */
    return false;
}

Item *actor_item_slot(Actor actor, Item item) {
    if (actor->equiped_right_hand == item) return &actor->equiped_right_hand;
    if (actor->equiped_head == item) return &actor->equiped_head;
    if (actor->equiped_foot == item) return &actor->equiped_foot;
    if (actor->equiped_legs == item) return &actor->equiped_legs;
    if (actor->equiped_arms == item) return &actor->equiped_arms;
    if (actor->equiped_body == item) return &actor->equiped_body;
    return NULL;
}

Actor actor_from_strings(Strings str) {
    Actor actor = make_actor(str[1][0], atoi(str[2]), atoi(str[3]));
    actor->name = strdup(str[0]);
    actor->color = cc_color_from_str(str[4]);
    actor->behavior = behavior_from_str(str[5]);
    actor->status = status_from_str(str[6]);
    actor->stat_hp = atoi(str[7]);
    actor->basestat_constitution = atoi(str[8]);
    actor->basestat_strength = atoi(str[9]);
    actor->basestat_dexterity = atoi(str[10]);
    actor->basestat_intelligence = atoi(str[11]);
    actor->role = role_from_str(str[12]);
    //actor->items = str[13];
    actor->exp = atol(str[14]);
    actor->lvl = atoi(str[15]);
    actor->stat_points = atoi(str[16]);
    return actor;
}

char *actor_serialize(Actor actor) {
    char buf[BUFSIZE];
    snprintf(buf, BUFSIZE, 
        actor_dump_format,
        actor->name,
        actor->c,
        actor->x,
        actor->y,
        ColorNames[actor->color.color],
        BehaviorNames[actor->behavior],
        StatusNames[actor->status],
        actor->stat_hp,
        actor->basestat_constitution,
        actor->basestat_strength,
        actor->basestat_dexterity,
        actor->basestat_intelligence,
        RoleNames[actor->role],
        actor->exp,
        actor->lvl,
        actor->stat_points
        );
    return strdup(buf);
}

GList *actors_load(char* filename) {
    GList *actors = NULL; // do not forget free GList itself
    filename = strdup(filename);
    StringTable st = parse_dsv_file(filename);
    StringTable it = &st[1]; // first line is header
    do {
        Actor actor = actor_from_strings(*it);
        actor_add(&actors, actor); 
        DEBUG_PRINT("load actor: %s\n", actor->name);
    } while (*++it);

    free_dsv_table(st);
    free(filename);
    return actors;
}

void actors_save(char* filename, GList *actors) {
    GList *it = actors;
    FILE *out = fopen(filename, "w+");
    fwrite(actor_file_header, strlen(actor_file_header), 1, out);
    fwrite(actor_file_type, strlen(actor_file_type), 1, out);

    while (it) {
        Actor actor = it->data;
        char *line = actor_serialize(actor);
        fwrite(line, strlen(line), 1, out);
        free(line);
        it = g_list_next(it);
    }

    fclose(out);
}

void actor_add(GList **actors, Actor actor) {
    *actors = g_list_append(*actors, actor);
}

void actor_exp_gain(Actor actor, long exp) {
    // TODO
    actor->exp += exp;
    
    while (is_lvl_up(actor->lvl, actor->exp)) {
        actor->lvl++;
        actor_lvl_up(actor);
    }
}

void actor_lvl_up(Actor actor) {
    // TODO
    actor->stat_points += 3; // Debug?
}

long actor_defeat(Actor actor, Actor subject) {
    long exp = calc_exp_gain(actor, subject);
    actor_exp_gain(actor, exp);
    return exp;
}

/* lvl & exp calc */
int need_defeat_to_next_lvl(int current_lvl) {
    return (current_lvl) + 10;
}

long calc_exp_gain(Actor actor, Actor subject) {
    if (subject->lvl > actor->lvl) {
        return (subject->exp) * (subject->lvl - actor->lvl);
    } else if (subject->lvl == actor->lvl) {
        return subject->exp;
    } else { // less
        return (subject->exp) / (actor->lvl - subject->lvl);
    }
}

static long exp_road[1024] = {0};

void fill_exp_road() {
    long exp = 0;
    int avg_gain = 5;
    for (int lvl = 0; lvl < 1024; lvl++) {
        int n = need_defeat_to_next_lvl(lvl);
        exp += n * avg_gain;
        exp_road[lvl] = exp;
    }
}

bool is_lvl_up(int lvl, long new_exp) {
    return exp_road[lvl + 1] <= new_exp;
}

long exp_get_to_next(int lvl) {
    return exp_road[lvl + 1];
}

void test_exp() {
    fill_exp_road();
    long exp = 0;
    long old_exp = 0;
    int avg_gain = 5;
    for (int lvl = 0; lvl < 100; lvl++) {
        int n = need_defeat_to_next_lvl(lvl);
        exp += n * avg_gain;
        exp_road[lvl] = exp;
        printf("lvl: %d\t exp: %ld\t n: %d\t exp diff: %ld \n", lvl, exp_road[lvl], n, exp - old_exp);
        old_exp = exp;
    }

    for (int lvl = 0; lvl < 10; lvl++) {
        printf("lvl: %d\t exp: %ld\n ", lvl, exp_road[lvl]);
    }
    for (int lvl = 200; lvl < 210; lvl++) {
        printf("lvl: %d\t exp: %ld\n ", lvl, exp_road[lvl]);
    }
    for (int lvl = 900; lvl < 910; lvl++) {
        printf("lvl: %d\t exp: %ld\n ", lvl, exp_road[lvl]);
    }
}
