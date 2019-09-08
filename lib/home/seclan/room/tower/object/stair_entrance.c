inherit "/base/stair";

#include <door.h>
#include <material.h>
#include <properties.h>

void
create_stair()
{
    set_name("staircase");
    add_name("stairs");
    add_name("stair");
    add_adj("spiral");
    add_adj("winding");
    add_adj("metal");
    set_short("spiral staircase");
    set_pshort("spiral staircases");
    set_long("A staircase of metal that winds around the center column "+
	     "of the whole tower.\n");
    add_lwvs(0, 300000, 250000, 120);
    set_value(100);
    add_prop(OBJ_MATERIAL, M_BLACK_ALLOY);
    set_connection("/home/seclan/room/tower/livingroom");
}

int *
query_range()
{
    return ({ 70, 70, 120 });
}
