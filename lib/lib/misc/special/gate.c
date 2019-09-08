/*
 * This file can be inherited and will simulate a gate that needs an exit
 * to stand on. The gate behaves like a door when closed, but in an open state
 * you can just walk through the exits it is belonging to without the
 * necessity for an enter-cmd.
 */


inherit "/base/object";
inherit "/system/parse";
inherit "/base/misc/openclose";

#include <composite.h>
#include <door.h>
#include <event.h>
#include <filepath.h>
#include <language.h>
#include <macros.h>
#include <material.h>
#include <room.h>
#include <skills.h>
#include <properties.h>

/* in cases, where a closed gate means only opaque, but passable
 * this might become useful for quick changes.
 */
#define CLOSED_EXIT_STATE EXIT_CLOSED

nomask object find_other_door();
static void   set_exits(string exit_one, string exit_two, int id);
nomask void   set_status(int st);
mixed         query_current_exit();
nomask string query_other_room();
nomask int    query_status();

private static int    door_id, exit_dir;
private static string other_room, room_desc, open_pre, close_pre, exit1, exit2;
private object obj_other_room, other_door;

static object
create_gate()
{
    set_name("gate");
    add_adj("wooden");
    set_short("wooden gate");
    set_pshort("wooden gatess");
    set_long("A huge wooden gate.\n");
    set_status(DOOR_OPEN);
    add_lwvs(0, 50000, 100000, 80);
    add_prop(OBJ_MATERIAL, M_OAK);
}

static nomask void
create_object()
{
    open_pre = "open";
    close_pre = "closed";
    add_prop(OBJ_NO_SHOW, 1);
    add_prop(OBJ_NO_GET, 1);
    create_gate();
    if (IS_CLONE) /* to prevent the master_object from listening to events */
	add_event(E_MOVE, "gate_move", this_object());
}

/* This function will actualize the gates exits after the first movement. */
nomask void
gate_move(object ob, object from, object to)
{
    int *coord;
    mapping exits;

    if (to->query_room_file() ==
	FPATH(to->query_room_file() + "/..", exit1))
	other_room = exit2;
    else
	other_room = exit1;

    if (exits = to->query_exits())
    {
	if (exits[other_room])
	    exit_dir = exits[other_room][EXIT_DIR];
	else
	{
	    this_player()->catch_tell("Exit for gate not found.\n");
	    return;
	}
    }
    if (query_status() != DOOR_OPEN)
    {
	obj_other_room = _ROOMD->load_room(query_other_room());
	/* is there already the twindoor in the other room? */
	if (!(other_door = find_other_door()))
	{
	    other_door = clone_object(program_name(ob));
	    remove_event(E_MOVE, "gate_move", other_door);
	    if (!(coord = query_coordinates()))
		return;
	    switch (query_current_exit()[0])
	    {
	      case 'u':
		  coord[2] += 1;break;
	      case 'd':
		  coord[2] -= 1;break;
	      case 'n':
		  coord[1] += 1;break;
	      case 's':
		  coord[1] -= 1;break;
	      case 'w':
		  coord[0] -= 1;break;
	      case 'e':
		  coord[0] += 1;break;
	      default:
		  ;
	    }
	    /* To prevent the new cloned door from cloning a further door. */
	    remove_event(E_MOVE, "gate_move", other_door);

	    other_door->set_coordinates(coord);
	    other_door->move(obj_other_room);
	}
	to->set_exit_status(query_current_exit()[1], CLOSED_EXIT_STATE);
	
	/* this line prevents set_exit_status() from causing an eternal
	   loop, as it contains a load room_room() command */
	_ROOMD->add_room(to->query_room_file(), to);
	obj_other_room->set_exit_status(other_door->query_current_exit()[1],
					CLOSED_EXIT_STATE);
	_ROOMD->remove_room(to->query_room_file());
	/* both stati have to be the same */
	other_door->set_status(query_status());
    }
    remove_event(E_MOVE, "gate_move", ob);
}

/* Function name: set_exits
 * Description:   sets the exits that belong to a gate and its id
 * Arguments:     exit_one - the filename of the first room (may be
 *                           relative to room where gate stands)
 *                exit_two - the filename of the second room (may be
 *                           relative to room where gate stands)
 */
static void
set_exits(string exit_one, string exit_two, int id)
{
    exit1 = exit_one;
    exit2 = exit_two;
    door_id = id;
}

/* Function name: query_current_exit
 * Description:   returns the exit direction and name the room it leads to
 * Returns:       ({
 *                   an integer as exit direction,
 *                   a string as room name
 *                 })
 */
mixed
query_current_exit()
{
    return ({ exit_dir, other_room });
}

/*
 * Function name: set_open_close_prefix
 * Description:   set description prefixes
 * Arguments:     o - the open description prefix, defaults to "open"
 *                c - the closed description prefix, defaults to "closed"
 */
nomask void
set_open_close_prefix(string o, string c)
{
    open_pre = o;
    close_pre = c;
}

/*
 * Function name: room_desc
 * Description:   gives the description added to the rooms description
 * Returns:       string
 */
nomask string
room_desc()
{
    return room_desc;
}

/*
 * Function name: query_door_id
 * Description:   gives door id, must be unique within each room and
 *                must equal the connected door's id (if there is one).
 * Returns:       int
 */
nomask int
query_door_id()
{
    return door_id;
}

/*
 * Function name: query_other_room
 * Description:   give filename of connected room
 * Returns:       string
 */
nomask string
query_other_room()
{
    return FPATH(E(this_object())->query_room_file() + "/..", other_room);
}

/*
 * Function name: set_status 
 * Description:   set door status (open, locked, etc.)
 * Arguments:     st - status (see <door.h>)
 */
nomask void
set_status(int st)
{
    if (st == DOOR_OPEN)
    {
	room_desc = capitalize(LANG_ADDART((open_pre ? open_pre + " " : "") +
					   ::short()));
	add_prop(CONT_LOCKED, 0);
	add_prop(CONT_CLOSED, 0);
    }
    else
    {
	room_desc = capitalize(LANG_ADDART((close_pre ? close_pre +" " : "") +
					   ::short()));
	if (st == DOOR_LOCKED)
	    add_prop(CONT_LOCKED, 1);
	add_prop(CONT_CLOSED, 1);
    }
}

/*
 * Function name: query_status
 * Description:   give actual door status
 * Returns:       integer
 */
nomask int
query_status()
{
    if (query_prop(CONT_LOCKED))
	return DOOR_LOCKED;
    if (query_prop(CONT_CLOSED))
	return DOOR_CLOSED;
    return DOOR_OPEN;
}

/*
 * Function name: find_other_door
 * Description:   find the connected door
 * Returns:       object
 */
nomask object
find_other_door()
{
    int    i;
    string room;
    object r, *all;
    
    if (other_door)
	return other_door;
    if (!(r = _ROOMD->load_room(query_other_room())))
	return 0;
    all = I(r, 4);
    room = E(this_object())->query_room_file();
    for (i=sizeof(all); i--;)
    {
	if (all[i]->query_door_id() == door_id &&
	    all[i]->query_other_room() == room)
	{
	    other_door = all[i];
	    break;
	}
    }
    return other_door;
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
    if ((query_status() != DOOR_OPEN) &&
	!find_other_door())
    {
	tp->catch_tell("Failed, the other side doesn't exist.\n");
	return -1;
    }
    if (::open_cmd(str, tp) == -1)
	return -1;
    /* exit should still be closed */
    tell_objects(LISTEN(E(other_door)),
		 ({ QCTNAME(other_door),
			" is opened from the other side.\n" }), MSG_SEE,
			other_door);
    /* opening the two sides of the exit */
    environment()->set_exit_status(query_current_exit()[1], EXIT_OPEN);
    set_status(DOOR_OPEN);
    E(other_door)->set_exit_status(other_door->query_current_exit()[1],
				   EXIT_OPEN);
    remove_object(other_door);
    other_door = 0;
    return 1;
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
    int *coord;
    object temp_env;
    
    if (::close_cmd(str, tp) == -1)
	return -1;

    obj_other_room = _ROOMD->load_room(query_other_room());

    if (other_door = find_other_door())
	tp->catch_tell("Warning, second open gate already exists.\n");
    else
	other_door = clone_object(program_name(this_object()));

    coord = query_coordinates();
    switch (query_current_exit()[0])
    {
      case 'u':
	  coord[2] += 1;break;
      case 'd':
	  coord[2] -= 1;break;
      case 'n':
	  coord[1] += 1;break;
      case 's':
	  coord[1] -= 1;break;
      case 'w':
	  coord[0] -= 1;break;
      case 'e':
	  coord[0] += 1;break;
      default:
	  ;
    }
    /* To prevent the new door from doing strange things upon moving */
    other_door->set_status(DOOR_OPEN);

    other_door->set_coordinates(coord);
    other_door->move(obj_other_room);
    environment()->set_exit_status(query_current_exit()[1],
				   CLOSED_EXIT_STATE);
    set_status(DOOR_CLOSED);
    E(other_door)->set_exit_status(other_door->query_current_exit()[1],
				   CLOSED_EXIT_STATE);
    other_door->set_status(DOOR_CLOSED);
    return 1;
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
    if (!find_other_door())
    {
	tp->catch_tell("Failed, the other side doesn't exist.\n");
	return -1;
    }
    if (::lock_cmd(str, tp) == -1)
	return -1;
    tell_objects(LISTEN(E(other_door)),
		 ({ QCTNAME(other_door),
		    " is locked from the other side.\n" }),
		 MSG_SEE, other_door);
    other_door->set_status(DOOR_LOCKED);
    set_status(DOOR_LOCKED);
    return 1;
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
    if (!find_other_door())
    {
	tp->catch_tell("Failed, the other side doesn't exist.\n");
	return -1;
    }
    if (::unlock_cmd(str, tp) == -1)
	return -1;
    tell_objects(LISTEN(E(other_door)),
		 ({ QCTNAME(other_door),
		    " is unlocked from the other side.\n" }), MSG_SEE,
		 other_door);
    other_door->set_status(DOOR_UNLOCKED);
    set_status(DOOR_UNLOCKED);
    return 1;
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
    if (!find_other_door())
    {
	tp->catch_tell("Failed, the other side doesn't exist.\n");
	return -1;
    }
    if (::unlock_cmd(str, tp) == -1)
	return -1;
    tell_objects(LISTEN(E(other_door)),
		 ({ QCTNAME(other_door),
			" is unlocked from the other side.\n" }), MSG_SEE,
			other_door);
    other_door->set_status(DOOR_UNLOCKED);
    set_status(DOOR_UNLOCKED);
    return 1;
}
