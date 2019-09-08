inherit "/base/door";

#include <door.h>
#include <event.h>
#include <material.h>
#include <properties.h>

static void
create_door()
{
    set_name("door");
    add_name("exit");
    add_adj("wooden");
    add_adj("crude");
    set_short("door");
    set_pshort("doors");
    set_long_bs("You see a wooden door made from crude boards. Its main "+
		"purpose seems to prevent curious persons from peering "+
		"into the room behind it.\n");
    set_status(DOOR_CLOSED);
    add_lwvs(0, 25000, 55000, 85);
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
	set_direction_desc("leading north");
	set_connection("/d/Standard/Bree/room/pub/back", 1);
    }
    else
    {
	set_direction_desc("leading south");
	set_connection("/d/Standard/Bree/room/pub/pub", 1);
    }
    set_status(DOOR_CLOSED); /* initialize correct description */
}
