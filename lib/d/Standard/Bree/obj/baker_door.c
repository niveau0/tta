
inherit "/base/door";

#include <door.h>
#include <event.h>
#include <material.h>
#include <properties.h>

void
create_door()
{
    set_name("door");
    set_name("oakdoor");
    set_name("bakerdoor");
    add_adj("small");
    set_short("small oakdoor");
    set_pshort("small oakdoors");
    set_long("A small oakdoor with a drawn pretzel on it.\n");
    set_status(DOOR_OPEN);
    set_key("/d/Standard/Bree/obj/baker_key");
    add_lwvs(0, 50000, 100000, 80);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_event(E_MOVE, "door_move", this_object());
}

nomask void
door_move(object ob, object from, object to)
{
    if (ob != this_object() || !to)
	return;
    
    if (to->query_room_file() == "/d/Standard/Bree/room/bakery")
    {
	set_direction_desc("leading outside to the marketplace");
	set_connection("/d/Standard/Bree/room/marketplace", 1);
    }
    else
    {
	set_direction_desc("leading into the bakery of Bree");
	set_connection("/d/Standard/Bree/room/bakery", 1);
    }
    set_status(DOOR_OPEN); /* initialize correct description */
}
