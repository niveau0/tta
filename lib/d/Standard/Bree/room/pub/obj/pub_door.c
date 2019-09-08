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
    add_name("entrance");
    add_adj("wide");
    set_short("entrance");
    set_pshort("entrances");
    set_long_bs("Being stained and scratched, this door seems to have "+
	     "witnessed alot of stories.\n");
    set_status(DOOR_OPEN);
    set_key("/d/Standard/Bree/room/pub/obj/pub_key");
    add_lwvs(0, 55000, 120000, 90);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_event(E_MOVE, "door_move", this_object());
}

nomask void
door_move(object ob, object from, object to)
{
    if (ob != this_object() || !to)
	return;
    
    if (to->query_room_file() == "/d/Standard/Bree/room/pub/pub")
    {
	set_direction_desc("leading outside to the marketplace");
	set_connection("/d/Standard/Bree/room/marketplace", 1);
    }
    else
    {
	set_direction_desc("leading into a pub");
	set_connection("/d/Standard/Bree/room/pub/pub", 1);
    }
    set_status(DOOR_OPEN); /* initialize correct description */
}
