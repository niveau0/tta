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
    set_name("jaildoor");
    add_adj("heavy");
    set_short("heavy oakdoor");
    set_pshort("heavy oakdoors");
    set_long("A very heavy oakdoor with a heavy bar.\n");
    set_status(DOOR_OPEN);
    set_key("/d/Standard/Bree/obj/jail_key");
    add_lwvs(0, 50000, 100000, 80);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_event(E_MOVE, "door_move", this_object());
}

nomask void
door_move(object ob, object from, object to)
{
    if (ob != this_object() || !to)
	return;
    
    if (to->query_room_file() == "/d/Standard/Bree/room/jail/jailer_room")
    {
	set_direction_desc("leading outside to the street");
	set_connection("/d/Standard/Bree/room/winestreet_north", 1);
    }
    else
    {
	set_direction_desc("leading into the jail of Bree");
	set_connection("/d/Standard/Bree/room/jail/jailer_room", 1);
    }
    set_status(DOOR_OPEN); /* initialize correct description */
}
