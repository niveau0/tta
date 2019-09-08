inherit "/base/door";

#include <door.h>
#include <event.h>
#include <material.h>
#include <properties.h>

static void
create_door()
{
    set_name("door");
    add_name("pubdoor");
    add_name("kitchendoor");
    add_name("yarddoor");
    add_adj("wooden");
    set_short("backdoor");
    set_pshort("backdoors");
    set_long_bs("A common wooden door that looks a bit neglected. Perhaps "+
		"its owner had no interest to care to much about its "+
		"appearance. Fact is, that time and weather took its toll.\n");
    set_status(DOOR_CLOSED);
    add_lwvs(0, 50000, 80000, 90);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_event(E_MOVE, "door_move", this_object());
}

nomask void
door_move(object ob, object from, object to)
{
    if (ob != this_object() || !to)
	return;
    
    if (to->query_room_file() == "/d/Standard/Bree/room/pub/back")
    {
	set_direction_desc("leading to a backyard");
	set_connection("/d/Standard/Bree/room/grass_northpub", 1);
    }
    else
    {
	set_direction_desc("leading into a building");
	set_connection("/d/Standard/Bree/room/pub/back", 1);
    }
    set_status(DOOR_CLOSED); /* initialize correct description */
}
