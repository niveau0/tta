inherit "object";

#include <event.h>
#include <filepath.h>
#include <language.h>
#include <macros.h>
#include <material.h>
#include <move.h>
#include <properties.h>

static void set_connection(string room);

private string  other_room, room_desc;
private object  other_stair;

static void
create_stair()
{
    set_name("staircase");
    add_name("stair");
    add_adj("wooden");
    set_short("wooden stair");
    set_pshort("wooden stair");
    set_long("A wooden stair.\n");
    set_connection("/base/room");
    add_prop(OBJ_MATERIAL, M_OAK);
    add_lwvs(0, 200000, 210000, 80);
}

static nomask void
create_object()
{
    create_stair();
    add_prop(OBJ_NO_SHOW, 1);
    add_prop(OBJ_NO_GET, 1);
    room_desc = capitalize(LANG_ADDART(::short()));
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
    int    part, *co, *ownco;
    
    if (!tp)
	return room_desc;
    co = tp->query_coordinates();
    ownco = query_coordinates();
    part = query_size() / 3;
    if (co[2] > ownco[2] + part * 2)
	return room_desc + " leading down";
    if (co[2] < ownco[2] + part)
	return room_desc + " leading up";
    return room_desc;
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
 * Function name: set_connection
 * Description:   set the staircase connection, id and where it leads
 * Arguments:     room - room file
 *                id - stair id
 *                dir - if the stair leads up or down <door.h>
 */
static void
set_connection(string room)
{
    other_room = room;
}

private void
_on_stair(object tp)
{
    if (tp->query_prop(UPDOWN_OBJ) != this_object())
    {
	tp->add_prop(UPDOWN_OBJ, this_object());
	add_event(E_MOVE, "E_move", tp);
    }
}

/*
 * Function name: up_cmd
 * Description:   handle 'up' direction command
 * Arguments:     tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
up_cmd(object tp)
{
    int    step, *co, *ownco;

    if (!tp)
	return 0;
    step  = tp->query_step();
    co    = tp->query_coordinates();
    ownco = query_coordinates()[..];
    if (co[2] >= ownco[2] + query_size()) 
    {
	/* standing at the top */
	tp->catch_tell("It leads down.\n");
	return -1;
    }
    if (co[2] + step >= ownco[2] + query_size())
    {
	/* current step reached the top */
	ownco[2] += query_size();
	if (!E(TO())->search_exit('u', ownco))
	{
	    tp->catch_tell("There is no further way up here.\n");
	    return -1;
	}
	tp->catch_tell("You reach the end.\n");
	tp->move_living(_ROOMD->load_room(other_room), ownco, "up", MOVE_WALK);
	_on_stair(tp);
	return 1;
    }
    ownco[2] = co[2] + step; /* walk up one step */
    tp->move_living(E(this_object()), ownco, "up", MOVE_WALK);
    _on_stair(tp);
    return 1;
}

/*
 * Function name: down_cmd
 * Description:   handle 'down' direction command
 * Arguments:     tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
down_cmd(object tp)
{
    int    step, *co, *ownco;
    object env, other;

    if (!tp)
	return 0;
    step  = tp->query_step();
    co    = tp->query_coordinates();
    ownco = query_coordinates()[..];
    if (co[2] <= ownco[2]) 
    {
	/* standing at the bottom */
	tp->catch_tell("It leads up.\n");
	return -1;
    }
    if (co[2] - step < ownco[2])
    {
	/* current step reaches the bottom */
	if (!E(TO())->search_exit('d', ownco))
	{
	    tp->catch_tell("There is no further way down here.\n");
	    tp->set_coordinates(ownco);
	    return -1;
	}
	tp->move_living(E(this_object()), ownco, "down", MOVE_WALK);
	tp->catch_tell("You reach the end.\n");
	_on_stair(tp);
	return 1;
    }
    ownco[2] = co[2] - step; /* walk down one step */
    tp->move_living(E(this_object()), ownco, "down", MOVE_WALK);
    _on_stair(tp);
    return 1;
}

nomask void
E_move(object ob, object from, object dest)
{
    int *co, *co_ob;
    
    if (!dest || ob == this_object())
	return;
    co = query_coordinates();
    co_ob = ob->query_coordinates();
    if (co && co[0] == co_ob[0] && co[1] == co_ob[1] &&
	co[2]+this_object()->query_range()[2] >= co_ob[2] &&
	co[2] <= co_ob[2])
	return;
    ob->add_prop(UPDOWN_OBJ, 0);
    remove_event(E_MOVE, "E_move", ob);
    co[2] = E(ob)->query_coordinates()[2];
    ob->move_living(dest, co, "away from the " + ::short(), MOVE_WALK);
    if ((co_ob[2] - dest->query_coordinates()[2]) > 150)
    {
	ob->catch_tell("Ouch, that was deeper than you thought.\n");
	ob->check_death(random(10), ob);
    }
    else
	ob->catch_tell("You fall down to the ground.\n");
}
