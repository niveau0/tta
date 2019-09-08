// missile fumble

#include "fumble.h"

void remove_paralyse(object player);

/*
 * drop missile
 * stumble				OK  1
 * slip badly				OK  2
 * VERY bad shot
 * fall down				OK  5-6
 * drop weapon				OK  7-8
 * break missile                        OK  9-10
 * cut self				OK 11-12
 * break weapon				OK 13-14
 * paralyse				OK 15-16
 * unconscious+break weapon		OK default
 */

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
      case 1:
          ATB(({ CB_DB_BONUS }),
              ({ ([ R2 : -20 ]) }));
          AT("You momentary lose the grip of your weapon.\n",
             player, MSG_SEE, 0);
          AOBS("Your opponent momentary loses the grip of "+
               possessive+" weapon.\n", 0, MSG_SEE, 0);
          TW(({ QCTNAME(player), " momentary loses the grip of "+
                    possessive+" weapon.\n" }),
             player, target);
          return;

      case 2..4:
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

      case 5..6:
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

      case 7..8:
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

      case 9..10:
          ATB(({ CB_DB_BONUS}),
              ({  ([ R1 : -15 ]) }));
	  AT("It seems that you missile was a bad one. It breaks before "+
	     "you could shoot it.\n", player, MSG_SEE, 0);
	  AOBS("Your foe breaks "+possessive+" missile with a clumsy "+
	       "maneuver.\n", 0, MSG_SEE, 0);
	  TW(({ QCTNAME(player), " breaks "+possessive+" missile with "+
		    "a clumsy maneuver\n" }), player, target);
	  return;
      case 11..12:
          ATB(({ CB_DB_BONUS, CB_STUNNED }),
              ({  ([ R3 : -25 ]), ([ R3 : 1 ])  }));
          if(i = player->query_slot_type(SLT_ARM))
          {
              slot = i[random(sizeof(i))];
              AT("With an unlucky move you cut your "+
                 player->query_slot_desc(slot)+".\n", player, MSG_SEE, 0);
              AOBS("Your foe cuts "+possessive+
                   " "+player->query_slot_desc(slot)+".\n", 0, MSG_SEE, 0);
              TW(({ QCTNAME(player), " cuts "+possessive+" "+
		        player->query_slot_desc(slot)+ 
			".\n" }), player, target);
              player->add_bloodloss((random(5)+2), slot);
          }
          player->check_death(random(4)+2);
          return;

      case 13..14:
	  ATB(({ CB_DB_BONUS, CB_STUNNED }),
              ({  ([ R4 : -35 ]), ([ R3 : 1 ])  }));
	  AT("Hmm. This weapon seems complexer then you thought - with "+
	     "a *bang* it breaks by your handling.\n", player, MSG_SEE, 0);
	  AOBS("Your foe breaks "+possessive+" weapon with a clumsy "+
	       "maneuver.\n", 0, MSG_SEE, 0);
	  TW(({ QCTNAME(player), " breaks "+possessive+" weapon with "+
		    "a clumsy maneuver\n" }), player, target);
	  weapon->add_prop(OBJ_DAMAGED, -1);
	  return;

      case 15..16:
          AT("When you strike out you dislocate your neck and are "+
             "somehow paralyzed.\n", player, MSG_SEE, 0);
          AOBS("Your opponent strikes out and dislocates "+
               possessive+" neck. Then"+
               pronoun+" loks somehow paralyzed.\n",
               0, MSG_SEE, 0);
          TW(({ QCTNAME(player), " strikes out and dislocates "+
                  possessive+" neck. Then "+
                  pronoun+" loks somehow paralyzed\n" }),
                  player, target);
          player->add_state(PARALYSED);
          player->add_effect(PARALYSE_EFFECT, EFF_FUMBLE_PARALYSE, 1);
          call_out("remove_paralyse", R3-time(), player);
          return;

      default:
          if (player->query_state() & ON_GROUND)
          {
             AT("Unfortunately you move very bad on "+
                "your weapon that breaks.\n", player, MSG_SEE, 0);
             AOBS("Your opponent moves very bad on "+
                  possessive+" weapon that breaks. When "+
                  pronoun+" hits "+ possessive+" head "+
                  pronoun+" closes "+ possessive+" eyes.\n", 0, MSG_SEE, 0);
             TW(({ QCTNAME(player), " suddenly moves very bad on "+
                       possessive+" weapon that breaks. "+
                       "When "+pronoun+ " hits the ground, " +pronoun+
                       " rolls "+possessive+ " eyes.\n" }),player, target);
             if(player->check_death(random(8)+3))
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
               pronoun+" hits "+ possessive+" head "+
               pronoun+" closes "+ possessive+" eyes.\n", 0, MSG_SEE, 0);
          TW(({ QCTNAME(player), " suddenly stumbles and falls on "+
                    possessive+" weapon that breaks. "+
                    "When "+pronoun+ " hits the ground, " +pronoun+
                    " rolls "+possessive+ " eyes.\n" }),player, target);
          if(player->check_death(random(10)+5))
              return;
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

