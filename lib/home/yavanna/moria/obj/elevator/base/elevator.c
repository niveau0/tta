inherit "/base/room";

#include <config.h>
#include <filepath.h>
#include <macros.h>
#include <properties.h>
#include <room.h>
#include <event.h>
#include "../../../include/moria.h"

private static object *elevator_doors; /* list of doors inside the room */
private static mapping other_doors;
private string *outer_rooms; /* to load the rooms */
private string winch_object;
private int move_speed; /*  moving speed in inch per 10 second */
private int move_direction; /* -1= down, 0= halted, 1= up */

int query_to_move(int direction);
void reload_rooms(mixed *data);
mixed* query_rooms();


/* TODO .o file erstellen   */

static void
create_elevator()
{

}

int
check_elevator()
{
    return 1;
}

void
set_move_speed(int speed)
{
    move_speed = speed;
}

int
get_move_speed()
{
    return move_speed;
}


void
set_short(string short)
{
    room_short = short;
}

void
set_long(string long)
{
    room_long = long;
}


static void
create_room()
{
    int i;

    move_direction = 0;
    elevator_doors = ({ });
    other_doors = ([ ]);
    outer_rooms = ({ });
    move_speed = 10;
    create_elevator();
/*    add_reload("query_rooms","reload_rooms");
    add_prop(ELEVATOR_IS, 1);

    for(i=0;i<sizeof(elevator_door);i++)
	call_out("update_exits",5, elevator_door[i]);
    if(outer_rooms)
    {
	for(i=0;sizeof(outer_rooms);i++)
	    outer_rooms[i]->load();
	    }
	    
    if(IS_CLONE)
    save();
    */
}


void
stop_move()
{
    int i,j, door_id, coords, *index1, *index2;
    mapping *doors;
    
    tell_objects(LISTEN(this_object()),
		 "The elevator stops to move.\n", 0);

    coords = query_coordinates()[Z_COORD];
    for(i=0;i<sizeof(elevator_doors);i++)
    {
	if(elevator_doors[i]->query_coordinates()[Z_COORD] == coords)
	{
	    doors = other_doors[elevator_doors[i]->query_door_id()];
	    if(index2 = map_indices(doors[EL_OUTER_DOORS]))
	    {
		for(j=0;j<sizeof(index2);j++)
		{
		    door_id = elevator_doors[i]->query_door_id();
		    doors[EL_OUTER_DOORS][index2[j]]->
			set_connection(this_object(),door_id);
		    elevator_doors[i]->
			set_connection(E(doors[EL_OUTER_DOORS][index2[j]]),
				       door_id);
		    elevator_doors[i]->do_open();
		    doors[EL_OUTER_DOORS][index2[j]]->do_open();
		}
	    }
	}
    }
    /* todo: update exits */
}

void
do_move(int to_move)
{
    
    int   i, *coords, movement;
    object *inv;
    
    if(!to_move)
	return;

    movement = MIN(move_speed, ABS(to_move));
    movement = movement * move_direction;
    
    tell_objects(LISTEN(this_object()),
		 "The elevator moves.\n", 0);
    
    room_coords[2] = room_coords[2] + movement;
    to_move = to_move - movement;
    
    inv = this_object()->inventory();
    
    for(i=0;i<sizeof(inv);i++)
    {
	coords = inv[i]->query_coordinates();
	coords[2] = coords[2] + movement;
	inv[i]->set_coordinates(coords);
    }    
    if(to_move != 0)
	call_out("do_move", 10, to_move);
    else
/*	for(i=0;i<sizeof(elevator_door);i++) */
	call_out("stop_move",5);
}

void
start_move(int direction)
{
    int i, next_z, tmp_z;
    object other;
    
    remove_call_out("do_move");
    
    for(i=0;i<sizeof(elevator_doors);i++)
    {
	elevator_doors[i]->do_close();
	if(other = elevator_doors[i]->query_other_door())
	    other->do_close();
	/* ToDo: get the z_coordinates of the next halt */
	/*
	tmp_z=outer_doors[i]->get_next_room_level(room_coords[2],direction);
	(elevator_door[i]->query_elevator_door())->do_close();
	if((tmp_z<next_z) || (next_z == 0))
	next_z = tmp_z;
	*/
    }
    move_direction = direction;
    if(move_direction)
    {
	call_out("do_move", 3, query_to_move(move_direction));
    }
    else
	for(i=0;i<sizeof(elevator_doors);i++)
	{
	    elevator_doors[i]->do_open();
	    if(other = elevator_doors[i]->query_other_door())
		other->do_open();
	}
}

void
set_winch(string object_path)
{
    winch_object = object_path;
}

string
query_winch()
{
    return winch_object;
}

void
set_direction(int direction)
{
    move_direction = direction;
}


int
query_direction()
{
    return move_direction;
}

object*
query_elevator_doors()
{
    return elevator_doors;
}

void
add_elevator_door(object e_door)
{
    int i;

    if((elevator_doors += ({ e_door })) &&
       !(other_doors[e_door->query_door_id()]))
	other_doors[e_door->query_door_id()] = ({ ([ ]), ([ ]) });
}

void
remove_elevator_door(object e_door)
{
    elevator_doors -= ({ e_door });
    other_doors[e_door->query_door_id()] = 0;
}

mapping
query_outer_doors(int door_id)
{
    if(!door_id || !other_doors[door_id])
	return ([ ]);
    return other_doors[door_id][EL_OUTER_DOORS];
}

void
add_outer_door(object new_door)
{
    int coord, door_id;
    mapping *doors;
    
    
    door_id = new_door->query_door_id();
    coord = E(new_door)->query_coordinates()[Z_COORD];
    if((other_doors[door_id]) &&
       (other_doors[door_id][EL_OUTER_DOORS][coord]))
	other_doors[door_id][EL_OUTER_DOORS][coord] += ({ new_door });
    else
    {
	if(!other_doors[door_id])
	    other_doors[door_id] = ({ ([ ]), ([ ]) });
	other_doors[door_id][EL_OUTER_DOORS][coord] = ({ new_door });
    }
    outer_rooms += ({ E(new_door)->query_room_file() });
}

void
remove_outer_door(object del)
{
    int coord, door_id;
    mapping *doors;
    
    door_id = del->query_door_id();
    coord = E(del)->query_coordinates()[Z_COORD];
    if((other_doors[door_id]=doors) &&
       (doors[EL_OUTER_DOORS][coord]))
	doors[EL_OUTER_DOORS][coord] -= ({ del });
    outer_rooms -= ({ E(del)->query_room_file() });
}

mapping
query_inner_doors(int door_id)
{
    if(!door_id || !other_doors[door_id])
	return ([ ]);
    return other_doors[door_id][EL_INNER_DOORS];
}

void
add_inner_door(object new_door)
{
    int coord, door_id;
    mapping *doors;
    
    door_id = new_door->query_door_id();
    coord = E(new_door)->query_coordinates()[Z_COORD];
    if((other_doors[door_id]) &&
       (other_doors[door_id][EL_INNER_DOORS][coord]))
	other_doors[door_id][EL_INNER_DOORS][coord] += ({ new_door });
    else
    {
	if(!other_doors[door_id])
	    other_doors[door_id] = ({ ([ ]), ([ ]) });
	other_doors[door_id][EL_INNER_DOORS][coord] = ({ new_door });
    }
    outer_rooms += ({ E(new_door)->query_room_file() });
}

void
remove_inner_door(object del)
{
    int coord, door_id;
    mapping *doors;
    
    door_id = del->query_door_id();
    coord = E(del)->query_coordinates()[Z_COORD];
    if((other_doors[door_id]=doors) &&
       (doors[EL_INNER_DOORS][coord]))
	doors[EL_INNER_DOORS][coord] -= ({ del });
    outer_rooms -= ({ E(del)->query_room_file() });
}

mixed*
query_rooms()
{
    return outer_rooms;
}


int
query_to_move(int direction)
{
    int i, rval, outer_coords, coords, *index1, *index2;
    mapping *doors;
    
    if(!direction)
	return 0;

    coords = query_coordinates()[Z_COORD];
    rval = 0;
    index1 = map_indices(other_doors);
    for(i=0;i<sizeof(index1);i++)
    {
	doors = other_doors[index1[i]];
	if(index2 = map_indices(doors[EL_OUTER_DOORS]))
	{
	    if(sizeof(index2))
	    {
		outer_coords = index2[0];
		if(direction == DIRECTION_UP)
		    if(outer_coords == MAX(coords, outer_coords))
			if(rval == 0)
			    rval = (outer_coords - coords);
			else
			    rval = MIN(rval, (outer_coords - coords));
		if(direction == DIRECTION_DOWN)
		    if(outer_coords == MIN(coords, outer_coords))
			if(rval == 0)
			    rval = -(ABS(outer_coords) - ABS(coords));
			else
			    rval = MIN(rval,-(ABS(outer_coords)-ABS(coords)));
	    }
	}
    }
    return rval;
}

void
reload_rooms(mixed *data)
{
    int i;

    outer_rooms = data[0];
    other_doors = ([ ]);
    elevator_doors = ({ });
    for(i=0;i<sizeof(outer_rooms);i++)
	_ROOMD->load_room(outer_rooms[i]);
}


    

