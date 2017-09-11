#include "charinfo.h"

int charinfo_draw(void* data) {
    G g = data;
    Actor player = g->player;
    clear();
    cc_printxy("@", cn_white, 0, 0);

    char buf[BUFSIZE * 10];
    snprintf(buf, BUFSIZE * 10, 
"Name: %s\n\
hp/max: %d/%d\n\
str: %d  con: %d\n\
atk: %d  def: %d\n\
\n\
Equiped:\n\
weapon: %s gain %d atk \n\
head: %s gain %d def \n\
foot: %s gain %d def \n\
legs: %s gain %d def \n\
arms: %s gain %d def \n\
body: %s gain %d def \n\
", 
    player->name, 
    player->stat_hp, actor_stat_maxhp(player),
    player->basestat_strength,
    player->basestat_constitution,
    actor_stat_attack(player),
    actor_stat_defence(player),
    item_descript(player->equiped_right_hand),
    item_value(player->equiped_right_hand),
    item_descript(player->equiped_head),
    item_value(player->equiped_head),
    item_descript(player->equiped_foot),
    item_value(player->equiped_foot),
    item_descript(player->equiped_legs),
    item_value(player->equiped_legs),
    item_descript(player->equiped_arms),
    item_value(player->equiped_arms),
    item_descript(player->equiped_body),
    item_value(player->equiped_body)
    );

    cc_printxy(buf, cn_white, 0, 1);
    return 0;
}

int charinfo_key(void* data) {
    G g = data;
    char key = g->key;
    switch (key) {
        case 'q':
            ss_setstate(state, State_cursor);
            break;
    }
    return 0;
}
