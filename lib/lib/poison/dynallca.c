/*
 * dynacalla poison
 */
inherit "/base/poison";

#include <effects.h>
#include <event.h>
#include <macros.h>
#include <rr_types.h>
#include <properties.h>
#include <state.h>

#define INTERVAL 5
#define LEVEL    3

static void
renew_poison(object tp)
{
    tp->catch_tell("Ouch! You feel, that something happens to your ears.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " rubs "+
			tp->query_possessive()+" ears.\n" }), MSG_SEE);
    tp->check_death(random(5), this_object());
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
	tp->catch_tell("A shockwave runs through your ears. "+
		       "Suddenly everything is quiet.\n" +
		       "You just can't even hear your own steps!\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " looks confused around.\n" }), MSG_SEE);
    }
    tp->add_effect(DEAF_EFFECT, EFF_DYNALLCA, 1);
    tp->check_death(1 + random(10), this_object());
}

/*
 * Function name: query_subdesc_id
 * Description:   gives name of subdesc
 * Returns:       string
 */
nomask string
query_subdesc_id()
{
    return "_dynallca_";
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
    return "Dynallca, causes deafness";
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

    tp->remove_effect(DEAF_EFFECT, EFF_DYNALLCA);
    if (living(tp) && !(tp->query_state() & (SLEEPING | UNCONSCIOUS)))
    {
	if (!map_sizeof(tp->query_effects()[DEAF_EFFECT]))
	{
	    tp->catch_tell("It seems you have regained your hearing.\n");
	    tell_objects(LISTEN(E(tp)) - ({ tp }),
			 ({ QCTNAME(tp), " smiles and rubs happily "+
		       (tp)->query_possessive()+" ears.\n" }), MSG_SEE);
	}
	else
	{
	    tp->catch_tell("It seems you can hear better now.\n");
	    tell_objects(LISTEN(E(tp)) - ({ tp }),
			 ({ QCTNAME(tp), " scratches confused "+
				(tp)->query_possessive()+" ears.\n" }),
			 MSG_SEE);
	}
    }
}
