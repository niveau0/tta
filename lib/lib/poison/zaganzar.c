/*
 * zaganzar poison
 */
inherit "/base/poison";

#include <effects.h>
#include <event.h>
#include <macros.h>
#include <rr_types.h>
#include <properties.h>
#include <state.h>

#define INTERVAL 5
#define LEVEL    5

static void
renew_poison(object tp)
{
    if (!(tp->query_state() & (SLEEPING | UNCONSCIOUS)))
    {
	tp->catch_tell("Outch! Something burns your blind eyes.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " rubs "+(tp)->query_possessive()+
			    " eyes.\n" }),
		     MSG_SEE);
    }
    tp->check_death(2+random(5), this_object());
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
	    tp->catch_tell("Your vision blurs. Something " +
			   "behind your eyes burns"+
			   " -\n suddenly everything is dark.\n");
	}
	else
	tp->catch_tell("Something happens to your eyes.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " rolls "+(tp)->query_possessive()+
		       " eyes. After that "+(tp)->query_possessive()+
		       " eyes become strangly transparent.\n" }),
		MSG_SEE);
    }
    tp->add_effect(BLIND_EFFECT, EFF_ZAGANZAR, 1);
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
    return "_zaganzar_";
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
    return "Zaganzar, it will reduce your optic nerves to water";
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

    tp->remove_effect(BLIND_EFFECT, EFF_ZAGANZAR);
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
