/*
 * henuial poison
 */
inherit "/base/poison";

#include <effects.h>
#include <event.h>
#include <macros.h>
#include <rr_types.h>
#include <properties.h>
#include <state.h>

#define INTERVAL (random(15) + 1)
#define LEVEL    6


static void
renew_poison(object tp)
{
    tp->catch_tell("Ouch! Something burns your blind eyes.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " rubs "+(tp)->query_possessive()+
			" eyes.\n" }), MSG_SEE);
    tp->check_death(2, this_object());
}

/*
 * Function name: poison_effect
 * Description:   this must be redefined to create a poison effect
 * Arguments:     tp - the affected player
 */
static nomask void
poison_effect(object tp)
{
    if (!tp || !(poisoned_players[tp]) || !living(tp))
	return;

    if (!(tp->query_state() & (SLEEPING | UNCONSCIOUS)))
    {
	if (!map_sizeof(tp->query_effects()[BLIND_EFFECT]))
	{
	    tp->catch_tell("Something happens to your eyes. Suddenly"+
			   " darkness surrounds you.\n");
	}
	else
	    tp->catch_tell("Something happens to your eyes.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " rubs "+(tp)->query_possessive()+
			    " eyes and moans about something.\n" }),
		     MSG_SEE);
    }
    tp->add_effect(BLIND_EFFECT, EFF_HENUIAL, 1);
}

/*
 * Function name: query_subdesc_id
 * Description:   gives name of subdesc
 * Returns:       string
 */
nomask string
query_subdesc_id()
{
    return "_henuial_";
}

/*
 * Function name: query_level
 * Description:   gives poison level
 * Returns:       int
 */
nomask int
query_level()
{
    return LEVEL;
}
     
/*
 * Function name: query_desc
 * Description:   gives poison description
 * Returns:       string
 */
nomask string
query_desc()
{
    return "Henuial, it will reduce your optic juices to honey";
}

/*
 * Function name: query_interval
 * Description:   gives call_out interval
 * Returns:       int
 */
nomask int
query_interval()
{
    return INTERVAL;
}

void
remove_poison(object tp)
{
    ::remove_poison(tp);

    tp->remove_effect(BLIND_EFFECT, EFF_HENUIAL);
    if (living(tp) && !(tp->query_state() & (SLEEPING | UNCONSCIOUS)))
    {
        if (!map_sizeof(tp->query_effects()[BLIND_EFFECT]))
        {
    	    tp->catch_tell("It seems you have returned your eyesight.\n");
	    tell_objects(LISTEN(E(tp)) - ({ tp }),
			 ({ QCTNAME(tp), " looks happily around.\n" }),
			 MSG_SEE);
	}
	else
	{
	    tp->catch_tell("Your eyes are feeling better, but you are "+
			   "still blind.\n");
	}
    }
}
