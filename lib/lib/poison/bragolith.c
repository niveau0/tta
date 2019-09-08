/*
 * bragolith poison
 */
inherit "/base/poison";

#include <effects.h>
#include <event.h>
#include <macros.h>
#include <rr_types.h>
#include <properties.h>
#include <state.h>

#define INTERVAL 20+random(220)
#define LEVEL    5

static void
renew_poison(object tp)
{
    tp->catch_tell("Ouch! You feel VERY hot!\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " seems to be VERY hot!\n" }), MSG_SEE);
    tp->check_death(random(5)+3, this_object());
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

    if (tp->query_state() & (SLEEPING | UNCONSCIOUS))
	sleep = 1;
	
    if (tp->resistance_roll(RR_POISON, LEVEL, -100) > 0)
    {
	remove_poison(tp);
	if (sleep)
	    return;
	tp->catch_tell("You seem to have calmed down.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " seems to cool down.\n" }),
		   MSG_SEE);
	return;
    }

    tp->add_effect(POISON_EFFECT, MASTER, count);
 
    switch (count)
    {
      case 1:
	  if (!sleep)
	      tp->catch_tell("You begin to sweat.\n");
	  tell_objects(LISTEN(E(tp)) - ({ tp }),
		       ({ QCTNAME(tp), " begins to sweat.\n" }),
		       MSG_SEE);
	  break;
     
      case 2:
	  if (!sleep)
	      tp->catch_tell("You just feel very hot!\n");
	  tell_objects(LISTEN(E(tp)) - ({ tp }),
		       ({ QCTNAME(tp), " sweats in an abnormal way.\n" }),
		       MSG_SEE);
	  break;

      case 3:
	  if (!sleep)
	      tp->catch_tell("You think you are burning inside.\n");
	  tell_objects(LISTEN(E(tp)) - ({ tp }),
		       ({ QCTNAME(tp), " gasps for breath. An unnatural "+
			      "heat comes from " +
			      tp->query_objective()+".\n" }), MSG_SEE);
	  break;

      case 4:
	  if (!sleep)
	  tp->catch_tell("Your skin becomes red. You can't concentrate "+
			 "on anything\nand suddenly a little flame shootes "+
			 "out of one of your fingers.\n");
	  tell_objects(LISTEN(E(tp)) - ({ tp }),
		       ({ QCTNAME(tp), "'s skin seems to be unnatural red, "+
			      "and suddenly a small flame shoots out of "+
			      tp->query_possessive()+ " fingers.\n" }),
		       MSG_SEE);
	  break;

      case 5:
	  tp->catch_tell("With an immense pain every opening of your corpse "+
			 "begins to burn.\nYou can't realize anything around "+
			 "you.\n");
	  tell_objects(LISTEN(E(tp)) - ({ tp }),
		       ({ QCTNAME(tp),
			      " looks very puzzled while every opening of " +
			      tp->query_possessive() +
			      " corpse begins to burn.\n" }), MSG_SEE);
	  break;

      case 6:
	  tp->catch_tell("You are burning like a torch. When the fire "+
			 "catches your\neyes everything gets dark. "+
			 "The last you realize,\nis that the whole nature "+
			 "seems to observe you.\n");
	  tell_objects(LISTEN(E(tp)) - ({ tp }),
		       ({ QCTNAME(tp), " burns like a torch. With " +
			      tp->query_possessive() + " last power " +
			      tp->query_pronoun()+
			      " looks around and just ... burns down.\n" }),
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
    return "_bragolith_";
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
    return "Bragolith, it burns you down to death";
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
