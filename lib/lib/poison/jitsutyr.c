/*
 * jitsutyr poison
 */
inherit "/base/poison";

#include <effects.h>
#include <event.h>
#include <macros.h>
#include <rr_types.h>
#include <properties.h>

#define INTERVAL (random(300)+20) 
#define LEVEL    2

static void
renew_poison(object tp)
{
    tp->catch_tell("Outch! You feel a fireball in your lungs.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
	({ QCTNAME(tp), " coughs, becomes red and then pale.\n" }),
	MSG_SEE);
    tp->check_death(random(7)+5, this_object());
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
   
    if (tp->resistance_roll(RR_POISON, LEVEL, -50) > 0)
    {
	remove_poison(tp);
	tp->catch_tell("You can breath more easily now.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " seems to breath more easily.\n" }),
		   MSG_SEE);
	return;
    }

    tp->add_effect(POISON_EFFECT, MASTER, count);
 
    switch (count)
    {
      case 1:
	tp->catch_tell("Breathing is more difficult now!\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " breathes in an abnormal way.\n" }),
		MSG_SEE);
	break;
     
      case 2:
	tp->catch_tell("Something is burning your lungs."+
		       " It is very hard to breath!\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " seems to have problems with "+
		   (tp)->query_possessive()+" breathing.\n" }),
		MSG_SEE);
	break;

      case 3:
	tp->catch_tell("Without warning a hellfire of pain "+
		       "hits you and everything gets dark\n"+
		       "for a few seconds. You have the taste of "+
		       "copper in your nose.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " closes "+(tp)->query_possessive()+
		   " eyes after a deep breath. Blood runs"+
		   " out of "+(tp)->query_possessive()+" mouth.\n" }),
		MSG_SEE);
	break;

      case 4:
	tp->catch_tell("With every breath you take, a fireball seems "+
		       "to explode in your breast.\n"+
		       "You try to avoid breathing, but you should "+
		       "search for help.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " pulls faces of pain with every "+
		   "breath "+(tp)->query_pronoun()+" takes.\n"}),
		MSG_SEE);
	break;

      case 5:
	tp->catch_tell("With a mighty clash your lungs explode. "+
		       "You want to breath a last time,\n"+
		       "but fail!\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " tries to take a last breath, "+
		   "but something inside of "+(tp)->query_objective()+ 
		   " explodes.\n" }),
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
    return "_jitsutyr_";
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
    return "Jitsutyr, it destroys your lungs";
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
        return "You feel your breathing is abnormal.\n";
    return capitalize(tp->query_pronoun()) + " breathes hardly.\n";
}
