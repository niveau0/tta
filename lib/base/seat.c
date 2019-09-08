inherit "object";

#include <composite.h>
#include <event.h>
#include <language.h>
#include <macros.h>
#include <properties.h>
#include <state.h>

static int    free;
static object *used_by;

static void
create_seat()
{
}

static nomask void
create_object()
{
    create_seat();
    used_by = ({ });
    add_adj("free");
}

/*
 * Function name: set_free_seats
 * Description:   how many people may sit on this seat at the same time
 * Arguments:     num - maximum free seats
 */
nomask void
set_free_seats(int num)
{
    free = num;
}

/*
 * Function name: query_persons
 * Description:   returns, who is sitting on this chair
 * Returns:       object array
 */
nomask object *
query_persons()
{
    return used_by;
}


/*
 * Function name: sit_cmd
 * Description:   Called by cmdsoul, let someone sit down on the seat
 * Returns:       1 success, -1 else
 */
int
sit_cmd()
{
    object tp;

    tp = this_player();
    if (sizeof(used_by) >= free) 
    {
	tp->catch_msg(({ QCTNAME(this_object()), " is fully occupied.\n" }),
		      0, 0);
	return -1;
    }
    if (E(this_object()) == tp)
    {
	tp->catch_tell("You should drop it first.\n");
	return -1;
    }
    used_by += ({ tp });
    if (sizeof(used_by) >= free)
    {
	remove_adj("free");
	add_adj("occupied");
    }
    add_event(E_MOVE, "E_move", tp);
    tp->set_coordinates(query_coordinates());
    tp->define_primary("catch_cmd", this_object());
    tp->add_state(RESTING);
    tp->add_extra_desc(" is sitting here");
    tp->echo("You sit down on the " + short() + ".\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " sits down on ", QNAME(this_object()),
			".\n"}), MSG_SEE ,tp);
    add_prop(OBJ_NO_GET, 1);
    return 1;
}

/*
 * Function name: leave_seat
 * Description:   leave the chair, by moving or stand up
 * Arguments:     tp - who is leaving
 */
static nomask void
leave_seat(object tp)
{
    used_by -= ({ tp });
    remove_adj("occupied");
    add_adj("free");
    tp->undefine_primary("catch_cmd", this_object());
    tp->remove_extra_desc(" is sitting here");
    add_prop(OBJ_NO_GET,0);
    remove_event(E_MOVE, "E_move", tp);
}

/*
 * Function name: catch_cmd
 * Description:   catch all cmds to ensure leaving the seat
 * Returns:       0/1 success, -1 else
 */
nomask int
catch_cmd(string cmd, string arg)
{
    int    state;
    object tp;
    
    tp = this_player();
    state = tp->query_state();
    if (!(state & RESTING))
    {
	leave_seat(tp);
	return 0;
    }
    if (cmd == "stand" && arg == "up")
    {
	if (MISC_SOUL->do_command("stand", "up", state) == -1)
	    return -1;
	leave_seat(tp);
	return 1;
    }
    return 0;
}

/*
 * Function name: E_move
 * Description:   event: catch moving of player
 * Arguments:     ob - moving object
 *                old - from where it comes
 *                dest - where it goes
 * Returns:       
 */
nomask void
E_move(object ob, object old, object dest)
{
    if (!ob)
	return;
    
    leave_seat(ob);
    ob->delete_state(RESTING);
}

/*
 * Function name: long
 * Description:   here: care for the description of people occupying the seat
 * Returns:       string, long description
 */
string
long()
{
    int    i;
    string name, *sitting;
    object tp, *seen;
    
    if (!(tp = this_player()))
	return ::long();
    if (!(i = sizeof(seen = tp->visible(used_by))))
	return ::long();
    for (sitting=allocate(i); i--;)
    {
	if (tp == seen[i])
	    sitting[i] = "you";
	else if (!(name = tp->query_met(seen[i]->query_name())))
	    sitting[i] = LANG_ADDART( seen[i]->query_nonmet_name());
	else 
	    sitting[i] = capitalize(name);
    }
    return (::long() + capitalize(COMPOSITE_WORDS(sitting)) +
	    ((sizeof(seen) > 1 || tp == seen[0]) ? " are" : " is") +
	    " sitting on it.\n");
}
