inherit "/base/door";

#include <door.h>
#include <event.h>
#include <material.h>
#include <properties.h>

static void
create_door()
{
    set_name("door");
    set_name("smithdoor");
    add_adj("large");
    add_adj("strong");
    set_short("strong door");
    set_pshort("strong doors");
    set_long("A large and very strong door.");
    set_status(DOOR_OPEN);
    set_key("/d/Standard/Bree/obj/smith_key");
    add_lwvs(0, 50000, 100000, 80);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_event(E_MOVE, "door_move", this_object());
}

nomask void
door_move(object ob, object from, object to)
{
    if (ob != this_object() || !to)
	return;
    
    if (to->query_room_file() == "/d/Standard/Bree/room/smithy")
    {
	set_direction_desc("leading outside to the Wateralley");
	set_connection("/d/Standard/Bree/room/wateralley_east3", 1);
    }
    else
    {
	set_direction_desc("leading into the smithy");
	set_connection("/d/Standard/Bree/room/smithy", 1);
    }
    set_status(DOOR_OPEN); /* initialize correct description */
}
