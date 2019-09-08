/*
 * slash criticals
 */

#include "crit.h"

static int
crit_head(mixed hloc, object ao, object att, int step)
{
    int     i, sz, flag, *slots;
    object  *armor;
    
    switch (step)
    {
      case 0..4:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R1 : -15 ]), ([ R1 : 1 ]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 4;
	  AO("The hit makes you stumble, you still can parry "+
	     "but your attack is lost.\n", 0, 0, 0);
	  AT("Your opponent stumbles.\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), " stumbles.\n" }), ao, att);
	  return 3;
      case 5..8:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R1 : -50 ]), ([ R1 : 1 ]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 5;
	  AO("The hit knocks you back, the pang seems to delay "+
	     "your reactions.\n", 0, 0, 0);
	  return 4;
      case 9..12:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R12 : -50 ]), ([ R2 : 1 ]) }));
	  ao->add_bloodloss(1, hloc[HITLOC_SLOT]);
	  return 4;
      case 13..16:
	  /* eyes destroyed */
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R12 : -50 ]), ([ R2 : 1 ]) }));
	  if (sz = sizeof(slots = ao->query_connections(hloc[HITLOC_SLOT])))
	  {
	      for (i=sz; i--;)
		  if (SLOT_TYPE(slots[i]) == SLT_EYE)
		  {
		      flag = 1;
		      ao->set_wound(slots[i], SLOT_SHATTERED);
		  }
	      if (flag)
	      {
		  AO("Arg, the hit destroyed your eyes.\n", 0, MSG_SEE, 0);
		  AT("Your blow destroyed your opponent's eyes.\n",
		     ao, MSG_SEE, 0);
		  TW(({ QCTNAME(ao), "'s eyes got damaged.\n" }), ao, att);
	      }
	  }
	  ao->add_bloodloss(1, hloc[HITLOC_SLOT]);
	  if (!random(3))
	      ao->add_unconscious(40);
	  return 5;
      case 17..19:
	  armor = ao->query_protection(hloc[HITLOC_SLOT]);
	  for (i=sizeof(armor); i--;)
	      if (armor[i]->query_prop(OBJ_MATERIAL) >= M_METAL)
	      {
		  AOB(({ CB_DB_BONUS, CB_STUNNED }),
		      ({ ([ R12 : -50 ]), ([ R2 : 1 ]) }));
		  AOBS("Ouch! That would have smashed your " +
		       hloc[HITLOC_DESC] + ". Luckily you wear good armor.\n",
		       0, MSG_SEE, 0);
		  if (!random(5))
		      ao->add_unconscious(40);
		  return 6;
	      }
	  AOBS("*Crack* That blow opened your " + hloc[HITLOC_DESC] +
	       "!\n", 0, MSG_SEE, 0);
	  ATBS("Your blow cuts off half of your opponent's " +
	       hloc[HITLOC_DESC] + ".\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), "'s " + hloc[HITLOC_DESC] + " got cut off.\n" }),
	     ao, att);
	  ao->do_die(att);
	  return 0;
      default:
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 25 ]) }));
	  AT("You cut off the " + hloc[HITLOC_DESC] + ".\n", ao, MSG_SEE, 0);
	  AO("Whoops?!? You're flying through the air...\n", 0, 0, 0);
	  ao->do_die(att);
	  return 0;
    }
}

static int
crit_torso(mixed hloc, object ao, object att, int step)
{
    switch (step)
    {
      case 0..3:
	  return 1;		/* extra damage */
      case 4..6:
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 5 ]) }));
	  AOB(({ CB_STUNNED }), ({ ([ RH : 1 ]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 2;
	  AO("The hit knocks you back, delaying your reactions.\n", 0, 0, 0);
	  return 1;
      case 7..9:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R1 : -10 ]), ([ R1 : 1 ]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 3;
	  AO("The hit makes you stumble, you still can parry "+
	     "but your attack is lost.\n", 0, 0, 0);
	  AT("Your opponent stumbles.\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), " stumbles.\n" }), ao, att);
	  return 2;
      case 10..11:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R2 : -10 ]), ([ R2 : 1 ]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 7;
	  AO("The hit knocks you back, delaying your reactions.\n", 0, 0, 0);
	  return 6;
      case 12..13:
	  ao->add_bloodloss(1, hloc[HITLOC_SLOT]);
	  return 2;
      case 14..15:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R2 : -30 ]), ([ R2 : 1 ]) }));
	  ao->add_bloodloss(1, hloc[HITLOC_SLOT]);
	  AO("That hurts!\n", 0, 0, 0);
	  return 0;
      case 16..17:
	  ATB(({ CB_DB_BONUS }), ({ ([ R2 : -15 ]) }));
	  ao->add_bloodloss(2, hloc[HITLOC_SLOT]);
	  return 4;
      case 18..19:
	  ATB( ({ CB_DB_BONUS, CB_STUNNED }),
	       ({ ([ R3 : -15 ]), ([ R3 : 1 ]) }));
	  ao->add_bloodloss(3, hloc[HITLOC_SLOT]);
	  return 6;
      default:
	  ATB( ({ CB_OB_BONUS }), ({ ([ R1 : 25 ]) }));
	  AO("Your opponent pierces through your body!\n", 0, 0, 0);
	  AT("You succeed to hit a lifeimportant part. That was lucky!\n",
	     0, 0, 0);
	  ao->do_die(att);
	  return 10;
    }
}

static int
crit_other(mixed hloc, object ao, object att, int step)
{
    int    reconn;
    object weap;
	
    switch (step)
    {
      case 0..1:
	  return 1;		/* extra damage */
      case 2..4:
	  AOB( ({ CB_STUNNED }), ({ ([ RH : 1 ]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 2;
	  AO("The hit knocks you back, delaying your reactions.\n", 0, 0, 0);
	  return 1;
      case 5..6:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R1 : -10 ]), ([ R1 : 1 ]) }));
	  return 4;
      case 7..8:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R1 : -50 ]), ([ R1 : 1 ]) }));
	  AO("You are stunned, cough.\n", 0, MSG_SEE, 0);
	  AT("Your opponent looks stunned.\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), " looks stunned.\n" }), ao, att);
	  return 2;
      case 9..10:
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 20 ]) }));
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R1 : -50, R3 : -20 ]), ([ R2 : 1 ]) }));
	  return 2;
      case 11..12:
	  ao->add_bloodloss(1, hloc[HITLOC_SLOT]);
	  return 2;
      case 13..14:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R3 : -5 ]), ([ RH : 1 ]) }));
	  ao->add_bloodloss(2, hloc[HITLOC_SLOT]);
	  return 2;
      case 15..16:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R2 : -20, time() + 500 : -20 ]), ([ R2 : 1 ]) }));
	  AOBS("Ouch, the blow cut off your " + hloc[HITLOC_DESC] + "!\n",
	       0, MSG_SEE, 0);
	  ao->set_wound(hloc[HITLOC_SLOT], SLOT_LOST);
	  if (reconn = ao->query_reconnect(hloc[HITLOC_SLOT]))
	      ao->add_bloodloss(5, reconn);
	  else 
	      ao->add_bloodloss(5, hloc[HITLOC_SLOT]);
	  ATBS("Your blow cut off your opponent's " +
	       hloc[HITLOC_DESC] + ".\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), "'s " + hloc[HITLOC_DESC] + " got cut off.\n" }),
	     ao, att);
	  return 2;
      case 17..18:
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 20 ]) }));
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R2 : -50, time() + 500 : -20 ]), ([ R2 : 1 ]) }));
	  AO("Ouch, the blow cut off your " + hloc[HITLOC_DESC] + "!\n",
	     0, 0, 0);
	  ao->set_wound(hloc[HITLOC_SLOT], SLOT_LOST);
	  if (reconn = ao->query_reconnect(hloc[HITLOC_SLOT]))
	      ao->add_bloodloss(5, reconn);
	  else 
	      ao->add_bloodloss(5, hloc[HITLOC_SLOT]);
	  ATBS("Your blow cut off your opponent's " + hloc[HITLOC_DESC] +
	       ".\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), "'s " + hloc[HITLOC_DESC] + " got cut off.\n" }),
	     ao, att);
	  return 4;
      default:
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 30 ]) }));
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R3 : -50, time() + 1500 : -20 ]), ([ R3 : 1 ]) }));
	  ao->set_wound(hloc[HITLOC_SLOT], SLOT_LOST);
	  if (reconn = ao->query_reconnect(hloc[HITLOC_SLOT]))
	      ao->add_bloodloss(5, reconn);
	  else 
	      ao->add_bloodloss(5, hloc[HITLOC_SLOT]);
	  AO("Ouch, that blow cut off your " + hloc[HITLOC_DESC] + "!\n",
	     0, 0, 0);
	  ATBS("You cut off your opponent's " + hloc[HITLOC_DESC] + ".\n",
	       ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), "'s " + hloc[HITLOC_DESC] + " got cut off.\n" }),
	     ao, att);
	  return 6;
    }
}
