#include "events.h"
#include "assert.h"

int main() {
    start_events();

    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            for (int z = 0; z < 3; z++) {
                printf(" %d ", _3d_array_index(x, y, z, 3));
            }
        }
    }

    event_register(ActionCollide, RolePlayer, RolePlayer, _no_action);
    Actor actor = make_actor(' ', 0, 0);
    actor->role = RolePlayer;
    ActionFunc f = event_get(ActionCollide, actor, actor, NULL);
    printf("%p", (void*)f);
    assert(f != NULL);
    free(actor);

    end_events();
}

