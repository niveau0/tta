/*
 * acaana poison
 */
inherit "/base/poison";

#include <effects.h>
#include <event.h>
#include <macros.h>
#include <rr_types.h>
#include <properties.h>

#define INTERVAL random(150)+50
#define LEVEL    10

static void
renew_poison(object tp)
{
    tp->catch_tell("Suddenly a shockbolt runs though your nerves.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " shivers in an unnatural way.\n" }),
		 MSG_SEE);
    tp->check_death(random(5)+2, this_object());
}

/*
 * Function name: poison_effect
 * Description:   this must be redefined to create a poison effect
 * Arguments:     tp - the affected player
 */
static nomask void
poison_effect(object tp)
{
    int count;

    if (!tp || !(poisoned_players[tp]) || !living(tp))
	return;

    count = poisoned_players[tp]++;

    call_out("poison_effect", INTERVAL, tp);

    if (tp->resistance_roll(RR_POISON, LEVEL, -100) > 0)
    {
	remove_poison(tp);
	tp->catch_tell("You seem to have returned the control "+
		       "of your body.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " seems to be glad about something.\n" }),
		   MSG_SEE);
	return;
    }

    tp->add_effect(POISON_EFFECT, MASTER, count);
 
    switch (count)
    {
      case 1:
	tp->catch_tell("You get an uneasy feeling!\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " looks nervously around.\n" }),
		     MSG_SEE);
	break;
     
      case 2:
	tp->catch_tell("You just feel hot and cold!\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " sweats and shivers simultaneously.\n" }),
		MSG_SEE);
	break;

      case 3:
	tp->catch_tell("You suddenly shake your arms. You try to stop"+
		       ", but you can't.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " shakes "+tp->query_possessive()+
		   " arms and looks very puzzled.\n" }),
		MSG_SEE);
	break;

      case 4:
	tp->catch_tell("Your whole body begins to shiver. Something "+
		       "is wrong with you, and you should really "+
		       "worry about it!\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " shivers all over "+tp->query_possessive()+
		   " body and you think, "+tp->query_pronoun()+
		   " should go to bed.\n" }),
		MSG_SEE);
	break;

      case 5:
	tp->catch_tell("You seem to have lost the control of your body!\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " seems to have lost the control"+
		   " of "+tp->query_possessive()+" body.\n" }),
		   MSG_SEE);
	break;

      case 6:
	tp->catch_tell("Your nerves seem to begin to quiver. A shockbolt "+
                       "runs through your nervs into your brain!\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " wildly flounces around for a while."+
			    " After a short time "+ tp->query_pronoun() +
			    " collapses and is just ... quiet.\n" }),
		   MSG_SEE);
	tp->do_die();
	remove_poison(tp);
	break;
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
    return "_acaana_";
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
    return "Acaana, it destroys your nervous system";
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

nomask string
subdesc(object tp, object for_ob)
{
    if (!poisoned_players[tp])
        return "";
    if (tp == for_ob)
        return "You feel your nerves tingeling.\n";
    return capitalize(tp->query_pronoun()) + " looks nervous.\n";
}
