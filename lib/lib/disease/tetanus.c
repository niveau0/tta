/*
 * tetanus disease
 */
inherit "/base/disease";

#include <effects.h>
#include <event.h>
#include <macros.h>
#include <rr_types.h>
#include <slots.h>
#include <properties.h>
#include <state.h>

#define INTERVAL 100+random(100)
#define LEVEL    1

static mapping tetanus_slots;

/* called by macro, see <effects.h> */
void
infected_slot(object tp, int slot)
{
    if (!infected_players[tp])
	return;

    if (!tetanus_slots)
	tetanus_slots = ([ tp: ({ slot }) ]);
    else if (!tetanus_slots[tp])
	tetanus_slots[tp] = ({ slot });
}

/*
 * Function name: disease_effect
 * Description:   this must be redefined to create a disease effect
 * Arguments:     tp - the affected player
 */
static nomask void
disease_effect(object tp)
{
    int     count, i, upper_slot, current_wound, sleep,
            *abdomen, *slot_connects;
    mapping old_wounds;
    
    if (!tp || !infected_players[tp] || !living(tp))
	return;
    if (!mappingp(tetanus_slots))
	tetanus_slots = ([]);
    
    count = infected_players[tp]++;
    abdomen = tp->query_slot_type(SLT_ABDOMEN);
    
    if (count > 10)
    {
	if (tp->resistance_roll(RR_DISEASE, LEVEL, 0) > 0)
	{
	    remove_disease(tp);
	    return;
	}
    }
    call_out("disease_effect", INTERVAL, tp);
    tp->add_effect(DISEASE_EFFECT, MASTER, count);

    old_wounds = tp->query_wounds();
    current_wound = tetanus_slots[tp][0];

    if(tp->query_state() & (SLEEPING | UNCONSCIOUS))
	sleep = 1;
    
    switch (count)
    {
      case 1..4:
	  if(sleep)
	  {
	      tp->catch_tell("You feel ill.\n");
	      break;
	  }
	  tp->catch_tell("You see a red line heading from your "+
			 tp->query_slot_desc(current_wound)+
			 " to your heart.\n");
	  tell_objects(LISTEN(E(tp)) - ({ tp }),
		       ({ QCTNAME(tp), " looks at " + tp->query_possessive() +
			      " " + tp->query_slot_desc(current_wound) +
			      ".\n" }), MSG_SEE);
	  break;
      case 5..8:
	  tp->catch_tell("Your bleeding wound on your "+
		         tp->query_slot_desc(current_wound)+" hurts.\n");
	  if(!sleep)
	      tell_objects(LISTEN(E(tp)) - ({ tp }),
			   ({ QCTNAME(tp), " looks at " +
				  tp->query_possessive() +
				  " " + tp->query_slot_desc(current_wound) +
				  ".\n" }), MSG_SEE);
	  break;
      case 9:
	  if(SLOT_TYPE(current_wound) == SLT_ABDOMEN)
	  {
	      tp->catch_tell("You are really ill. You should search for " +
			     "a good healer.\n");
	      if(!sleep)
		  tell_objects(LISTEN(E(tp)) - ({ tp }),
			       ({ QCTNAME(tp), " asks you for a good " +
				      "healer.\n" }),
			       MSG_SEE);
	      tp->check_death(random(3)+1);
	  }
          else
	  {    
	      if (!(tp->query_wounds()[current_wound]))
	      {    
		  if(!sleep)
		  {
		      tp->catch_tell("You can't move your "+
				     tp->query_slot_desc(current_wound)+
				     " anymore. The red line heads on to "+
				     "your heart.\n");
		      tell_objects(LISTEN(E(tp)) - ({ tp }),
				   ({ QCTNAME(tp),
					  " seems to lose the control over "+
					  tp->query_possessive() +
					  " " +
					  tp->query_slot_desc(current_wound)+
					  ".\n" }), MSG_SEE);
		  }
		  else		  
		      tp->catch_tell("Your " +
				     tp->query_slot_desc(current_wound)+
				     " hurts like hell.\n");
		  slot_connects = tp->query_connections(current_wound);
		  for (i=sizeof(slot_connects);i--;)
		  {
		      if (!(old_wounds[slot_connects[i]]))
			  tetanus_slots[tp] += ({ slot_connects[i] });
		  }
	          tp->set_wound(current_wound,SLOT_UNUSABLE);
	      }
	      else
	      {
	          tp->catch_tell("Your " + tp->query_slot_desc(current_wound)+
			         " hurts like hell.\n");
	          tell_objects(LISTEN(E(tp)) - ({ tp }),
			       ({ QCTNAME(tp), " writhes from pain.\n" }),
			       MSG_SEE);
	      }
	  }
	  break;
      case 10..13:
	  if (!(upper_slot = tp->query_reconnect(current_wound)))
	  {
	      tp->catch_tell("You are really ill. You should search for a " +
			     "good healer.\n");
	      if (!sleep)
		  tell_objects(LISTEN(E(tp)) - ({ tp }),
			       ({ QCTNAME(tp), " asks you for a good "+
				      "healer.\n" }),
			       MSG_SEE);
	      tp->check_death(random(4)+1);
	  }
          else
	  {
	      if (!sleep)
	      {
		  tp->catch_tell("Your " + tp->query_slot_desc(upper_slot) +
				 " is itching. The red line heads on here.\n");
		  tell_objects(LISTEN(E(tp)) - ({ tp }),
			       ({ QCTNAME(tp), " scratches " +
				      tp->query_possessive() + " " +
				      tp->query_slot_desc(upper_slot) +
				      ".\n"}),
			       MSG_SEE);
	      }
	  }
	  break;
      case 14:
	  if (!(upper_slot = tp->query_reconnect(current_wound)))
	  {
	      tp->catch_tell("You are really ill. You should search for a " +
			     "good healer.\n");
	      if (!sleep)
		  tell_objects(LISTEN(E(tp)) - ({ tp }),
			       ({ QCTNAME(tp), " asks you for a good " +
				      "healer.\n" }),
			       MSG_SEE);
	      tp->check_death(random(4)+2);
	  }
          else
	  {
	      if (!(tp->query_wounds()[upper_slot]))
	      {
		  if (!sleep)
		  {
		      tp->catch_tell("You can't move your "+
				     tp->query_slot_desc(upper_slot)+
				     " anymore. The red line heads on to "+
				     "your heart.\n");
		      tell_objects(LISTEN(E(tp)) - ({ tp }),
				   ({ QCTNAME(tp),
					  " seems to lose the control over "+
					  tp->query_possessive()+
					  " " +
					  tp->query_slot_desc(upper_slot)+
					  ".\n" }),
				   MSG_SEE);
		  }
		  slot_connects = tp->query_connections(upper_slot);
		  tetanus_slots[tp] += ({ upper_slot });
		  for(i=sizeof(slot_connects);i--;)
		  {
		      if(!(old_wounds[slot_connects[i]]))
			  tetanus_slots[tp] += ({ slot_connects[i] });
		  }
		  tp->set_wound(upper_slot, SLOT_UNUSABLE);
	      }
	      else
	      {
	          tp->catch_tell("Your " + tp->query_slot_desc(upper_slot)+
			         " hurts like hell.\n");
	          tell_objects(LISTEN(E(tp)) - ({ tp }),
			       ({ QCTNAME(tp), " writhes from pain.\n" }),
			       MSG_SEE);
	      }
	  }
	  break;
      default:
	  if ((random(100) - count + 15) <= 0)
    	  {
	      tp->do_die();
	      return;
    	  }
	  tp->catch_tell("You are deadly ill. Your infected wound on your "+
			 tp->query_slot_desc(current_wound)+
			 " burns like hell.\n");
	  tell_objects(LISTEN(E(tp)) - ({ tp }),
		       ({ QCTNAME(tp), " seems to be deadly ill.\n" }),
		       MSG_SEE);
	  tp->check_death(random(5) + 3);
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
    return "_tetanus_";
}

nomask string
subdesc(object tp, object for_ob)
{
    if (!infected_players[tp] || tp != for_ob)
	return "";
    return "A red line heads from your " +
	tp->query_slot_desc(tetanus_slots[tp][0]) + " to your heart.\n";
}

/*
 * Function name: query_level
 * Description:   gives disease level
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
 * Description:   gives disease description
 * Returns:       string
 */
nomask string
query_desc()
{
    return "Tetanus will paralyse your infected limbs and after"+
	" a while cause death";
}
  
void
remove_disease(object tp)
{
    int i;
    
    ::remove_disease(tp);

    if (living(tp))
    {
	tp->catch_tell("The red line disappears from your "+
		       tp->query_slot_desc(tetanus_slots[tp][0])+".\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp)," seems to be glad.\n" }),
		     MSG_SEE);
    }
    for(i=sizeof(tetanus_slots[tp]); i--;)
	tp->set_wound(tetanus_slots[tp][i], 0);
    tetanus_slots[tp] = 0;
}
