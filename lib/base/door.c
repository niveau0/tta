inherit "object";
inherit "/system/parse";
inherit "misc/openclose";

#include <composite.h>
#include <config.h>
#include <door.h>
#include <event.h>
#include <filepath.h>
#include <language.h>
#include <macros.h>
#include <material.h>
#include <skills.h>
#include <properties.h>

static void set_direction_desc(string desc);
nomask void set_connection(string room, int id);
nomask void set_status(int st);

private static int     door_id;
private static string  other_room, room_desc, dir_desc, open_pre, close_pre;
private static object  other_door;

static void
create_door()
{
    set_name("door");
    add_adj("wooden");
    set_short("wooden door");
    set_pshort("wooden doors");
    set_direction_desc("leading to nowhere");
    set_long("A simple wooden door.\n");
    set_status(DOOR_LOCKED);
    set_connection("/base/room", 1); /* environment of door, door id */
    add_lwvs(0, 50000, 100000, 80);
}

static nomask void
create_object()
{
    open_pre = "open";
    close_pre = "closed";
    create_door();
    add_prop(OBJ_NO_SHOW, 1);
    add_prop(OBJ_NO_GET, 1);
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
 * Arguments:     tp - acting living
 * Returns:       string
 */
nomask string
room_desc(object tp)
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
					   ::short())) +
		    (dir_desc ? " " + dir_desc : "");
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
 * Function name: set_connection
 * Description:   set the doors connection and its id
 * Arguments:     room - room file
 *                id - door id
 */
nomask void
set_connection(string room, int id)
{
    other_door = 0;
    other_room = room;
    door_id = id;
}

/*
 * Function name: set_direction_desc
 * Description:   set a description for the direction
 * Arguments:     desc - description string
 */
static void
set_direction_desc(string desc)
{
    dir_desc = desc;
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
 * Function name: enter_cmd
 * Description:   handle enter cmd
 * Arguments:     str - command arguments
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
int
enter_cmd(string str, object tp)
{
    object env;

    if (query_status() != DOOR_OPEN)
    {
	tp->catch_tell("But it's closed.\n");
	return -1;
    }
    if (!find_other_door())
    {
	tp->catch_tell("Failed, the other side doesn't exist.\n");
	return -1;
    }
    tp->move_living(env = E(other_door), other_door->query_coordinates(),
		    "through the " + ::short(), 0);
    tell_objects(LISTEN(env) - ({ tp }),
		 ({ QCTNAME(tp), " arrives through the " + 
			other_door->short() +
			".\n" }), MSG_SEE | MSG_WALK, tp);
    return 1;
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
    if (!find_other_door())
    {
	tp->catch_tell("Failed, the other side doesn't exist.\n");
	return -1;
    }
    if (::open_cmd(str, tp) == -1)
	return -1;
    tell_objects(LISTEN(E(other_door)),
		 ({ QCTNAME(other_door),
			" is opened from the other side.\n" }), MSG_SEE,
			other_door);
    other_door->set_status(DOOR_OPEN);
    set_status(DOOR_OPEN);
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
    if (!find_other_door())
    {
	tp->catch_tell("Failed, the other side doesn't exist.\n");
	return -1;
    }
    if (::close_cmd(str, tp) == -1)
	return -1;
    tell_objects(LISTEN(E(other_door)),
		 ({ QCTNAME(other_door),
		    " is closed from the other side.\n" }), MSG_SEE,
		 other_door);
    other_door->set_status(DOOR_CLOSED);
    set_status(DOOR_CLOSED);
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
