inherit "/base/room";

#include <filepath.h>
#include <macros.h>
#include <properties.h>
#include <room.h>
#include <event.h>
#include "../../../include/moria.h"

#define ROOM_LONG \
"A room nearly square in shape. You can see a winch here.\n"
#define ROOM_SHORT \
"A elevator"


private string *elevator_door; /* object-path(MASTER)*/
private string winch_object;
private int move_speed; /*  moving speed in inch/10 sec. */
private int move_direction;  /* down: -1, halted: 0 , up: 1*/
private string *outer_rooms; /* for loading the other doors when needed */


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
    elevator_door = ({ });
    room_long = ROOM_LONG;
    room_short = ROOM_SHORT;
    move_speed = 10;
    xsize = 100;
    ysize = 100;
    light = 100;
    type = TYPE_INSIDE;
    climate = 1;
    height = 100;
    room_coords =({-9999999, -9999999, 9999950 });
    room_file = MORIA_OBJ+"elevator/elevator.o";
    create_elevator();
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
}

void
add_elevator_door(string door_master)
{
    elevator_door += ({ door_master });
}

int
remove_elevator_door(string door_master)
{
    if(elevator_door -= ({ door_master }))
	return 1;
    return 0;
}
string*
query_elevator_door()
{
    return elevator_door;
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
update_exits(string door_master)
{
    door_master->reconnect(room_coords[2]);   
}

void
stop_move(string door_master)
{
    tell_objects(LISTEN(this_object()),
		 "The elevator stops to move.\n", 0); 
    call_out("update_exits", 2, door_master);
}

int
query_direction()
{
    return move_direction;
}


void
do_move(int next_stop)
{
    
    int   i, *coords, movement;
    object *inv;

    movement = MIN(move_speed, ABS(next_stop));
    movement = movement * move_direction;
    
    if(!next_stop)
	return;
    
    tell_objects(LISTEN(this_object()),
		 "The elevator moves.\n", 0);
    
    room_coords[2] = room_coords[2] + movement;
    next_stop = next_stop - movement;
    
    inv = inventory(this_object());
    
    for(i=0;i<sizeof(inv);i++)
    {
	coords = inv[i]->query_coordinates();
	coords[2] = coords[2] + movement;
	inv[i]->set_coordinates(coords);
    }    
    if(next_stop != 0)
	call_out("do_move", 3, next_stop);
    else
	for(i=0;i<sizeof(elevator_door);i++)
	    call_out("stop_move",5, elevator_door[i]);
}

void
start_move(int direction)
{
    int i, next_z, tmp_z;

    remove_call_out("do_move");
    
    for(i=0;i<sizeof(elevator_door);i++)
    {
	tmp_z=elevator_door[i]->get_next_room_level(room_coords[2],direction);
	(elevator_door[i]->query_elevator_door())->do_close();
	if((tmp_z<next_z) || (next_z == 0))
	    next_z = tmp_z;
    }
    next_z -= room_coords[2];
    move_direction = direction;
    if(move_direction)
    {
	call_out("do_move", 3, next_z);
    }
    else
	for(i=0;i<sizeof(elevator_door);i++)
	    (elevator_door[i]->query_elevator_door())->do_open();
}

void
add_outer_room(string room)
{
    outer_rooms += ({ room });
    save();
    debug("yavanna","test");
}

void
remove_outer_room(string room)
{
    outer_rooms -= ({ room });
}

string*
query_outer_rooms()
{
    return outer_rooms;
}

