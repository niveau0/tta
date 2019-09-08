/*
 * ondohithui poison
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
    tp->catch_tell("For a second you would give a kingdom for a \n"+
	"good swallow of water.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
	({ QCTNAME(tp), " seems to have an immense thirst.\n" }),
	MSG_SEE);
    tp->check_death(random(3)+3, this_object());
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
	tp->catch_tell("Your tongue feels like leather.\n");
	if (!sleep)
	    tell_objects(LISTEN(E(tp)) - ({ tp }),
			 ({ QCTNAME(tp), " looks around for some water.\n" }),
			 MSG_SEE);
	break;
     
      case 2:
	tp->catch_tell("Somehow, you feel yourself drying out.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " has lost "+(tp)->query_possessive()+
		   " vital appearance.\n" }),
		MSG_SEE);
	break;

      case 3:
	tp->catch_tell("Your skin seems to lose all liquid. It looks "+
		       "like an old parchment.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), "'s skin seems to lose all liquid.\n" }),
		     MSG_SEE);
	break;

      case 4:
	  if (sleep)
	      tp->catch_tell("You feel dry - very dry indeed.\n");
	  else
	      tp->catch_tell("With every movement your " +
			     "joints seem to creak.\n"+
			     " Your skin is now very pale and ashen.\n");
	  tell_objects(LISTEN(E(tp)) - ({ tp }),
		       ({ QCTNAME(tp), " nearly looks like a mummy. "+
			      "Every vitality seems to be gone from "+
			      (tp)->query_objective()+".\n"}),
		       MSG_SEE);
	  break;

      case 5:
	tp->catch_tell("You are totally dehydrated. Every muscle, "+
		       "every sinew \neven every organ is dried out.\n"); 
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), "'s body is totally dehydrated. "+
		   "Every muscle hangs flabby down and "+
		   (tp)->query_possessive()+" skin seems to "+
		   "crumble to dust if you touch it!\n" }),
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
    return "_ondohithui_";
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
    return "Ondohithui, it dehydrates your body";
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
	return "Your skin is abnormal grey.\n";
    return capitalize(tp->query_pronoun()) + " looks abnormally grey.\n";
}
