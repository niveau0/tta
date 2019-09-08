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
    add_adj("wooden");
    set_short("wooden door");
    set_pshort("wooden doors");
    set_long_bs("A wooden door of dark brown boards.\n");
    set_status(DOOR_CLOSED);
    set_key("/d/Rhudaur/Bree/obj/hob1_key");
    add_lwvs(0, 30000, 130000, 85);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_event(E_MOVE, "door_move", this_object());
}

nomask void
door_move(object ob, object from, object to)
{
    if (ob != this_object() || !to)
	return;
    
    if (to->query_room_file() != BREE_PATH+"/Prancing_Pony/hobbit_room2")
    {
	set_direction_desc("leading into a dark room");
	set_connection(BREE_PATH+"/Prancing_Pony/hobbit_room2", 1);
    }
    else
    {
	set_direction_desc("leading to a small corridor");
	set_connection(BREE_PATH+"/Prancing_Pony/corridor2", 1);
    }
    set_status(query_status()); /* initialize correct description */
}
