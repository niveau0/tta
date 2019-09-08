inherit "/base/stair";

#include <door.h>
#include "../../include/moria.h"

void
create_stair()
{
    set_name("staircase");
    add_name("stair");
    set_short("stone staircase");
    set_pshort("stone staircases");
    set_long("A staircase made of stone.\n");
    add_lwvs(0, 200000, 210000, 80);
    set_connection(MORIA_ROOM+"level0/w_entry_hall");
}

int *
query_range()
{
    return ({110, 210, 360});
    
}


static nomask void
enter_env(object dest, object old)
{
/*    
    if (!dest)
	return;
    
    if (dest->query_room_file() ==
	"/home/yavanna/moria/room/level0/w_entry_hall")
    {
	set_direction_desc("leading down to the gate");
	set_connection("/home/yavanna/moria/room/level0/w_entry", 2,
		       STAIR_DOWN);
    }
    else
    {
	set_direction_desc("leading up");
	set_connection("/home/yavanna/moria/room/level0/w_entry_hall", 2,
		       STAIR_UP);
		       }
*/
}
