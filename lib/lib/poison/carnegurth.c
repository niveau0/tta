/*
 * carnegurth poison
 */
inherit "/base/poison";

#include <effects.h>
#include <event.h>
#include <macros.h>
#include <rr_types.h>
#include <properties.h>
#include <state.h>

#define INTERVAL (120 + random(100))
#define LEVEL    1

static void
renew_poison(object tp)
{
    tp->catch_tell("Ouch! Your heart is aching.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " rubs " +
			tp->query_possessive() + " breast.\n" }), MSG_SEE);
    tp->check_death(random(4)+1, this_object());
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
    if (count > 3)
    {
	if (tp->resistance_roll(RR_POISON, LEVEL, -50) > 0)
	{
	    remove_poison(tp);
	    tp->catch_tell("You feel like your veins have been"+
			   " cleared.\n");
	    tell_objects(LISTEN(E(tp)) - ({ tp }),
			 ({ QCTNAME(tp),
				" seems to be glad about something.\n" }),
			 MSG_SEE);
	    return;
	}
    }
    tp->add_effect(POISON_EFFECT, MASTER, count);

    if (tp->query_state() & (SLEEPING | UNCONSCIOUS))
	sleep = 1;
	
    switch (count)
    {
      case 2:
	  tp->catch_tell("Your muscles become somehow cold.\n");
	  break;
      case 3:
	  tp->catch_tell("You feel very cold, are you getting ill?\n");
	  tell_objects(LISTEN(E(tp)) - ({ tp }),
		       ({ QCTNAME(tp), " shivers, looking sick and pale.\n" }),
		       MSG_SEE);
	  break;
      case 4:
	  if (sleep)
	  {
	      tp->catch_tell("You feel very cold, are you getting ill?\n");
	      tell_objects(LISTEN(E(tp)) - ({ tp }),
			   ({ QCTNAME(tp), " shivers, looking sick and " +
				  "pale.\n" }),
			   MSG_SEE);
	      break;
	  }
	  tp->catch_tell("Ouch! You feel hurt by every move you make.\n");
	  tell_objects(LISTEN(E(tp)) - ({ tp }),
		       ({ QCTNAME(tp), " shivers, looking sick and pale.\n" }),
		       MSG_SEE);
	  break;
      case 5:
	  if (sleep)
	      tp->catch_tell("You feel very cold, are you getting ill?\n");
	  else
	      tp->catch_tell("After verifying yourself, due to immense " +
			     "pain, you notice your blood\ncirculation " +
			     "has nearly " +
			     "stopped. You should search a healer ... now!\n");
	  tell_objects(LISTEN(E(tp)) - ({ tp }),
		       ({ QCTNAME(tp),
			      " moans and seems to pass away from life.\n" }),
		       MSG_SEE);
	  break;
      case 6:
	  tp->catch_tell("You feel your heart sending a last weak pulse ... "+
			 "but then, blackness falls\nover your sight.\n");
	  remove_poison(tp);
	  tp->do_die();
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
    return "_carnegurth_";
}

nomask string
subdesc(object tp, object for_ob)
{
    if (!poisoned_players[tp])
	return "";
    if (tp == for_ob)
	return "You feel badly hurt, every move is painful.\n";
    return capitalize(tp->query_pronoun()) + " looks really bad.\n";
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
 * Function name: query_interval
 * Description:   gives call_out interval
 * Returns:       int
 */
nomask int
query_interval()
{
    return INTERVAL;
}

/*
 * Function name: query_desc
 * Description:   gives poison description
 * Returns:       string
 */
nomask string
query_desc()
{
    return "Carnegurth, will cause massive blood clotting";
}
