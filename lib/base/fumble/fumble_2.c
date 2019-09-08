/*
 * melee weapon fumble
 */

#include "fumble.h"

void remove_paralyse(object player);

/*
 * loosing grip			OK  0-2
 * slipping			OK  3-4
 * loosing grip badly		OK  5-6
 * drop weapon			OK  7-8
 * throw weapon away		OK  9-10
 * fall down			OK 11-12
 * paralyse			OK 13-14
 * breaks weapon		OK 15-16
 * fall down	                OK 17
 * hits self			OK 18
 * unconscious+break weapon	OK default
 */


void
fumble(int skill, object weapon, object player, object target, int step)
{
    int* i;
    int slot;
    string possessive, pronoun;

    possessive = player->query_possessive();
    pronoun = player->query_pronoun();

    switch (step)
    {
      case 0..2:
	  ATB(({ CB_DB_BONUS }),
	      ({  ([ R2 : step*(-20) ]) }));
	  AT("You momentary lose the grip of your weapon.\n",
	     player, MSG_SEE, 0);
	  AOBS("Your opponent momentary loses the grip of "+
	       possessive+" weapon.\n", 0, MSG_SEE, 0);
	  TW(({ QCTNAME(player), " momentary loses the grip of "+
		    possessive+" weapon.\n" }),
	     player, target);
	  return;
      case 3..4:
	  ATB(({ CB_DB_BONUS , CB_STUNNED }),
	      ({  ([ R3 : -25 ]), ([ R1 : 1]) }));
	  AOB(({ CB_OB_BONUS }), ({  ([ R3 : 30 ]) }));
	  if (player->query_state() & ON_GROUND)
	      return;
	  AT("You slip but luckily can get the balance again.\n",
	     player, MSG_SEE, 0);
	  AOBS("Your opponent slips and tries to get a good "+
	       "stand again.\n", 0, MSG_SEE, 0);
	  TW(({ QCTNAME(player), " slips but doesn't fall.\n" }),
	     player, target);
	  return;
      case 5..6:
	  ATB(({ CB_DB_BONUS , CB_OB_BONUS , CB_STUNNED }),
	      ({  ([ R3 : -30 ]), ([ R3 : -30 ]), ([ R3 : 1]) }));
	  AT("You suddenly lose the grip of your weapon and aren't "+
	     "able\nto get a good grip again in a second.\n",
	     player, MSG_SEE, 0);
	  AOBS("Your opponent suddenly loses the grip of "+
	       possessive+" weapon and juggles it some "+
	       "time before "+pronoun+" gets a good "+
	       "grip again.\n", 0, MSG_SEE, 0);
	  TW(({ QCTNAME(player), " suddenly loses the grip of "+
		    possessive+" weapon and juggles "+
		    "it sometimes before "+pronoun+
		    " gets a good grip again.\n" }), player, target);
	  return;
      case 7..8:
	  ATB(({ CB_DB_BONUS , CB_STUNNED }),
	      ({  ([ R4 : -50 ]), ([ R4 : 1]) }));
	  if (call_event(E_REMOVE_TOOL, player, weapon))
	  {
	      AT("Your weapon vibrates in your hands.\n",
		 player, MSG_SEE, 0);
	      AOBS("Your opponent suddenly loses the grip of "+
		   possessive+" weapon.\n", 0, MSG_SEE, 0);
	      TW(({ QCTNAME(player), " suddenly loses the grip of "+
			possessive+" weapon.\n" }),
		 player, target);
	      return;
	  }
	  AT("You suddenly lose the grip of your weapon and drop it "+
	     "to your feet.\n", player, MSG_SEE, 0);
	  AOBS("Your opponent suddenly loses the grip of "+
	       possessive+" weapon and drops it to "+
	       possessive+" feet.\n", 0, MSG_SEE, 0);
	  TW(({ QCTNAME(player), " suddenly loses the grip of "+
		    possessive+" weapon and drops it to "+
		    possessive+" feet.\n" }), player, target);
	  weapon->set_coordinates(player->query_coordinates());
	  weapon->move(E(player));
	  return;	  
      case 9..10:
	  ATB(({ CB_DB_BONUS , CB_STUNNED }),
	      ({  ([ R4 : -50 ]), ([ R4 : 1]) }));
	  if (call_event(E_REMOVE_TOOL, player, weapon))
	  {
	      ATB(({ CB_OB_BONUS }),
		  ({  ([ R4 : -50 ]) }));
	      AT("You suddenly lose the grip of your weapon.\n",
		 player, MSG_SEE, 0);
	      AOBS("Your opponent suddenly loses the grip of "+
		   possessive+" weapon.\n", 0, MSG_SEE, 0);
	      TW(({ QCTNAME(player), " suddenly loses the grip of "+
			possessive+" weapon.\n" }),
		 player, target);
	      return;
	  }
	  AT("You suddenly lose the grip of your weapon and drop it.\n",
	     player, MSG_SEE, 0);
	  AOBS("Your opponent suddenly loses the grip of "+
	       possessive+" weapon and drops it.\n", 0, MSG_SEE, 0);
	  TW(({ QCTNAME(player), " suddenly loses the grip of "+
		    possessive+" weapon and drops it.\n" }),
	     player, target);
	  i=move_weapon(player, random(3)+1);
	  weapon->set_coordinates(i);
	  weapon->move(E(player));
	  return;	  
      case 11..12:
	  ATB(({ CB_DB_BONUS , CB_STUNNED }),
	      ({  ([ R3 : -40 ]), ([ R3 : 1]) }));
	  AOB(({ CB_OB_BONUS }), ({  ([ R3 : 30 ]) }));
	  if (player->query_state() & ON_GROUND)
	     return;
	  AT("You trip and fall flat on your face.\n",
	     player, MSG_SEE, 0);
	  AOBS("Your opponent trips and falls flat on the face.\n",
	       0, MSG_SEE, 0);
	  TW(({ QCTNAME(player), " trips and falls flat on "+
		    possessive+" face.\n" }),
	     player, target);
	  player->add_state(ON_GROUND);
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
	  ATB(({ CB_DB_BONUS , CB_STUNNED }),
	      ({  ([ R3 : -40 ]), ([ R2 : 1]) }));
	  AOB(({ CB_OB_BONUS }), ({  ([ R3 : 45 ]) }));
	  if (call_event(E_REMOVE_TOOL, player, weapon))
	  {
	      AT("You trip but luckily don't fall. But unfortunetely you\n" +
		 " break your weapon.\n",
		 player, MSG_SEE, 0);
	      AOBS("Your opponent trips but doesn't fall. Luckily "+
		   pronoun+" breaks "+possessive+ " weapon.\n",
		   0, MSG_SEE, 0);
	      TW(({ QCTNAME(player), " trips but doesn't fall but "+
			pronoun+" breaks "+
			possessive+" weapon.\n" }), player, target);
	  }
	  else
	  {
	      AT("You trip but luckily don't fall. But unfortunetely you\n" +
		 " drop your weapon, that breaks.\n",
		 player, MSG_SEE, 0);
	      AOBS("Your opponent trips but doesn't fall. Luckily "+
		   pronoun+" drops "+possessive+
		   " weapon, that breaks.\n",
		   0, MSG_SEE, 0);
	      TW(({ QCTNAME(player), " trips but doesn't fall but "+
			pronoun+" drops "+
			possessive+
			" weapon, that breaks.\n" }),
		 player, target);
	      weapon->set_coordinates(player->query_coordinates());
	      weapon->move(E(player));
	  }
	  weapon->add_prop(OBJ_DAMAGED, -1);
	  return;
      case 17:
	  ATB(({ CB_DB_BONUS , CB_STUNNED }),
	      ({  ([ R3 : -30 ]), ([ R6 : 1]) }));
	  if (player->query_state() & ON_GROUND)
	  {
	     player->check_death(random(10)+3);
	     return;
	  }
	  if(i = player->query_slot_type(SLT_HAND))
	  {
	      slot = i[random(sizeof(i))];
	      AT("With an unlucky move you stumble. You try to avoid "+
	         "falling\nby firm up with your "+player->query_slot_desc(slot)+
	         ", you hear a loud *crack* and an immense pain comes "+
	         "from your "+player->query_slot_desc(slot)+
		 ".\n", player, MSG_SEE, 0);
	      AOBS("Your opponent stumbles and tries to firm up "+
	           possessive+" fall with "+
	           possessive+" "+player->query_slot_desc(slot)+". When "+
	           pronoun+" hits the ground you hear a "+
	           "loud *crack*.\n", 0, MSG_SEE, 0);
	      TW(({ QCTNAME(player), " stumbles and tries to firm up "+
		        possessive+" fall with "+
		        possessive+" "+player->query_slot_desc(slot)+". When "+
		        pronoun+" hits the ground you hear a "+
		        "loud *crack*.\n" }), player, target);
	      player->set_wound(slot,SLOT_BROKEN);
	  }
	  else
	  {
	      AT("With an unlucky move you stumble and fall flat on your "+
	         "face.\n", player, MSG_SEE, 0);
	      AOBS("Your opponent stumbles and tries to firm up "+
	           possessive+" fall.\n", 0, MSG_SEE, 0);
	      TW(({ QCTNAME(player), " stumbles and tries to firm up "+
		        possessive+" fall.\n" }), player, target);
	  }
	  player->add_state(ON_GROUND);
	  player->check_death(random(10)+3);
	  return;
      case 18:
	  ATB(({ CB_DB_BONUS , CB_STUNNED }),
	      ({  ([ R3 : -50 ]), ([ R1 : 1]) }));
	  AOB(({ CB_OB_BONUS }), ({  ([ R6 : 30 ]) }));
	  AT("You make your opponent laugh when your mighty blow "+
	     "misses\n your foe, but hits yourself.\n",
	     player, MSG_SEE, 0);
	  AOBS("You laugh when the blow of your opponent misses his "+
	       "target and\nhits your foe "+player->query_objective()+
	       "self.\n", 0, MSG_SEE, 0);
	  TW(({ QCTNAME(player), " mighty blow misses "+
		    possessive+" opponent and hits "+
		    player->query_objective()+"self.\n" }),
	     player, target);
	  player->check_death(random(20)+10);
	  return;
      default:
	  if (player->query_state() & ON_GROUND)
	  {
	     AT("Unfortunately you move very bad on "+
	        "your weapon that breaks.\n", player, MSG_SEE, 0);
	     AOBS("Your foe moves very bad on "+
		  possessive+" weapon, that breaks. When "+
		  pronoun+" hits the ground, "+
	          possessive+" eyes.\n", 0, MSG_SEE, 0);
	     TW(({ QCTNAME(player), " suddenly moves very bad on "+
		       possessive+" weapon that breaks. "+
		       "When "+pronoun+
		       " hits the ground, " +pronoun+
		       " rolls "+possessive+
		       " eyes.\n" }),player, target);
	     if(player->check_death(random(10)+5))
	         return;
	     player->add_unconscious(random(200)+50);
	     weapon->add_prop(OBJ_DAMAGED, -1);
	     return;
	  }
	  AT("You stumble and unfortunately you fall very bad on "+
	     "your weapon.\nYou hit your head and it suddenly goes "+
	     "very dark.\n", player, MSG_SEE, 0);
	  AOBS("Your opponent stumbles and falls on "+
	       possessive+" weapon that breaks. When "+
	       pronoun+" hits "+ possessive+" head "+pronoun+" closes "+
	       possessive+" eyes.\n", 0, MSG_SEE, 0);
	  TW(({ QCTNAME(player), " suddenly stumbles and falls on "+
		    possessive+" weapon that breaks. "+
		    "When "+pronoun+
		    " hits the ground, " +pronoun+
		    " rolls "+possessive+
		    " eyes.\n" }),player, target);
	  if(player->check_death(random(10)+5))
	      return;
	  player->add_unconscious(random(200)+50);
	  weapon->add_prop(OBJ_DAMAGED, -1);
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
				" seems to be more dexterous.\n" }),
			 MSG_SEE);
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
