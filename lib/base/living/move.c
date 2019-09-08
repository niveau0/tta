/* contains the move related functions for a living */

#include <combat.h>
#include <config.h>
#include <event.h>
#include <language.h>
#include <living.h>
#include <macros.h>
#include <move.h>
#include <properties.h>
#include "prototypes.h"
 
string         move_msg,        /* Message while walking. */
	       run_msg,         /* Message while running. */
	       teleport_in,     /* Message when arriving by teleport. */
	       teleport_out;	/* Message when leaving by teleport. */

/*
 * Function name: move_reset
 * Description:   set some default values, init arrays
 */
static nomask void
move_reset()
{
    set_move_msg(DEFAULT_WALK);
    set_run_msg(DEFAULT_RUN);
    set_teleport_in(DEFAULT_TELEIN);
    set_teleport_out(DEFAULT_TELEOUT);
}

/*
 * Function name:   set_move_msg(string m)
 * Description:     Set the walking message of this living.
 * Arguments:       m - The message string
 */
nomask void
set_move_msg(string m)
{
    move_msg = m;
}

/*
 * Function name:   query_move_msg()
 * Description:     Gives the walking message of this living.
 * Returns:         The message string
 */
nomask string
query_move_msg()
{
    return move_msg;
}

/*
 * Function name:   set_run_msg(string m)
 * Description:     Set the running message of this living.
 * Arguments:       m - The message string
 */
nomask void
set_run_msg(string m)
{
    run_msg = m;
}

/*
 * Function name:   query_run_msg()
 * Description:     Gives the normal running message of this living.
 * Returns:         The message string
 */
nomask string
query_run_msg()
{
    return run_msg;
}

/*
 * Function name:   set_teleport_in(string m)
 * Description:     Set the magical entrance message of this living. You 
 *                  should end the string with a "."
 * Arguments:       m - The message string
 */
nomask void
set_teleport_in(string m)
{
    teleport_in = m;
}

/*
 * Function name:   query_teleport_in()
 * Description:     Gives the magical entrance message of this living.
 * Returns:         The message string
 */
nomask string
query_teleport_in()
{
    return teleport_in;
}

/*
 * Function name:   set_teleport_out(string m)
 * Description:     Set the magical exit message of this living. You should
 *                  end the string with a "."
 * Arguments:       m - The message string
 */
nomask void
set_teleport_out(string m)
{
    teleport_out = m;
}

/*
 * Function name:   query_teleport_out()
 * Description:     Gives the magical exit message of this living.
 * Returns:         The message string
 */
nomask string
query_teleport_out()
{
    return teleport_out;
}

/*
 * Function name: move_living
 * Description:   moves a living with arrival and depart message
 * Arguments:     dest - where to move
 *                co - new coordinates
 *                dir - which direction
 *                how - the way living moves: MOVE_WALK 
 *                                            MOVE_TELEPORT
 *                                            MOVE_SNEAK 
 *                                            MOVE_RUN
 *                                            MOVE_RIDE
 *                                            MOVE_STEP
 * Returns:       the return of move(), -1 is 'too tired'
 */
int
move_living(object dest, int *co, string dir, int how)
{
    int    e;
    int *oldco;
    object env;

    if (!dest)
	return 1;
    
    env = E(this_object());
    oldco = query_coordinates();
    if (how == MOVE_TELEPORT)
    {
	tell_objects(LISTEN(env) - ({ this_object() }),
		     ({ QCTNAME(this_object()), " " + teleport_out + "\n" }),
		     MSG_SEE, this_object());
	set_coordinates(co);
	if (e = move(dest))
	{
	    set_coordinates(oldco);
	    return e;
	}
	if (env != dest)
	    look_room(dest);
	tell_objects(LISTEN(dest) - ({ this_object() }),
		     ({ QCNAME(this_object()),	" " + teleport_in + "\n" }),
		     MSG_SEE, this_object());
	return 0;
    }

    if (how != MOVE_RIDE)
    {
	if (query_fatigue() < 1)
	{
	    catch_tell("You are too tired to move.\n");
	    return -1;
	}
	e = env->query_move_fatigue() + weight_encumbrance() / 20;
    }
    
    switch (how)
    {
      case MOVE_SNEAK:
	  e *= 2;
	  catch_tell("You sneak " + dir + ".\n");
	  break;
      case MOVE_RUN:
	  e *= 3;
	  catch_tell("You run " + dir + ".\n");
	  tell_objects(LISTEN(env) - ({ this_object() }),
		       ({ QCTNAME(this_object()),
			      " " + run_msg + " " + dir + ".\n" }),
		       MSG_SEE | MSG_WALK, this_object());
	  break;
      case MOVE_RIDE:
	  catch_tell("You ride " + dir + ".\n");
	  tell_objects(LISTEN(env) - ({ this_object() }),
		       ({ QCTNAME(this_object()),
			      " rides " + dir + ".\n" }),
		       MSG_SEE | MSG_WALK, this_object());
	  break;
      case MOVE_STEP:
	  if (e > 1) e /= 2;
	  catch_tell("You take a small step " + dir + ".\n");
	  tell_objects(LISTEN(env) - ({ this_object() }),
		       ({ QCTNAME(this_object()),
			      " takes a small step " + dir + ".\n" }),
		       MSG_SEE | MSG_WALK, this_object());
	  break;
      default:
	  catch_tell("You walk " + dir + ".\n");
	  tell_objects(LISTEN(env) - ({ this_object() }),
		       ({ QCTNAME(this_object()),
			      " " + move_msg + " " + dir + ".\n" }),
		       MSG_SEE | MSG_WALK, this_object());
    }
    add_fatigue(e);
    
    /* message must be sent before moving */
    set_coordinates(co);
    if (e = move(dest))
    {
	set_coordinates(oldco);
	return e;
    }
    if (env != dest)
	look_room(dest);
    return 0;
}

/*
 * Function name: query_step
 * Description:   how fast does this living walk
 * Returns:       integer, step length (inch)
 */
int
query_step()
{
    int sp;
    
    if (!(sp = query_prop(LIVE_MOVE_SPEED)))
    {
	if ((sp = query_stat_bonus(STAT_QU)) < 0)
	    sp = query_size() / 2;
	else if (sp > 10)
	    sp = 10 + query_size() / 2;
	else
	    sp += query_size() / 2;
	add_prop(LIVE_MOVE_SPEED, sp);
    }
    return (sp * (100 - weight_encumbrance() +
		  cb_query_bonus(CB_ACTION_MOD)) / 100);
}
