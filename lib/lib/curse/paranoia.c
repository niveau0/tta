inherit "/base/curse";

#include <effects.h>
#include <living.h>
#include <macros.h>
#include <mudtime.h>
#include <rr_types.h>
#include <properties.h>
#include <state.h>

#define INTERVAL random(150)+50
#define LEVEL    30

static string get_victim(object tp, int rm);

static nomask void
curse_effect(object tp)
{
    string the_victim;
    
    if (!tp || !(cursed_players[tp]) || !living(tp))
	return;

    call_out("curse_effect", INTERVAL, tp);

    if (tp->query_state() & (SLEEPING | UNCONSCIOUS))
    {
	tp->catch_tell("You know - you are not save here anymore.\n");
	return;
    }

    switch(random(22))
    {
      case 1:
	  the_victim = get_victim(tp,3);
	  tp->catch_tell(capitalize(the_victim)+
			 " tried to attack you from behind.\n");
	  break;

      case 2:
	  the_victim = get_victim(tp,4);
	  tp->catch_tell(capitalize(the_victim)+
			 " attacks you!\n");
	  break;

      case 3:
	  the_victim = get_victim(tp,25);
	  tp->catch_tell(capitalize(the_victim)+
			 " looks at you and grins demonically!\n");
	  break;

      case 4:
	  tp->catch_tell("You notice somebody sneaking around you.\n");
	  break;

      case 5:
	  the_victim = get_victim(tp,4);
	  if(the_victim != "someone")
	      tp->catch_tell(capitalize(the_victim) +
			     " lifts a weapon against you, but changes mind "+
			     "thereafter and grins friendly again.\n");
	  else
	  	  tp->catch_tell("You notice "+the_victim+" touching you.\n");
	  break;

      case 6:
	  tp->catch_tell("You feel watched.\n");
	  break;

      case 7:
	  tp->catch_tell("You notice somebody in some distance aiming at "+
			 "you with a bow.\n");
	  break;
      case 8:
	  the_victim = get_victim(tp,15);
	  tp->catch_tell(capitalize(the_victim)+" grins at you demonically "+
			 "and prepares an evil spell.\n");
	  break;
	  
      case 9:
	  tp->catch_tell("Suddenly you feel a strange aura around you.\n");
	  break;

      case 10:
	  tp->catch_tell("Two red gleaming eyes inspect you and disappear " +
			 "after a second.\n");
	  break;

      case 11:
	  tp->catch_tell("You hear somebody calling your name!\n");
	  break;
	  
      case 12:
	  tp->catch_tell("You can't move for a moment. Seems that anyone " +
			 "has the power to stop you.\n");
	  break;
	  
      case 13:
	  tp->catch_tell("A sudden gust of wind makes you shiver. After a " +
			 "second you realize\nthat this wind tried " +
			 "to tell you something.\n");
	  break;
	  
      case 14:
	  the_victim = get_victim(tp,15);
	  tp->catch_tell(capitalize(the_victim)+" suddenly seems to be very "+
			 "unreal. Maybe it's only an illusion ?\n");
	  break;
	  
      case 15:
	  the_victim = get_victim(tp,15);
	  tp->catch_tell(capitalize(the_victim)+"'s eyes glow with a " +
			 "strange red for a second.\n");
	  break;

      case 16:
	  tp->catch_tell("You ask yourself why all insects look at you at " +
			 "the moment.\n");
	  break;

      case 17:
	  tp->catch_tell("Your skin aches like hell.\n");
	  break;

      case 18:
	  if (NIGHT)
	      tp->catch_tell("You hope the sunbeams will reach you soon " +
			     "again. You get a\nvery uneasy feeling at this " +
			     " time of a day ...\n");
	  else
	      tp->catch_tell("You hope the night will come soon. In the " +
			     "brightness of the day\nyou don't feel very " +
			     "well.\n");
	  break;
	  
      case 19:
	  tp->catch_tell("Suddenly your skin begins to glow in a lot of " +
			 "strange colours.\n");
	  break;

      case 20:
	  tp->catch_tell("For a moment a gust of wind hits you from all " +
			 "directions - strange.\n");
	  break;

      case 21:
	  the_victim = get_victim(tp, 10);
	  tp->catch_tell("All livings seem to hail "+the_victim+" for a "+
			 "second.\n");
	  break;
		       
      default :
	  the_victim = get_victim(tp,2);
	  tp->catch_tell("You notice "+the_victim+" touching you.\n");
	  break;
    }
}



/*  function:    get_victim
    parameters:  tp=this_player, rm=range_modifier
    description: searches a 'victim' in the range of tp
    returns:     a living in range or "somebody"
 */

static string
get_victim(object tp, int rm)
{
    int i, j, step;
    int *co, *range;
    string victim_name;
    object victim;
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
	victim_name = "someone";
    }
    else
    {
	j = random(sizeof(seen_players));

	victim = seen_players[j];
	if(!tp->query_met(victim->query_name()))
	{
	    victim_name = "the "+victim->query_nonmet_name();
	}
	else
	{
	    victim_name = capitalize(victim->query_name());
	}
    }
    return victim_name;
}
/*
 * Function name: query_subdesc_id
 * Description:   gives name of subdesc
 * Returns:       string
 */
nomask string
query_subdesc_id()
{
    return "_paranoia_";
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
    return "let you see things happen, that doesn't exist";
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
