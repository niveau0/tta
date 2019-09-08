inherit "/base/curse";

#include <effects.h>
#include <event.h>
#include <language.h>
#include <living.h>
#include <macros.h>
#include <rr_types.h>
#include <properties.h>
#include <state.h>

#define INTERVAL 100 + random(50)
#define LEVEL    30

/* Check if player is able to speak not made.
 */


static object get_victim(object tp, int rm);

static nomask void
curse_effect(object tp)
{
    object victim;
    
    if (!tp || !(cursed_players[tp]) || !living(tp))
	return;

    call_out("curse_effect", INTERVAL, tp);

    if (tp->query_state() & (SLEEPING | UNCONSCIOUS))
	return;
	
    switch(random(10))
    {
      case 1:
	  victim = get_victim(tp,4);
	  if(victim)
	  {
	      if(!tp->query_met(victim->query_name()))
		  tp->command( "spit " + victim->query_nonmet_name() );
	      else
		  tp->command( "spit " + victim->query_name() );
	  }
	  else
	  {
	      tp->command("spit");
	  }
	  break;

      case 2:
	  victim = get_victim(tp,10);
          tell_objects(LISTEN(E(tp)) - ({ tp, victim }),
		       ({ QCNAME(tp), " makes a rude comment towards ",
			  QNAME(victim),
		          ".\n" }), MSG_HEAR, tp, 700);
	  tell_objects(({ victim }),
		       ({ QCNAME(tp),
		       " makes a rude comment towards you.\n" }),
		       MSG_HEAR, tp, 700);
	  tell_objects(({ tp }),
		       ({ "You make a short comment towards ",
			      QNAME(victim), ".\n" }),
		              MSG_HEAR, tp, 700);
	  break;
	  
      case 3:
	  tp->command("fart");
	  break;
	  
      case 4:
	  victim = get_victim(tp,3);
	  if(victim)
	  {
	      if(tp->query_met(victim->query_name()))
		  tp->command( "poke " + victim->query_name() );
	      else
		  tp->command( "poke " + victim->query_nonmet_name() );
	  }
	  break;
	  
      case 5:
          tell_objects(LISTEN(E(tp)) - ({ tp }),
		       ({ QCTNAME(tp), " sticks a finger into his nose and"+
			      " digs for a while.\n" }), MSG_SEE, tp);
	  tell_objects(({ tp }),
		       ({ "You scratch a nasty itch in your nose.\n" }),
		              MSG_SEE, tp);
	  break;
	  
      case 6:
	  victim = get_victim(tp,50);
	  if(victim)
	  {
	      if(tp->query_met(victim->query_name()))
		  tp->command( "tongue " + victim->query_name() );
	      else
	      {
		  tp->command( "tongue " + victim->query_nonmet_name() );
	      }
	  }
	  else
	  {
	      tp->command( "tongue" );
	  }
	  break;

      case 7:
	  tp->command("burp");
	  break;
	  
      case 8:
          tell_objects(LISTEN(E(tp)) - ({ tp }),
		       ({ QCTNAME(tp), " scratches "+
			      tp->query_possessive() +" ass.\n" }),
		       MSG_SEE, tp);
	  tell_objects(({ tp }),
		       ({ "Something itches your lower back.\n" }),
		              MSG_SEE, tp);
	  break;
	  
      case 9:
	  victim = get_victim(tp,2);
	  if(victim)
	  {
	      if(tp->query_met(victim->query_name()))
		  tp->command( "puke " + victim->query_name() );
	      else
	      {
		  tp->command( "puke " + victim->query_nonmet_name() );
	      }
	  }
	  else
	  {
	      tp->command( "puke" );
	  }
	  break;

      default :
	  /* Lautstaerke ueberdenken! */
	  {
	      tell_objects(LISTEN(E(tp)) - ({ tp }),
			   ({ QCTNAME(tp), " insults everyone and everything"+
				  " in the surrounding.\n" }),
			   MSG_HEAR, tp, 1000);
	      tell_objects(({ tp }),
		       ({ "You have a good swear about your situation.\n" }),
		       MSG_HEAR, tp, 1000);
	  }
	  break;
    }
}



/*  function:    get_victim
    parameters:  tp=this_player, rm=range_modifier
    description: searches a 'victim' in the range of tp
    returns:     an living in range or 0
 */

static object get_victim(object tp, int rm)
{
    int i, j, step;
    int *co, *range;
    object *seen_players;

    seen_players = I(E(tp), 2) - ({ tp });
    co = tp->query_coordinates()[..];
    step = tp->query_step();
    co[0] -= step;
    co[1] -= step;
    co[2] -= step;
    range = ({ step*rm, step*rm, step*rm });

    for (i=sizeof(seen_players);i--;)
    {
	if(!living(seen_players[i]))
	    seen_players[i] = 0;
    
	if(!in_range(seen_players[i]->query_coordinates(), co, range))
            seen_players[i] = 0;
    }
    seen_players -= ({ 0 });
    seen_players = tp->visible(seen_players);
    if (!sizeof(seen_players))
    {
	return 0;
    }
    else
    {
	j = random(sizeof(seen_players));
        return seen_players[j];
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
    return "_roughness_";
}

/*
 * Function name: query_level
 * Description:   gives curse level
 * Returns:       int
 */
nomask int
query_level()
{
    return LEVEL;
}
     
/*
 * Function name: query_desc
 * Description:   gives curse description
 * Returns:       string
 */
nomask string
query_desc()
{
    return "lets you behave like an idiot";
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
