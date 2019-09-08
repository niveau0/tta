inherit "/base/door";

#include <door.h>
#include <event.h>
#include <material.h>
#include <properties.h>

static void
create_door()
{
    set_name("door");
    set_name("tailordoor");
    add_adj("fine");
    add_adj("coloured");
    set_short("coloured door");
    set_pshort("coloured doors");
    set_long("A fine coloured door. A sign with a needle and a pair of \n"+
	     "scissors hangs above it.\n");
    set_status(DOOR_OPEN);
    set_key("/d/Standard/Bree/obj/tailor_key");
    add_lwvs(0, 50000, 100000, 80);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_event(E_MOVE, "door_move", this_object());
}

nomask void
door_move(object ob, object from, object to)
{
    if (ob != this_object() || !to)
	return;
    
    if (to->query_room_file() == "/d/Standard/Bree/room/tailor")
    {
	set_direction_desc("leading outside to the Ale Road");
	set_connection("/d/Standard/Bree/room/aleroad", 1);
    }
    else
    {
	set_direction_desc("leading into the tailoring");
	set_connection("/d/Standard/Bree/room/tailor", 1);
    }
    set_status(DOOR_OPEN); /* initialize correct description */
}
