inherit "/base/door";

#include <door.h>
#include <event.h>
#include <material.h>
#include <properties.h>

#define BREE_PATH "/d/Rhudaur/Bree"

void
create_door()
{
    set_name("door");
    set_name("oakdoor");
    set_name("diningdoor");
    add_adj("wooden");
    set_short("wooden door");
    set_pshort("wooden doors");
    set_long_bs("A simple wooden door of thin boards.\n");
    set_status(DOOR_OPEN);
    set_key("/d/Rhudaur/Bree/obj/Pony_key");
    add_lwvs(0, 25000, 100000, 80);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_event(E_MOVE, "door_move", this_object());
}

nomask void
door_move(object ob, object from, object to)
{
    if (ob != this_object() || !to)
	return;
    
    if (to->query_room_file() != BREE_PATH+"/Prancing_Pony/little_dining")
    {
	set_direction_desc("leading into a little room");
	set_connection(BREE_PATH+"/Prancing_Pony/little_dining", 1);
    }
    else
    {
	set_direction_desc("leading to a small corridor");
	set_connection(BREE_PATH+"/Prancing_Pony/corridor2", 1);
    }
    set_status(DOOR_OPEN); /* initialize correct description */
}
