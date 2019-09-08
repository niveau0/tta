/*
 * muilfana poison
 */
inherit "/base/poison";

#include <effects.h>
#include <event.h>
#include <macros.h>
#include <rr_types.h>
#include <properties.h>
#include <state.h>

#define INTERVAL 1 
#define LEVEL    2

static void
renew_poison(object tp)
{
    if (map_sizeof(tp->query_effects()[BLIND_EFFECT]) )
    {
	tp->catch_tell("Outch! Something burns your blind eyes.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
	    ( { QCTNAME(tp), " rubs "+(tp)->query_possessive()+
	    " eyes.\n" }),
	    MSG_SEE);
	tp->check_death(random(10)+2, this_object());
    }
    else
    {
	tp->catch_tell("Outch! Something burns your damaged windpipe.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
	    ( { QCTNAME(tp), " coughs and spits some blood.\n" }),
	    MSG_SEE);
	tp->check_death(random(20)+ 20, this_object());
    }
}
/*
 * Function name: poison_effect
 * Description:   this must be redefined to create a poison effect
 * Arguments:     tp - the affected player
 */
static nomask void
poison_effect(object tp)
{
    int count, if_dead, sleep;

    if (!tp || !(poisoned_players[tp]) || !living(tp))
	return;

    if (tp->query_state() & (SLEEPING | UNCONSCIOUS))
	sleep = 1;
	
    if (((poisoned_players[tp]++) == 1) && (random (2)) )
    {
	if (!sleep)
	{
	    if (!map_sizeof(tp->query_effects()[BLIND_EFFECT]))
	    {
		tp->catch_tell("You feel your eyes changing. Everything " +
			       "seems to dim.\n");
		tell_objects(LISTEN(E(tp)) - ({ tp }),
			     ({ QCTNAME(tp), " seems to look through " +
				    "everything.\n" }),
			     MSG_SEE);
	    }
	    else
		tp->catch_tell("You feel your eyes changing.\n");
	}
	tp->add_effect(BLIND_EFFECT, EFF_MUILFANA, 1);
	poisoned_players[tp] = -1;
	return;
    }

    call_out("poison_effect", random(150), tp);
   
    if (tp->resistance_roll(RR_POISON, LEVEL, -80) > 0)
    {
	remove_poison(tp);
	if (!sleep)
	    tp->catch_tell("You can breath more easily now.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " seems to breath more easily.\n" }),
		   MSG_SEE);
	return;
    }

    tp->add_effect(POISON_EFFECT, MASTER, count);
 
    count = 1+random(5);
    switch(count)
    {
      case 1:
	tp->catch_tell("Breathing is very difficult now!\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " breathes in an abnormal way.\n" }),
	             MSG_SEE);
	if_dead = tp->check_death(1+random(5), this_object());
	break;
     
      case 2:
	tp->catch_tell("Something is burning down your windpipe."+
	               " It is very hard to breath!\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " seems to have problems with "+
			    (tp)->query_possessive()+" breathing.\n" }),
	             MSG_SEE);
	if_dead = tp->check_death(1+random(8), this_object());
	break;

      case 3:
	tp->catch_tell("It seems that you drunk some liquid steel!\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " breathes very hard.\n" }),
	             MSG_SEE);
	if_dead = tp->check_death(2+random(9), this_object());
	break;

      case 4:
	tp->catch_tell("Are you sure, you didn't eat a dagger? "+
	  	       "The pain\nyou receive says you did.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " pulls faces of pain with every "+
			    "breath "+(tp)->query_pronoun()+" takes.\n"}),
	             MSG_SEE);
	if_dead = tp->check_death(2+random(12), this_object());
	break;

      case 5:
	tp->catch_tell("You seem to be asthmatic. You whistle "+
		       "with every breath you take!\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " breathes asthmatic. With every "+
			    "breath "+(tp)->query_pronoun()+" takes, "+
			    (tp)->query_pronoun()+
			    " makes a quiet whistle.\n" }), MSG_SEE);
	if_dead = tp->check_death(3+random(15), this_object());
	break;
    } /* end of switch */
    
    if (if_dead == 1)
	remove_poison(tp);
}

/*
 * Function name: query_subdesc_id
 * Description:   gives name of subdesc
 * Returns:       string
 */
nomask string
query_subdesc_id()
{
    return "_muilfana_";
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
    return "Muilfana, it destroys your windpipe or eyes.";
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
    if (map_sizeof(tp->query_effects()[BLIND_EFFECT]))
	return "";
    if (tp == for_ob)
	return "You feel your breathing is abnormal.\n";
    return capitalize(tp->query_pronoun()) + " breathes hardly.\n";
}

void
remove_poison(object tp)
{
    if (poisoned_players[tp] == -1)
    {
	tp->remove_effect(BLIND_EFFECT, EFF_MUILFANA);
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
    else
    {
	if (living(tp))
	{
	    if (!(tp->query_state() & (SLEEPING | UNCONSCIOUS)))
		tp->catch_tell("You feel your windpipe ok again.\n");
    	    tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " breathes more easily.\n" }),
		MSG_SEE);
	}
    }
    ::remove_poison(tp);
    return;
}
