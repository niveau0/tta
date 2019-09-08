inherit "/base/door";

#include <door.h>
#include <event.h>
#include <material.h>
#include <properties.h>

static void
create_door()
{
    set_name("door");
    set_name("watch door");
    add_adj("strong");
    add_adj("wooden");
    set_short("strong wooden door");
    set_pshort("strong wooden doors");
    set_long("A strong wooden door that surely locks something dangerous.\n");
    set_status(DOOR_LOCKED);
    set_key("/d/Rhudaur/Orc_caves/obj/watch_key");
    add_lwvs(0, 50000, 100000, 80);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_event(E_MOVE, "door_move", this_object());
}

nomask void
door_move(object ob, object from, object to)
{
    if (ob != this_object() || !to)
	return;
    
    if (to->query_room_file() == "/d/Rhudaur/Orc_caves/room/watch_room_2")
    {
	set_direction_desc("leading inside a dark room");
	set_connection("/d/Rhudaur/Orc_caves/room/kennel", 1);
    }
    else
    {
	set_direction_desc("leading into the watch room");
	set_connection("/d/Rhudaur/Orc_caves/room/watch_room_2", 1);
    }
    set_status(DOOR_LOCKED); /* initialize correct description */
}
