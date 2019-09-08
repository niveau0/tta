/*
 * nimnaur poison
 */
inherit "/base/poison";

#include <effects.h>
#include <event.h>
#include <macros.h>
#include <rr_types.h>
#include <properties.h>
#include <state.h>

#define INTERVAL 150+(random(300)) 
#define LEVEL    3

static void
renew_poison(object tp)
{
    tp->catch_tell("Your stomach feels like acid.\n");
    tp->command("burp");
    tp->check_death(random(10)+2, this_object());
}
				     
/*
 * Function name: poison_effect
 * Description:   this must be redefined to create a poison effect
 * Arguments:     tp - the affected player
 */
static nomask void
poison_effect(object tp)
{
    int count, sleep;

    if (!tp || !(poisoned_players[tp]) || !living(tp))
	return;

    count = poisoned_players[tp]++;

    call_out("poison_effect", INTERVAL, tp);
   
    if (tp->resistance_roll(RR_POISON, LEVEL, -50) > 0)
    {
	remove_poison(tp);
	tp->catch_tell("Your body seems to be more healthy now.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " seems to be more healthy now.\n" }),
		   MSG_SEE);
	return;
    }

    tp->add_effect(POISON_EFFECT, MASTER, count);

    if (tp->query_state() & (SLEEPING | UNCONSCIOUS))
	sleep = 1;
	
    switch (count)
    {
      case 1:
	tp->catch_tell("You seem to have a gastric flu!\n");
	if (!sleep)
	    tell_objects(LISTEN(E(tp)) - ({ tp }),
			 ({ QCTNAME(tp), " holds "+(tp)->query_possessive()+
				" belly.\n" }),
			 MSG_SEE);
	break;
     
      case 2:
	tp->catch_tell("Ouch! Something burns your stomach."+
		       " What did you eat today?\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " seems to have problems with "+
		   (tp)->query_possessive()+" stomach.\n" }),
		MSG_SEE);
	break;

      case 3:
	tp->catch_tell("You just can't take a step without a "+
		       "hellfire of pain.\nNow it's not only "+
		       "the stomach that hurts.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " bends down from pain and "+
		   "looks very sick.\n" }),
		MSG_SEE);
	break;

      case 4:
	tp->catch_tell("With every gulp a fireball explodes "+
		       "in your belly.\n");
	if (!sleep)
	    tell_objects(LISTEN(E(tp)) - ({ tp }),
			 ({ QCTNAME(tp), " can't stand upright anymore. "+
				"Something bends "+
				tp->query_objective()+" down.\n"}),
			 MSG_SEE);
	break;

      case 5:
	tp->catch_tell("Your stomach dissolved to water. Your "+
		       "stomach acid corrodes\nevery vital organ "+
		       "around! Everything gets dark.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " holds "+(tp)->query_possessive()+
		   " belly a last time and throws up a mixture of "+
		   "blood and water!\n" }),
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
    return "_nimnaur_";
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
    return "Nimnaur, it liquifies your stomach";
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
