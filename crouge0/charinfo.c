#include "charinfo.h"

static char buf[BUFSIZE * 10];

char *charinfo_print_edit(Actor player) {
    snprintf(buf, BUFSIZE * 10, 
"(s) str: %d  (c) con: %d \t press (key) to spend points to up \n\
(d) dex: %d  (i) int: %d  \t or Shift-(key) to down\n\
", 
    player->basestat_strength,
    player->basestat_constitution,
    player->basestat_dexterity,
    player->basestat_intelligence
    );
    return buf;
}

char *charinfo_print(Actor player) {
    char *right_hand = item_descript(player->equiped_right_hand);
    char *head = item_descript(player->equiped_head);
    char *foot = item_descript(player->equiped_foot);
    char *legs = item_descript(player->equiped_legs);
    char *arms = item_descript(player->equiped_arms);
    char *body = item_descript(player->equiped_body);
    snprintf(buf, BUFSIZE * 10, 
"Name: %s %d$\n\
hp/max: %d/%d\n\
mp/max: %d/%d\n\
str: %d  con: %d \n\
dex: %d  int: %d \n\
atk: %d-%d  def: %d\n\
dodge: %f \n\
exp: %ld  lvl: %d\n\
*points to spend: %d\n\
\n\
Equiped:\n\
weapon: %s gain %d atk \n\
head: %s gain %d def \n\
foot: %s gain %d def \n\
legs: %s gain %d def \n\
arms: %s gain %d def \n\
body: %s gain %d def \n\
", 
    player->name, actor_money_count(player),
    player->stat_hp, actor_stat_maxhp(player),
    player->stat_mp, actor_stat_maxmp(player),
    player->basestat_strength,
    player->basestat_constitution,
    player->basestat_dexterity,
    player->basestat_intelligence,
    actor_stat_attack(player) - actor_weapon_dispersion(player),
    actor_stat_attack(player) + actor_weapon_dispersion(player),
    actor_stat_defence(player),
    actor_stat_dodge(player),
    player->exp, player->lvl,
    player->stat_points,
    right_hand,
    item_value(player->equiped_right_hand),
    head,
    item_value(player->equiped_head),
    foot,
    item_value(player->equiped_foot),
    legs,
    item_value(player->equiped_legs),
    arms,
    item_value(player->equiped_arms),
    body,
    item_value(player->equiped_body)
    );

    free(right_hand);
    free(head);
    free(foot);
    free(legs);
    free(arms);
    free(body);

    return buf;
}

int charinfo_draw(void* data) {
    G g = data;
    Actor player = g->player;
    clear();
    cc_printxy("@", cn_white, 0, 0);
    charinfo_print(player);
    cc_printxy(buf, cn_white, 0, 1);
    if (g_is_stat_edit(g)) {
        charinfo_print_edit(player);
        cc_printxy(buf, cn_white, 0, 20);
    }
    return 0;
}

int charinfo_key(void* data) {
    G g = data;
    char key = g->key;
    Actor you = g->player;
    if (!g_is_stat_edit(g)) {
        switch (key) {
            case 'k':
                g->mode |= G_STAT_EDIT;
                break;
            case '.':
                break;
            default:
                ss_setstate(state, State_cursor);
                break;
    
        }
        return 0;
    }
    switch (key) {
        case 'k':
            g->mode ^= G_STAT_EDIT;
            break;
        case 'q':
            ss_setstate(state, State_cursor);
            g->mode ^= G_STAT_EDIT;
            break;
        case 's':
            if (you->stat_points) {
                you->stat_points--;
                you->basestat_strength++;
            }
            break;
        case 'c':
            if (you->stat_points) {
                you->stat_points--;
                you->basestat_constitution++;
            }
            break;
        case 'd':
            if (you->stat_points) {
                you->stat_points--;
                you->basestat_dexterity++;
            }
            break;
        case 'D':
            if (you->basestat_strength) {
                you->stat_points++;
                you->basestat_dexterity--;
            }
            break;
        case 'i':
            if (you->stat_points) {
                you->stat_points--;
                you->basestat_intelligence++;
            }
            break;
        case 'I':
            if (you->basestat_strength) {
                you->stat_points++;
                you->basestat_intelligence--;
            }
            break;
        case 'C': // if can? or reset points
            if (you->basestat_constitution) {
                you->stat_points++;
                you->basestat_constitution--;
            }
            break;
        case 'S':
            if (you->basestat_strength) {
                you->stat_points++;
                you->basestat_strength--;
            }
            break;

    }
    return 0;
}
