inherit "/base/door";
/* inherit "/system/parse"; */
/* inherit "/base/misc/openclose"; */

#include <composite.h>
#include <door.h>
#include <filepath.h>
#include <language.h>
#include <macros.h>
#include <material.h>
#include <skills.h>
#include <config.h>
#include <properties.h>
#include <event.h>
#include "../../../include/moria.h"
#include "../../../include/elevator.h"

private string elevator; 

// static void set_direction_desc(string desc);
// nomask void set_status(int st);
nomask void enter_env(object ob, object from, object to);   
void set_elevator(string elevator_path);


// It is nec. to call the set_elevator - func. , because it is needed to
// add this door for update purposes and usage as an elevator-door
static void
create_elevator_door()
{
    set_elevator("/home/yavanna/room/home");
}


static void
create_door()
{    
    set_name("door");
    add_adj("stone");
    add_adj("double-winged");
    set_short("stone door");
    set_pshort("stone doors");
    set_long("A double-winged stone door.\n");
    set_status(DOOR_CLOSED);
    add_prop(OBJ_MATERIAL, M_STONE);
    add_prop(OBJ_NO_GET, 1);
    elevator = "";
    set_connection("/base/room", 1); /* environment of door, door id */
    add_lwvs(0, 50000, 100000, 80);
    create_elevator_door();
    if(IS_CLONE)
	add_event(E_MOVE, "enter_env", this_object());
}


nomask void
enter_env(object ob, object from, object to)
{
    if (!to || ob != TO() || (to->query_prop(IS_ROOM) != 1) || elevator == "")
	return;
    
    if(to->query_prop(IS_ELEVATOR))
	MORIA_ELEVATOR_MASTER->add_elevator_door(elevator,
						 INSIDE_DOORS,
						 ob);
    else
    {
	MORIA_ELEVATOR_MASTER->add_elevator_door(elevator,
						 OUTSIDE_DOORS,
						 ob);
	
 	MORIA_ELEVATOR_MASTER->add_elevator_data(elevator,
						 OUTSIDE_ROOMS,
						 to->query_room_file());
    }
    remove_event(E_MOVE, "enter_env", this_object());   
}

int
check_elevator_door()
{
    return 1;
}

void
set_elevator(string elevator_path)
{
    elevator = elevator_path;
}

string
query_elevator()
{
    return elevator;
}

/*
 * Function name: open_cmd
 * Description:   handle open cmd
 * Arguments:     str - command arguments
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
int
open_cmd(string str, object tp)
{
    return -1; /* cannot be opened normaly */
}

/*
 * Function name: close_cmd
 * Description:   handle close cmd
 * Arguments:     str - command arguments
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
int
close_cmd(string str, object tp)
{
    return -1; /* cannot be opened normaly */
}

/*
 * Function name: lock_cmd
 * Description:   handle lock cmd
 * Arguments:     str - command arguments
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
int
lock_cmd(string str, object tp)
{
   return -1; /* cannot be locked*/
}

/*
 * Function name: unlock_cmd
 * Description:   handle unlock cmd
 * Arguments:     str - command arguments
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
int
unlock_cmd(string str, object tp)
{
    return -1; /* cannot be unlocked*/
}

/*
 * Function name: pick_cmd
 * Description:   try to pick the lock
 * Arguments:     str - an optional tool used for lockpicking
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
int
pick_cmd(string str, object tp)
{
    return -1; /* cannot be picked*/
}

/* 
 * Function name: remove_object
 * Description:   Removes this object from the game.
 */
varargs void
remove_object(object ob)
{
    MORIA_ELEVATOR_MASTER->remove_elevator_door(elevator, TO());
    ::remove_object(ob);
}

do_open()
{
    if((query_status() == DOOR_CLOSED) || (query_status() == DOOR_LOCKED))
    {
    set_status(DOOR_OPEN);
    tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
		 "The " + short() + " opens.\n", MSG_SEE, this_object());

    }
}

do_close()
{
    if(query_status() == DOOR_OPEN)
    {	
    set_status(DOOR_LOCKED);
    tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
		 "The " + short() + " closes.\n", MSG_SEE, this_object());    
    }
}

int
reconnect(int room_z)
{
/*
    int i,j, z, z2, id;
    object room, elevator, room2;
    string room_file, room_file2, elevator_file;
    
    for(i=0;i<sizeof(outer_doors);i++)
    {
	room = E(outer_doors[i]);
	elevator = E(elevator_door);
	room_file = room->query_room_file();
	elevator_file = elevator->query_room_file();
	id = outer_doors[i]->query_door_id();
	
	z = (outer_doors[i]->query_coordinates())[2];
	
	if (z == room_z)
	{
	    outer_doors[i]->set_connection(elevator_file, id);
	    elevator_door->set_connection(room_file, id);    
	    outer_doors[i]->do_open();
	    elevator_door->do_open();
	}
	else
	{
	    outer_doors[i]->do_close();
	    for(j=1;j<sizeof(inner_doors);j++)
	    {
		room2 = E(inner_doors[j]);
		room_file2 = room2->query_room_file();
		
		z2 = (inner_doors[j]->query_coordinatess())[2];
		if(z == z2)
		{
		    outer_doors[i]->set_connection(room_file2, id);
		    inner_doors[j]->set_connection(room_file, id);
		    inner_doors[j]->do_close();
		}
	    }
	}
	}
	*/
    return 1;    
}

int
query_elevation()
{
    return TO()->query_coordinates()[Z_COORD];
}

	


