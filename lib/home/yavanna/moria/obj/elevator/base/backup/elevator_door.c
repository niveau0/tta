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

private static object *outer_doors, *inner_doors, elevator_door;

static void set_direction_desc(string desc);
nomask void set_status(int st);
nomask void add_inner_door(object ob);
nomask void add_outer_door(object ob);
nomask void add_elevator_door(object ob);
nomask void enter_env(object ob, object from, object to);   

int
check_elevator_door()
{
    return 1;
}


static void
create_elevator_door()
{

}


static void
create_door()
{    
    set_name("door");
    add_adj("stone");
    set_short("stone door");
    set_pshort("stone doors");
//    set_direction_desc("");
    set_long("A two-winged stone door.\n");
    set_status(DOOR_CLOSED);
    add_prop(OBJ_MATERIAL,M_STONE);
    
    set_connection("/base/room", 1); /* environment of door, door id */
    add_lwvs(0, 50000, 100000, 80);
    outer_doors = ({ });
    inner_doors = ({ });
    
    create_elevator_door();
//    add_prop(ELEVATOR_DOOR_IS,1);

    if(IS_CLONE)
	add_event(E_MOVE, "enter_env", this_object());
}

nomask void
enter_env(object ob, object from, object to)
{
    object ob2;
    
    if (!to || ob != this_object() || living(to))
       return;
   
   if(E(this_object())->check_elevator() == ELEVATOR_DUCT_IS)
	   MASTER->add_inner_door(this_object());
   else if(E(this_object())->check_elevator() == ELEVATOR_IS) 
	   MASTER->add_elevator_door(this_object());
   else
   {    
       MASTER->add_outer_door(this_object());
       if(ob2 = E(MASTER->query_elevator_door()))
	   ob2->add_outer_room(E(this_object())->query_room_file());
   }
   
   if(MASTER->query_elevator_door())
       MASTER->reconnect(elevator_door->query_coordinates()[2]);
   remove_event(E_MOVE, "enter_env", this_object());
   
}

nomask void
add_inner_door(object ob)
{
    inner_doors += ({ ob });
}

nomask void
add_outer_door(object ob)
{
    outer_doors += ({ ob });
}

nomask void
add_elevator_door(object ob)
{
    elevator_door = ob;
    E(ob)->add_elevator_door(MASTER_OB(ob));
}

nomask void
remove_door(object ob)
{
    if(ob == elevator_door)
    {
	elevator_door = 0;
	E(ob)->remove_elevator_door(MASTER_OB(ob));
    }
    inner_doors -= ({ ob });
    outer_doors -= ({ ob });
}
 

object*
query_inner_doors()
{
    if(IS_CLONE)
	return ::MASTER->query_inner_doors();
    return inner_doors;
}

object*
query_outer_doors()
{
    if(IS_CLONE)
	return ::MASTER->query_outer_doors();
    return outer_doors;
}

object
query_elevator_door()
{
    if(IS_CLONE)
	return ::MASTER->query_elevator_door();
    return elevator_door;
}

object*
query_inner_rooms()
{
    int i;
    object *inner_rooms;

    inner_rooms = ({ });

    for(i=0;i<sizeof(inner_doors);i++)
	inner_rooms += ({ E(inner_doors[i])->query_room_file() });
    
    return inner_rooms;
}

string*
query_outer_rooms()
{
    int i;
    string *outer_rooms;
    
    outer_rooms = ({ });
    
    for(i=0;i<sizeof(outer_doors);i++)
	outer_rooms += ({ E(outer_doors[i])->query_room_file() });
    return outer_rooms;
}

string
query_elevator_room()
{
    if(elevator_door)
	return E(elevator_door)->query_room_file();
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
    MASTER->remove_door(this_object());
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
get_next_room_level(int room_z, int direction)
{
    int i, rval, room_coords, elevator_coords;

    if(!direction)
	return 0;
    if(IS_CLONE)
	return MASTER->get_next_room_level(room_z, direction);
    
    elevator_coords = (elevator_door->query_coordinates())[2];
    rval = elevator_coords;
    for(i=0;i<sizeof(outer_doors);i++)
    {
	room_coords = (outer_doors[i]->query_coordinates())[2];
	if(direction == DIRECTION_UP)
	    if(room_coords == MAX(room_coords, room_z))
		if(rval == elevator_coords)
		    rval = room_coords;
		else
		    rval = MIN(room_coords, rval);
	
	if(direction == DIRECTION_DOWN)
	    if(room_coords == MIN(room_coords, room_z))
		rval = MIN(room_coords, rval);
    }
    return rval;    
}




int
reconnect(int room_z)
{
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
	
/*	if (((z - 10) < room_z) && ((z+10) > room_z))*/
	if (z == room_z)
	{
/*	    debug("yavanna",outer_doors[i]);
	    debug("yavanna",elevator_file);
	    debug("yavanna",room_file);
	    */	    
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
    return 1;    
}

	


