/*
 * karfar poison
 */
inherit "/base/poison";

#include <effects.h>
#include <event.h>
#include <macros.h>
#include <rr_types.h>
#include <properties.h>
#include <state.h>

#define INTERVAL (random(200)+50)
#define LEVEL    7

static void
renew_poison(object tp)
{
    tp->catch_tell("Your heart stops beating for a second.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
	({ QCTNAME(tp), " looks confused "+
	(tp)->query_possessive()+" breast.\n" }),
	MSG_SEE);
    tp->check_death(random(3)+7, this_object());
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

    call_out("poison_effect", (INTERVAL+random(30)), tp);
   
    if (tp->resistance_roll(RR_POISON, LEVEL, -50) > 0)
    {
	remove_poison(tp);
	tp->catch_tell("You feel your heart beating normal again.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " seems to be glad about something.\n" }),
		   MSG_SEE);
	return;
    }

    tp->add_effect(POISON_EFFECT, MASTER, count);

    if (tp->query_state() & (SLEEPING | UNCONSCIOUS))
	sleep = 1;
    
    switch (count)
    {
      case 1:
	  tp->catch_tell("You feel, something is wrong with your heart !\n");
	  if (!sleep)
	      tell_objects(LISTEN(E(tp)) - ({ tp }),
			   ({ QCTNAME(tp), " looks confused on "+
				  tp->query_possessive()+
				  " breast.\n"}),
			   MSG_SEE);
	  break;

      case 2:
	  tp->catch_tell("You are just out of breath.\n");
	  tell_objects(LISTEN(E(tp)) - ({ tp }),
		       ({ QCTNAME(tp), " seems to be out of breath.\n"}),
		       MSG_SEE);
	  break;
	  
      case 3:
	  if (sleep)
	  {
	      tp->catch_tell("Your heart aches.\n");
	      break;
	  }
	  tp->catch_tell("Everything seems to disappear for a few seconds.\n");
	  tell_objects(LISTEN(E(tp)) - ({ tp }),
		       ({ QCTNAME(tp), " closes "+tp->query_possessive()+
			      " eyes and stumbles for a moment.\n" }),
		       MSG_SEE);
	  break;

      case 4:
	tp->catch_tell("Your heart beats VERY slow now. You just "+
		       "see your own ancestors looking at your "+
		       "poor soul.\n");
	if (!sleep)
	    tell_objects(LISTEN(E(tp)) - ({ tp }),
			 ({ QCTNAME(tp), " looks confused at an unreal " +
				"person.\n" }),
			 MSG_SEE);
	break;

      case 5:
	  if (sleep)
	  {
	      tp->catch_tell("You hear the wailing of the Banshee.\n");
	      tell_objects(LISTEN(E(tp)) - ({ tp }),
			   ({ QCTNAME(tp), " sweats in an unnatural way.\n" }),
			   MSG_SEE);
	      break;
	  }
	  tp->catch_tell("You hear the wailing of the Banshee. Every "+
			 "move is very hard now, every effort could bring"+
			 " you to death.\n");
	  tell_objects(LISTEN(E(tp)) - ({ tp }),
		       ({ QCTNAME(tp), " sweats in an unnatural way"+
			      " and tries to prevent every"+
			      " useless effort.\n" }),
		       MSG_SEE);
	  break;

      case 6:
	tp->catch_tell("You feel yourself becoming pale. Suddenly your heart"+
		       " makes a last beat.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " becomes very pale, uttering some words."+
		   " After that "+tp->query_pronoun()+" rolls "+
		   tp->query_possessive()+" eyes and sinks on "+
		   tp->query_possessive()+" knees.\n" }),
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
    return "_karfar_";
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
    return "Karfar, it causes your heart to shut down";
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
        return "You feel your heart beating abnormal.\n";
    return capitalize(tp->query_pronoun()) + " looks pale.\n";
}
