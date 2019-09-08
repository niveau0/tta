inherit "/base/room";

#include <macros.h>

static void
create_room()
{
    set_room_file(MASTER);
}

/* never save a player in this room */
int
query_no_save()
{
    return 1;
}
