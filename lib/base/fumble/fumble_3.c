// throw fumbles

#include "fumble.h"

/*
 * loosing grip				OK   0-2
 * slip					OK   3-4
 * drop weapon				OK   5-6
 * throw weapon behind			OK   7-8
 * VERY bad throw, and break wepon	OK   9-11
 * fall down				OK  12-13
 * paralyse				OK  14-15
 * cut self				OK  16-17
 * fall down+break weapon		OK  default
 */

void remove_paralyse(object player);
static nomask int * move_weapon(object tp, int mod);

void
fumble(int skill, object weapon, object player, object target, int step)
{
    string possessive, pronoun;
    int *i;
    int slot;

    possessive = player->query_possessive();
    pronoun = player->query_pronoun();

    switch (step)
    {
      case 0..1:
	  ATB(({ CB_DB_BONUS }),
	      ({ ([ R2 : (step+1)*(-20) ]) }));
	  AT("You momentary lose the grip of your weapon.\n",
	     player, MSG_SEE, 0);
	  AOBS("Your opponent momentary loses the grip of "+
	       possessive+" weapon.\n", 0, MSG_SEE, 0);
          TW(({ QCTNAME(player), " momentary loses the grip of "+
		    possessive+" weapon.\n" }),
	     player, target);
	  return;
      case 2:
	  ATB(({ CB_DB_BONUS , CB_STUNNED }),
	      ({  ([ R3 : -25 ]), ([ R2 : 1]) }));
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
      case 3..4:
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
      case 5..6:
	  ATB(({ CB_DB_BONUS , CB_STUNNED }),
	      ({  ([ R4 : -50 ]), ([ R4 : 1]) }));

	  if (call_event(E_REMOVE_TOOL, player, weapon))
	  {
	      ATB(({ CB_OB_BONUS }),
		  ({  ([ R4 : -50 ]) }));
	      AT("You suddenly lose the grip of your weapon and aren't "+
		 "able\nto get a good grip again in a second.\n",
		 player, MSG_SEE, 0);
	      AOBS("Your opponent suddenly loses the grip of "+
		   possessive+" weapon and juggles it "+
		   "some time before "+pronoun+
		   " gets a good grip again.\n", 0, MSG_SEE, 0);
	      TW(({ QCTNAME(player), " suddenly loses the grip of "+
			possessive+" weapon and juggles "+
			"it sometimes before "+pronoun+
			" gets a good grip again.\n" }), player, target);
              return; 
          } 
	  AT("You suddenly lose the grip of your weapon and drop it.\n",
	     player, MSG_SEE, 0);
          AOBS("Your opponent suddenly loses the grip of "+
	       possessive+" weapon and drops it.\n", 0, MSG_SEE, 0);
          TW(({ QCTNAME(player), " suddenly loses the grip of "+
		    possessive+" weapon and drops it.\n" }),
             player, target);
          weapon->set_coordinates(player->query_coordinates());
	  weapon->move(E(player));
          return;
      case 7..8:
	  ATB(({ CB_DB_BONUS , CB_STUNNED }),
	      ({  ([ R4 : -50 ]), ([ R1 : 1]) }));

	  if (call_event(E_REMOVE_TOOL, player, weapon))
	  {
	      ATB(({ CB_OB_BONUS }),
		  ({  ([ R4 : -50 ]) }));
	      AT("You suddenly lose the grip of your weapon and aren't "+
		 "able\nto get a good grip again in a second.\n",
		 player, MSG_SEE, 0);
	      AOBS("Your opponent suddenly loses the grip of "+
		   possessive+" weapon and juggles it "+
		   "some time before "+pronoun+
		   " gets a good grip again.\n", 0, MSG_SEE, 0);
	      TW(({ QCTNAME(player), " suddenly loses the grip of "+
			possessive+" weapon and juggles "+
			"it sometimes before "+pronoun+
			" gets a good grip again.\n" }), player, target);
              return; 
          }
	  AT("When you strike out, your weapon slips out of your hands."+
	     "\n", player, MSG_SEE, 0);
	  AOBS("Your opponent suddenly loses the grip of "+
	       possessive+" weapon and drops it.\n", 0, MSG_SEE, 0);
	  TW(({ QCTNAME(player), " suddenly loses the grip of "+
		    possessive+" weapon and drops it.\n" }), player, target);
	  i=move_weapon(player, 10);
          weapon->set_coordinates(i);
	  weapon->move(E(player));
          return;
      case 9..10:
	  ATB(({ CB_DB_BONUS , CB_STUNNED }),
	      ({  ([ R4 : -50 ]), ([ R4 : 1]) }));

	  if (call_event(E_REMOVE_TOOL, player, weapon))
	  {
	      ATB(({ CB_OB_BONUS }),
		  ({  ([ R4 : -50 ]) }));
	      AT("You suddenly lose the grip of your weapon and aren't "+
		 "able\nto get a good grip again in a second.\n",
		 player, MSG_SEE, 0);
	      AOBS("Your opponent suddenly loses the grip of "+
		   possessive+" weapon and juggles it "+
		   "some time before "+pronoun+
		   " gets a good grip again.\n", 0, MSG_SEE, 0);
	      TW(({ QCTNAME(player), " suddenly loses the grip of "+
			possessive+" weapon and juggles "+
			"it sometimes before "+pronoun+
			" gets a good grip again.\n" }), player, target);
              return; 
          } 
	  AT("When you strike out, your weapon slips out of your hands."+
	     "\nIt misses your foe by meters and you hear a load *crack*"+
	     " when it\nhits the ground.\n", player, MSG_SEE, 0);
          AOBS("Your opponent suddenly loses the grip of "+
	       possessive+" weapon when "+pronoun+" throws it. When it "+
	       "lands some meters beside of you, you hear it breaking.\n",
	       0, MSG_SEE, 0);
          TW(({ QCTNAME(player), " suddenly loses the grip of "+
		    possessive+" weapon and makes a very bad throw.\n" }),
             player, target);
	  i=move_weapon(target, 5);
          weapon->set_coordinates(i);
	  weapon->move(E(player));
	  weapon->add_prop(OBJ_DAMAGED,-1);
          return;

      case 11..13:
	  ATB(({ CB_DB_BONUS , CB_STUNNED }),
	      ({  ([ R3 : -40 ]), ([ R2 : 1 ]) }));
          AOB(({ CB_OB_BONUS }), ({  ([ R3 : 40 ]) }));
	  if (player->query_state() & ON_GROUND)
	  {
	      player->check_death(random(3)+1);
	      return;
	  }
	  AT("When you strike out you lose your balance and fall flat "+
	     "on your face.\n", player, MSG_SEE,0);
	  AOBS("Your foe suddenly slips badly and falls flat on "+
	       possessive+" face.\n", 0, MSG_SEE, 0);
          TW(({ QCTNAME(player), " suddenly slips badly and falls flat on "+
		    possessive+" face.\n" }), player, target);
          player->check_death(random(3)+1);
	  player->add_state(ON_GROUND);
	  return;

      case 14..15:
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

      case 16..17:
          ATB(({ CB_DB_BONUS, CB_STUNNED }),
              ({  ([ R3 : -25 ]), ([ R3 : 1 ])  }));
	  if(i = player->query_slot_type(SLT_ARM))
	  {
	      slot = i[random(sizeof(i))];
              AT("With an unlucky attack you cut your "+
                 player->query_slot_desc(slot)+".\n", player, MSG_SEE, 0);
              AOBS("Your foe strikes out but cuts "+possessive+
                   " "+player->query_slot_desc(slot)+".\n", 0, MSG_SEE, 0);
              TW(({ QCTNAME(player), " strikes out, "+
                        "but only cuts "+possessive+" "+
			player->query_slot_desc(slot)+
		        ".\n" }), player, target);
              player->add_bloodloss((random(5)+2), slot);
	  }
          player->check_death(random(4)+2);
          return;

      default:
          if (player->query_state() & ON_GROUND)
          {
             AT("Unfortunately you move very bad on "+
                "your weapon that breaks.\n", player, MSG_SEE, 0);
             AOBS("Your opponent moves very bad on "+
                  possessive+" weapon that breaks. When "+
                  pronoun+" hits "+ possessive+" head "+
                  pronoun+" closes "+ possessive + 
		  " eyes.\n", 0, MSG_SEE, 0);
             TW(({ QCTNAME(player), " suddenly moves very bad on "+
                       possessive+" weapon that breaks. "+
                       "When "+pronoun+ " hits the ground, " +pronoun+
                       " rolls "+possessive+ " eyes.\n" }),player, target);
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
               pronoun+" hits "+
               possessive+" head "+
               pronoun+" closes "+
               possessive+" eyes.\n", 0, MSG_SEE, 0);
          TW(({ QCTNAME(player), " suddenly stumbles and falls on "+
                    possessive+" weapon that breaks. "+
                    "When "+pronoun+
                    " hits the ground, " +pronoun+
                    " rolls "+possessive+
                    " eyes.\n" }),player, target);
          if(player->check_death(random(10)+5))
              return;
	  weapon->add_prop(OBJ_DAMAGED, -1);
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
