
inherit "/base/object";

#include <config.h>
#include <filepath.h>
#include <macros.h>
#include <properties.h>
#include <event.h>
#include <material.h>
#include <room.h>
#include "../../include/moria.h"
#include "../../include/elevator.h"

mapping elevator_data; // All Data of elevators, keyed by
                               // elevator-room-filepath
static mapping elevator_doors; // The Elevator_doors, keyed like data

/*
 * elevator-data:
 * key: filepath of elevator-room
 *
 * data:
 *       int move_speed <- moving speed of the elevator in inch per 10 sec.
 *       int move_direction <- -1=moving down, 0=halted, 1=moving up
 *       int elevation <- current elevation (z-coord) of the elevator
 *       string winch_obj <- path to obj to control/start movement
 *       string *rooms <- pathes to rooms that have an access to the elevator
 */

/*
 * elevator_doors:
 * key: filepath of elevator-room
 *
 * data:
 *       mixed *doors <- Array of Doors: Pos 0 <- door-objects inside the
 *                                                elevator(mapping)
 *                                       Pos 1 <- mapping outer_doors
 *                         
 */

/*
 * outer_doors:
 * key: elevation of Door-Object (Z-Coord)
 *
 * data: object *doors <- Array of all doors with key-elevation
 *
 */
/*
 * inner_doors:
 * key: Door_id
 *
 * data: object *doors <- Array of all doors with key-id
 *
 */
  


int add_elevator_data(string elevator, int what, mixed data);
void load();
void save();
string file_name();
object load_room(string roompath);
int load_rooms(string elevator);

void
create_elevator_master()
{
    object ob;
    
    elevator_data = ([ ]);
    elevator_doors = ([ ]);
    
    if(!IS_CLONE && (ob = find_player("yavanna")))
	ob->catch_tell("Elevator-Master initialised!\n");
}

void
create_object()
{
    object room;
    
    create_elevator_master();
    
    set_lwvs(0,100,100,100);
    add_prop(OBJ_MATERIAL, M_STONE);
    set_name("statue");
    set_pname("statues");
    set_short("statue");
    set_pshort("statues");
    add_adj("small");
    add_adj("stone");
    set_long("A small statue, made from stone.\n"+
		"You wounder what it is good for.\n"); 
    load();
}

void
init_elevator(string elevator)
{
    object ob;

    ob = load_room(elevator);
    
    if(!elevator_data[elevator])
	elevator_data[elevator] = ({ 10,   // default move speed 10 inch/sec.
				     0,    // default direction halted
				     (ob->query_coordinates())[Z_COORD],
				     "",   // default winch object
				     ({ }) // default outer rooms
	}); 
    if(!elevator_doors[elevator])
	elevator_doors[elevator] = ({ ([ ]),// doors inside the elevator
				      ([ ]) // doors in the outer rooms
	});
}

int
query_is_new(string elevator)
{
    if(elevator_data[elevator] && elevator_doors[elevator])
	return 0;
    return 1;
}

void
new_elevator(object elevator)
{
    string rpath;
    rpath = elevator->query_room_file();
    init_elevator(rpath);
    add_elevator_data(rpath, ELEVATION,
		      (elevator->query_coordinates())[Z_COORD]);
}
	
mixed
query_elevator_data(object elevator)
{
    string rpath;
    if(!elevator || !(elevator->query_prop(IS_ELEVATOR)))
	return 0;
    rpath = elevator->query_room_file();
    if(query_is_new(rpath))
	new_elevator(elevator);
    return elevator_data[rpath];
}

mixed
query_elevator_doors(object elevator)
{
    string rpath;
    if(!elevator || !(elevator->query_prop(IS_ELEVATOR)))
	return 0;
    rpath = elevator->query_room_file();
    if(query_is_new(rpath))
	new_elevator(elevator);
    return elevator_doors[rpath];
}

int
set_elevator_data(string elevator, mapping data)
{
    if(!elevator)
	return 0;
    elevator_data[elevator] = data;
    return 1;
}

int
remove_elevator_door(string elevator, object door)
{
    int el, id;
    if(query_is_new(elevator))
	return 0;
    id = door->query_door_id();
    if(elevator_doors[elevator][DOORS_INNER][id])
	if(IN_ARRAY(door, elevator_doors[elevator][DOORS_INNER][id]))
	elevator_doors[elevator][DOORS_INNER][id] -= ({ door });
    el = door->query_elevation();
    if(elevator_doors[elevator][DOORS_OUTER][el])
	if(IN_ARRAY(door, elevator_doors[elevator][DOORS_OUTER][el]))
	    elevator_doors[elevator][DOORS_OUTER][el] -= ({ door });
    return 1;
}

int
add_elevator_door(string elevator, int what, object door)
{
    int el, id;
    
    if(query_is_new(elevator))
	init_elevator(elevator);
    switch(what)
    {
      case INSIDE_DOORS:
	  id = door->query_door_id();
	  if(elevator_doors[elevator][DOORS_INNER][id])
	  {
	      if(!IN_ARRAY(door, elevator_doors[elevator][DOORS_INNER][id]))
		  elevator_doors[elevator][DOORS_INNER][id] += ({ door });
	  }
	  else
		  elevator_doors[elevator][DOORS_INNER][id] = ({ door });
	  return 1;
      case OUTSIDE_DOORS:
	  el = door->query_elevation();
	  if(elevator_doors[elevator][DOORS_OUTER][el])
	  {
	      if(!IN_ARRAY(door, elevator_doors[elevator][DOORS_OUTER][el]))
		  elevator_doors[elevator][DOORS_OUTER][el] += ({ door });
	  }
	  else
	      elevator_doors[elevator][DOORS_OUTER][el] = ({ door });
	  return 1;
      default:
	  return 0;
    }    
}

int
add_elevator_data(string elevator, int what, mixed data)
{
    int el;

    if(query_is_new(elevator))
	init_elevator(elevator);
    switch(what)
    {
      case MOVE_SPEED:
      case MOVE_DIRECTION:
      case ELEVATION:
      case WINCH_OBJ:
	  elevator_data[elevator][what] = data;
	  break;
      case OUTSIDE_ROOMS:
	  if(!IN_ARRAY(data, elevator_data[elevator][what]))
	      elevator_data[elevator][what] += ({ data });
	  break;
      case INSIDE_DOORS:
      case OUTSIDE_DOORS:
	  if(!(add_elevator_door(elevator, what, data)))
	      return 0;
	  break;
      default:
	  return 0;
    }
    save();
    return 1;
}

mixed
query_data(object elevator, int what)
{
    string rpath;
    
    if(!elevator || !what)
	return 0;
    rpath = elevator->query_room_path();
    
    if(query_is_new(rpath))
	new_elevator(elevator);
 
    switch(what)
    {
      case MOVE_SPEED:
      case MOVE_DIRECTION:
      case ELEVATION:
      case WINCH_OBJ:
      case OUTSIDE_ROOMS:
	  return elevator_data[rpath][what];
      case INSIDE_DOORS:
	  return elevator_doors[rpath][DOORS_INNER]; 
      case OUTSIDE_DOORS:
	  return elevator_doors[rpath][DOORS_OUTER]; 
      default:
	  return 0;
    }
}

string
file_name()
{
    return MORIA_OBJ + "master/elevator_master";
}


void
save()
{
    string file;

    file = file_name();
    rename_file(file + ".o", file + ".bak");
    save_object(file);
    remove_file(file + ".bak");
}

void
load()
{
    restore_object(file_name());
}

    
string*
query_elevators()
{
    return map_indices(elevator_data);
}

object
load_room(string roompath)
{
    return _ROOMD->load_room(roompath);
}

int
load_rooms(string elevator)
{
    int i;
    
    if(!elevator_data[elevator])
	return 0;
    for(i=0;i<sizeof(elevator_data[elevator][OUTSIDE_ROOMS]);i++)
	load_room(elevator_data[elevator][OUTSIDE_ROOMS][i]);
    return 1;
}

int
query_inch_to_move(object elevator, int direction)
{
    int i, rval, el_elevation, d_elevation, *door_elevations;
    string rpath;
    
    if(!elevator || !direction)
	return 0;
    rpath = elevator->query_room_file();
    if(!elevator_doors[rpath])
	return 0;
    rval = 0;
    el_elevation = elevator->query_coordinates()[Z_COORD];
    door_elevations = map_indices(elevator_doors[rpath][OUTSIDE_DOORS]);
    for(i=0;i<sizeof(door_elevations);i++)
    {
	d_elevation = door_elevations[i];
	if(direction == DIRECTION_UP)
	    if(d_elevation == MAX(el_elevation, d_elevation))
		if(rval == 0)
		    rval = (d_elevation - el_elevation);
		else
		    rval = MIN(rval, (d_elevation - el_elevation));
	if(direction == DIRECTION_DOWN)
	    if(d_elevation == MIN(el_elevation, d_elevation))
		if(rval == 0)
		    rval = -(ABS(d_elevation) - ABS(el_elevation));
		else
		    rval = MAX(rval, -(ABS(d_elevation) - ABS(el_elevation)));
    }
    return rval;
}


void
end_move(object elevator)
{
    int i, elevation, id;
    string rpath;
    object *inner_doors;
    mapping outer_doors;
    
    if(!elevator)
	return;
    rpath = elevator->query_room_file();
    if(!query_data(elevator, MOVE_DIRECTION))
	return;
    add_elevator_data(rpath, MOVE_DIRECTION, ELEVATOR_HALTED);
    
    tell_objects(LISTEN(elevator),
		 "The elevator stops to move.\n", 0);
    inner_doors = elevator_doors[rpath][DOORS_INNER];
    outer_doors = elevator_doors[rpath][DOORS_OUTER];
    elevation = elevator->query_coordinates()[Z_COORD];

    for(i=0;i<sizeof(outer_doors[elevation]);i++)
    {
	id = outer_doors[elevation][i]->query_door_id();
	if(!inner_doors[id])
	    return;
	inner_doors[id]->set_connection(E(outer_doors[elevation]), id);
	outer_doors[elevation]->set_connection(E(inner_doors[id]), id);
	inner_doors[id]->do_open();
	outer_doors[elevation]->do_open();
    }
}


void
do_move(object elevator, int still_missing)
{
    int movement, *coords, i;
    string rpath;
    object *inv;
    
    
    if(!still_missing || !(query_data(elevator, MOVE_DIRECTION)))
	return;
    rpath = elevator->query_room_file();
    
    movement = MIN(query_data(elevator, MOVE_SPEED), still_missing);
    movement *= query_data(elevator, MOVE_DIRECTION);
    still_missing -= movement;
    
    tell_objects(LISTEN(this_object()),
		 "The elevator moves.\n", 0);
    coords = elevator->query_coordinates();
    coords[Z_COORD] += movement;
    elevator->set_coordinates(coords);
    inv = elevator->inventory();
    for(i=0;i<sizeof(inv);i++)
    {
	coords = inv[i]->query_coordinates();
	coords[Z_COORD] = coords[2] + movement;
	inv[Z_COORD]->set_coordinates(coords);
    }
    if(still_missing)
	call_out("do_move", 10, elevator, still_missing);
    else
	call_out("end_move", 10, elevator);
}


void
try_start_move(string elevator, int direction)
{
    int i,j, *door_index, to_move;
    object ob, other;
    mapping inner_doors;
    
    if(!load_rooms(elevator)|| !elevator_doors[elevator])
	return;
    ob = load_room(elevator);
    to_move = query_inch_to_move(ob, direction);
    if(!to_move)
	return;
    
    inner_doors = elevator_doors[elevator][DOORS_INNER];
    door_index = map_indices(inner_doors);

    for(j=0;j<sizeof(door_index);i++)
	for(i=0;i<sizeof(elevator_doors[elevator][DOORS_INNER][j]);i++)
	{
	   inner_doors[j][i]->do_close();
	    if(other = inner_doors[j][i]->query_other_door())
		other->do_close();
	}
    add_elevator_data(elevator, MOVE_DIRECTION, direction);
    if(direction)
	call_out("do_move", 3, ob, to_move );
    
}


void
try_stop_move(string elevator, int direction)
{
    object ob;
    
    if(elevator_data[elevator][MOVE_DIRECTION])
    {
	ob = load_room(elevator);
	call_out("end_move", 2, ob);
    }
}

