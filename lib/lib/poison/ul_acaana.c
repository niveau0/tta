/*
 * ul-acaana poison
 */
inherit "/base/poison";

#include <effects.h>
#include <event.h>
#include <macros.h>
#include <rr_types.h>
#include <properties.h>
#include <slots.h>
#include <state.h>

#define INTERVAL 10
#define LEVEL    20

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

    call_out("poison_effect", (random(600)), tp);

    if (tp->resistance_roll(RR_POISON, LEVEL, -100) > 0)
    {
	remove_poison(tp);
	return;
    }

    tp->add_effect(POISON_EFFECT, MASTER, count);
 
    if (count == 1)
    {
	if(living(tp) && !(tp->query_state() & (SLEEPING | UNCONSCIOUS)))
	{
	    tp->catch_tell("You seem to have lost the control of your " +
			   "body !\nYou can't move anymore.\n");
	    tell_objects(LISTEN(E(tp)) - ({ tp }),
			 ({ QCTNAME(tp), " seems to have lost the control"+
				" of "+(tp)->query_possessive()+" body.\n" }),
			 MSG_SEE);
	}
	else
	    tp->catch_tell("Soething evil hits you.\n");
	tp->add_state(PARALYSED);
	tp->add_effect(PARALYSE_EFFECT, EFF_UL_ACAANA, 1);
    }

    if (count == 2)
    {
	tp->catch_tell("You think your life is worthless since you can't move"+
		       " yourself.\nNothing seems to be worth to live for!\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " murmurs about live. After that "+
		   (tp)->query_pronoun()+" closes "+(tp)->query_possessive()+
		   " eyes and leaves "+(tp)->query_possessive()+" body.\n" }),
		   MSG_SEE);
	tp->do_die();
	remove_poison(tp);
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
    return "_ul_acaana_";
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
    return "Ul-Acaana, it destroys your nervous system";
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
        return "You are completely paralysed.\n";
    return capitalize(tp->query_pronoun()) + " is paralysed.\n";
}

void 
remove_poison(object tp)
{
    ::remove_poison(tp);

    tp->remove_effect(PARALYSE_EFFECT, EFF_UL_ACAANA);
    if (!map_sizeof(tp->query_effects()[PARALYSE_EFFECT]))
	tp->delete_state(PARALYSED);
    if (living(tp) && !(tp->query_state() & (SLEEPING | UNCONSCIOUS)))
    {
        if (!map_sizeof(tp->query_effects()[PARALYSE_EFFECT]))
	{
	    tp->catch_tell("It seems you have returned your agility.\n");
	    tell_objects(LISTEN(E(tp)) - ({ tp }),
			 ({ QCTNAME(tp), " seems to be more dexterous.\n" }),
			 MSG_SEE);
	}
    }
}
