inherit "/base/door";

#include <door.h>
#include <event.h>
#include <material.h>
#include <properties.h>

static void
create_door()
{
    set_name("door");
    set_name("oakdoor");
    set_name("celldoor");
    add_adj("cell");
    add_adj("two");
    set_short("strong oakdoor");
    set_pshort("strong oakdoors");
    set_long("A very strong oakdoor with a heavy bar.\n");
    set_status(DOOR_OPEN);
    set_key("/d/Standard/Bree/obj/cell1_key");
    add_lwvs(0, 50000, 100000, 80);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_event(E_MOVE, "door_move", this_object());
}

nomask void
door_move(object ob, object from, object to)
{
    if (ob != this_object() || !to)
	return;
    
    if (to->query_room_file() == "/d/Standard/Bree/room/jail/cell1")
    {
	set_direction_desc("leading outside to the jail hall");
	set_connection("/d/Standard/Bree/room/jail/hall", 1);
    }
    else
    {
	set_direction_desc("leading into cell one");
	set_connection("/d/Standard/Bree/room/jail/cell1", 1);
    }
    set_status(DOOR_OPEN); /* initialize correct description */
}
