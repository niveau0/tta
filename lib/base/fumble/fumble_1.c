#include "fumble.h"
#include <material.h>

void remove_paralyse(object player);

// fumbles without a weapon

/*
 * slipping			OK  0-2
 * heavy slipping		OK  3-4
 * on ground			OK  5-6
 * damage self			OK  7-8
 * stunned			OK  9-10
 * cuts self			OK 11-12
 * paralyse			OK 13-14
 * foe blocks			OK 15-16
 * breaks/shatters slot		OK default
 */

static int
check_metal_armor(int slot, object player)
{
    int    i;
    object *armor;

    for (i=sizeof(armor = player->query_worn(slot)); i--;)
	if (armor[i]->query_prop(OBJ_MATERIAL) >= M_METAL)
	    return 1;
    return 0;
}


void
fumble(int skill, int slot, object player, object target, int step)
{
    string possessive, pronoun, slot_name;
     
    possessive = player->query_possessive();
    pronoun = player->query_pronoun();
    slot_name = player->query_slot_desc(slot);

    switch(step)
    {
      case 0..2:
	  ATB(({ CB_DB_BONUS }),
	      ({  ([ R3 : step*(-10) ])  }));
	  AT("When you try to hit your foe you suddenly slip.\n",
	     player, MSG_SEE, 0);
	  AOBS("Your foe suddenly slips.\n", 0, MSG_SEE, 0);
	  TW(({ QCTNAME(player), " suddenly slips.\n" }),
	     player, target);
	  return;

      case 3..4:
	  ATB(({ CB_DB_BONUS , CB_STUNNED }),
	      ({  ([ R3 : -30 ]), ([ R2 : 1 ]) }));
	  AOB(({ CB_OB_BONUS }), ({  ([ R3 : 30 ]) }));
	  if (player->query_state() & ON_GROUND)
	  {
	      player->check_death(random(3)+1);
	      return;
	  }
	  AT("When you try to hit your foe with full impact you trip and "+
	     "have\nto concentrate to get a good stand again.\n",
	     player, MSG_SEE, 0);
	  AOBS("Your foe suddenly slips badly and has problems to get "+
	       possessive+" good stand again.\n", 0, MSG_SEE, 0);
	  TW(({ QCTNAME(player), " suddenly slips badly and has problems "+
	            "to get "+possessive+" good stand again.\n" }),
	     player, target);
	  return;
      
      case 5..6:
	  ATB(({ CB_DB_BONUS , CB_STUNNED }),
	      ({  ([ R3 : -40 ]), ([ R2 : 1 ]) }));
	  AOB(({ CB_OB_BONUS }), ({  ([ R3 : 40 ]) }));
	  if (player->query_state() & ON_GROUND)
	  {
	      player->check_death(random(3)+1);
	      return;
	  }
	  AT("When you try to hit your foe with full impact you trip and "+
	     "fall flat\non your face.\n", player, MSG_SEE, 0);
	  AOBS("Your foe suddenly slips badly and falls flat on "+
	       possessive+" face.\n", 0, MSG_SEE, 0);
	  TW(({ QCTNAME(player), " suddenly slips badly and falls flat on "+
	            possessive+" face.\n" }), player, target);
	  player->check_death(random(3)+1);
	  player->add_state(ON_GROUND);
	  return;
      
      case 7..8:
	  ATB(({ CB_OB_BONUS, CB_DB_BONUS }),
	      ({  ([ R3 : -40 ]), ([ R2 : -15 ])  }));
	  AT("Ouch, that hurts, when you hit your opponent with your "+
	     slot_name+".\n", player, MSG_SEE, 0);
	  AOBS("Your foe suddenly pulls back "+possessive+
	       " "+slot_name+".\n", 0, MSG_SEE, 0);
	  player->check_death(random(step)+1);
	  return;

      case 9..10:
	  ATB(({ CB_OB_BONUS, CB_DB_BONUS , CB_STUNNED }),
	      ({  ([ R6 : -45 ]), ([ R3 : -35 ]), ([ R3 : 1 ])  }));
	  AOB(({ CB_OB_BONUS }), ({  ([ R3 : 40 ]) }));
	  AT("Suddenly a really big dark and evil shadow seems to "+
	     "digest you.\nThis takes your full concentration.\n", 
	     player, MSG_SEE, 0);
	  AOBS("Your foe suddenly glances through you and stops "+
	       "hitting you.\n", 0, MSG_SEE, 0);
	  TW(({ QCTNAME(player), " suddenly glances around and stops "+
		    "hitting "+possessive+" foe.\n" }), player, target);
	  return;

      case 11..12:
	  ATB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({  ([ R3 : -25 ]), ([ R3 : 1 ])  }));
	  AT("With an unlucky attack you cut your "+
	     slot_name+".\n", player, MSG_SEE, 0);
	  AOBS("Your foe attacks you but cuts "+possessive+
	       " "+slot_name+".\n", 0, MSG_SEE, 0);
	  TW(({ QCTNAME(player), " attacks "+possessive+" opponent, "+
		    "but only cuts "+possessive+" "+slot_name+".\n" }),
	     player, target);
	  player->add_bloodloss((random(5)+1), slot);
	  player->check_death(random(3)+1);
	  return;

      case 13..14:
	  AT("When you strike out you dislocate your neck and are "+
	     "somehow paralyzed.\n", player, MSG_SEE, 0);
	  AOBS("Your opponent strikes out and dislocates "+
	       possessive+" neck. Then"+
	       pronoun+" looks somehow paralyzed.\n",
	       0, MSG_SEE, 0);
	  TW(({ QCTNAME(player), " strikes out and dislocates "+
		  possessive+" neck. Then "+
		  pronoun+" looks somehow paralyzed\n" }),
		  player, target);
	  player->add_state(PARALYSED);
	  player->add_effect(PARALYSE_EFFECT, EFF_FUMBLE_PARALYSE, 1);
	  call_out("remove_paralyse", R3-time(), player);
	  return;

      case 15..16:
	  ATB(({ CB_OB_BONUS, CB_DB_BONUS , CB_STUNNED }),
	      ({  ([ R12 : -50 ]), ([ R2 : -25 ]), ([ R3 : 1 ]) }));
	  if(check_metal_armor(slot, player))
	  {
	      AT("Your foe blocks your attack with your "+slot_name+
	         ". Your hard armor absorbs most of the damage of the "+
		 "block.\n", player, MSG_SEE, 0);
	      AOBS("Your foe attacks you with a clumsy move but you "+
		   "are able\nto block the attack.\n", 0, MSG_SEE, 0);
	      TW(({ QCTNAME(player), " attacks "+possessive+" foe with "+
		        "clumsy move, but "+target->query_pronoun()+
			" blocks the move.\n" }), player, target);
	      player->check_death(random(5)+1);
	  }
	  else
	  {
	      AT("You hit your foe with your "+slot_name+
	         ", but this unlucky move breaks your "+
	         slot_name+".\n", player, MSG_SEE, 0);
	      AOBS("Your foe attacks you with "+possessive+" "+
	           slot_name+" that breaks with a loud *crack*.\n", 
		   0, MSG_SEE, 0);
	      TW(({ QCTNAME(player), " attacks "+possessive+" foe with "+
		        possessive+" "+slot_name+", that breaks with a "+
		        "loud *crack*.\n" }), player, target);
	      player->check_death(random(6)+2);
	      player->set_wound(slot,SLOT_BROKEN);
	  }
	  return;

      default:
	  ATB(({ CB_OB_BONUS, CB_DB_BONUS , CB_STUNNED }),
	      ({  ([ R6 : -45 ]), ([ R3 : -35 ]), ([ R3 : 1 ])  }));
	  AOB(({ CB_OB_BONUS }), ({  ([ R3 : 40 ]) }));
	  AT("When your blow would have hit your foe, it could "+
	     "immediately\nkill - but only could kill. It misses "+
	     "and the full impact of\nthis blow hits you, when you "+
	     "stumble and fall on your face.\n", player, MSG_SEE, 0);
	  AOBS("Your foe attacks you with a mighty blow, but "+
	       pronoun+" stumbles and hits the ground badly.\n", 
	       0, MSG_SEE, 0);
	  TW(({ QCTNAME(player), " attacks "+possessive+" foe with "+
	            "a mighty blow, but "+pronoun+" stumbles and "+
		    "hits the ground badly.\n" }), player, target);
	  if(check_metal_armor(slot, player))
	  {
	      player->check_death(random(4)+2);
	      player->set_wound(slot,SLOT_BROKEN);
	  }
	  else
	  {
	      player->check_death(random(7)+3);
	      player->set_wound(slot,SLOT_SHATTERED);
	  }
	  player->add_bloodloss((random(5)+2), slot);
	  return;
    }
}

void
remove_paralyse(object player)
{
    player->remove_effect(PARALYSE_EFFECT, EFF_FUMBLE_PARALYSE);
     
    if(living(player))
    {
        if (!map_sizeof(player->query_effects()[PARALYSE_EFFECT]))
        {
    	    player->catch_tell("Great, you can move again.\n");
	    tell_objects(LISTEN(E(player)) - ({ player }),
		         ({ QCTNAME(player),
			    " seems to be more dexterous.\n" }),MSG_SEE);
	    player->delete_state(PARALYSED);
        }
	else
	    player->catch_tell("Something evil has left your body, "+
			       "but you are still paralysed.\n");
    }
    else
	if (!map_sizeof(player->query_effects()[PARALYSE_EFFECT]))
	    player->delete_state(PARALYSED);
}
