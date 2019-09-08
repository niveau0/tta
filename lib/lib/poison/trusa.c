/*
 * trusa poison
 */
inherit "/base/poison";

#include <effects.h>
#include <event.h>
#include <macros.h>
#include <mudtime.h>
#include <rr_types.h>
#include <properties.h>
#include <state.h>

#define INTERVAL 5+random(50)
#define LEVEL    4

/*
 * Function name: poison_effect
 * Description:   this must be redefined to create a poison effect
 * Arguments:     tp - the affected player
 */
static nomask void
poison_effect(object tp)
{
    int rr, sleep;

    if (!tp || !(poisoned_players[tp]) || !living(tp))
	return;

    rr = tp->resistance_roll(RR_POISON, LEVEL, 0);

    if (tp->query_state() & (SLEEPING | UNCONSCIOUS))
	sleep = 1;
    
    if (rr <= -21)
    {
    	tp->catch_tell("You just don't have the power to lift your arms. "+
		       "Everything seems to fade\nfor a few seconds "+
		       "and the world seems to be far far away ...\n");
	if (!sleep)
	    tell_objects(LISTEN(E(tp)) - ({ tp }),
			 ({ QCTNAME(tp), " staggers and closes "+
				(tp)->query_possessive()+
				" eyes for a few seconds, "+
				(tp)->query_pronoun()+
				" seems to look far far away for " +
				"a better future.\n"}),
			 MSG_SEE);
    	remove_poison(tp);
	tp->do_die();
    }
    else if (rr <= 0)
    {
	if (!sleep)
	{
	    tp->catch_tell("Suddenly the world begins to fade. You just "+
			   "don't have the power\nto stand on your "+
			   "own feets anymore. Your vision blurs.\n");
	    tell_objects(LISTEN(E(tp)) - ({ tp }),
			 ({ QCTNAME(tp), " stumbles and tries to get a hold."+
				" You see that something stole "+
				tp->query_possessive()+
				" energy and "+tp->query_pronoun()+
				" even can't "+
				"stand anymore.\n"}),
			 MSG_SEE);
	}
	else
	    tp->catch_tell("Something evil hits you.\n");
	tp->add_unconscious((1+random(5)) * 86400 / TIME_SCALE );
	tp->add_effect(BLIND_EFFECT, EFF_TRUSA, 1);
    }
    else
    {
	if (!sleep)
	{
	    if (!map_sizeof(tp->query_effects()[BLIND_EFFECT]))
	    {
		tp->catch_tell("Everything seems to lose its colors. "+
			       "After a while the horizon begins to\n"+
			       "fade. Your eyes begin to hurt and the light " +
			       "seems to disappear.\n");
	    }
	    else
		tp->catch_tell("Your eyes hurt.\n");
	    tell_objects(LISTEN(E(tp)) - ({ tp }),
			 ({ QCTNAME(tp), " stares to the horizon " +
				"and sometimes rubs "+
				tp->query_possessive()+" eyes.\n"}),
			 MSG_SEE);
	}
	else
	    tp->catch_tell("Something evil hits you.\n");
	tp->add_effect(BLIND_EFFECT, EFF_TRUSA, 1);
    }


}

/*
 * Function name: query_subdesc_id
 * Description:   gives name of subdesc
 * Returns:       string
 */
nomask string
query_subdesc_id()
{
    return "_trusa_";
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
    return "Trusa, it destroys some very important muscles";
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

    tp->remove_effect(BLIND_EFFECT, EFF_TRUSA);
    if (living(tp) && !(tp->query_state() & (SLEEPING | UNCONSCIOUS)) &&
	!map_sizeof(tp->query_effects()[BLIND_EFFECT]))
    {
	tp->catch_tell("It seems your sight has returned.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " looks happily around.\n" }),
		     MSG_SEE);
    }
}
