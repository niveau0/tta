/*
 * crush criticals
 */

#include "crit.h"

static int
crit_head(mixed hloc, object ao, object att, int step)
{
    int     i, sz, flag, *slots;
    object  *armor;
    
    switch (step)
    {
      case 0..3:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R1 : -15 ]), ([ R1 : 1 ]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 7;
	  AO("The hit makes you stumble, you still can parry "+
	     "but your attack is lost.\n", 0, 0, 0);
	  AT("Your hit makes your opponent stumble.\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(att), "'s hit makes ", QTNAME(ao),
		    " stumbling a bit.\n" }), ao, att);
	  return 6;
      case 4..7:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R1 : -50 ]), ([ R1 : 1 ]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 8;
	  AO("The hit knocks you back, the pang seems to delay "+
	     "your reactions.\n", 0, 0, 0);
	  AT("Your opponent is knocked back, but mangaged to " +
	     "keep the balance.\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), " is knocked back, but keeps the "+
		    "balance.\n" }), ao, att);
	  return 7;
      case 8..11:
	  /* nose destroyed */
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R24 : -50 ]), ([ R24 : 1 ]) }));
	  if (sizeof(ao->query_slot_type(SLT_NOSE)))
	  {
	      ao->set_wound(SLT_NOSE + (hloc[HITLOC_SLOT] % 100),
			    SLOT_SHATTERED);
	      AO("Arg, the hit smashed your nose.\n", 0, MSG_SEE, 0);
	      AT("You smashed your opponent's nose.\n", ao, MSG_SEE, 0);
	      TW( ({ QCTNAME(ao), "'s nose got smashed.\n" }), ao, att);
	  }
	  return 20;
      case 12..14:
	  /* eyes destroyed */
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R24 : -50 ]), ([ R24 : 1 ]) }));
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
		  TW( ({ QCTNAME(ao), "'s eyes got damaged.\n" }), ao, att);
	      }
	  }
	  if (!random(3))
	      ao->add_unconscious(40);
	  return 20;
      case 15..17:
	  armor = ao->query_protection(hloc[HITLOC_SLOT]);
	  for (i=sizeof(armor); i--;)
	      if (armor[i]->query_prop(OBJ_MATERIAL) >= M_METAL)
	      {
		  AOB(({ CB_DB_BONUS, CB_STUNNED }),
		      ({ ([ R12 : -50 ]), ([ R12 : 1 ]) }));
		  AOBS("That mighty blow would have broken your " +
		       hloc[HITLOC_DESC] + ", but luckily your armor " +
		       "absorbs most of the damage.\n", 0, MSG_SEE, 0);
		  ATBS("With a powerful blow you hit the " +
		       hloc[HITLOC_DESC] + " of your opponent, but the " +
		       "armor absorbed most of the damage.\n", ao, MSG_SEE, 0);
		  TW(({ QCTNAME(att), " hits ", QTNAME(ao),
			    " with a mighty blow, but it seems the armor "+
			    "absorbed most of the damage.\n" }), ao, att);
		  if (!random(5))
		      ao->add_unconscious(40);
		  return 25;
	      }
	  AOBS("With a loud *crack* that hit broke your " + hloc[HITLOC_DESC] +
	       ".\n", 0, MSG_SEE, 0);
	  ATBS("Your blow broke your opponent's " + hloc[HITLOC_DESC] + ".\n",
	       ao, MSG_SEE, 0);
	  TW(({ QCTNAME(att), "'s hit broke the ", + hloc[HITLOC_DESC] +
		    " of ", QTNAME(ao), ".\n" }), ao, att);
	  ao->do_die(att);
	  return 0;
      default:
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 25 ]) }));
	  AOBS("A tremendous blow smashes your " + hloc[HITLOC_DESC] +
	       " and after a short moment of incredible pain, "+
	       "the mist enclosing your mind fades and you get " +
	       "an unfamiliar feeling of weightlessness...\n", 0, MSG_SEE, 0);
	  AT(({ "With a tremendous blow you smashed ", QTNAME(ao),
		    "'s "+ hloc[HITLOC_DESC] + ".\n" }), ao, MSG_SEE, 0);
	  TW(({ "The tremendous blow of ", QTNAME(att), " smashed the " +
		    hloc[HITLOC_DESC] + " of ", QTNAME(ao),
		    " into pieces.\n" }), ao, att);
	  ao->do_die(att);
	  return 0;
    }
}

static int
crit_torso(mixed hloc, object ao, object att, int step)
{
    switch (step)
    {
      case 0..2:
	  return 3;		/* extra damage */
      case 3..5:
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 5 ]) }));
	  AOB(({ CB_STUNNED }), ({ ([ RH : 1 ]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 4;
	  AO("The hit knocks you back, delaying your reactions.\n", 0, 0, 0);
	  return 3;
      case 6..8:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R1 : -10 ]), ([ R1 : 1 ]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 6;
	  AO("The hit makes you stumble, you still can parry "+
	     "but your attack is lost.\n", 0, 0, 0);
	  AT("Your opponent stumbles.\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), " stumbles.\n" }), ao, att);
	  return 5;
      case 9..10:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R2 : -10 ]), ([ R2 : 1 ]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 7;
	  AO("The hit knocks you back, delaying your reactions.\n", 0, 0, 0);
	  AT("Your opponent stumbles.\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), " stumbles.\n" }), ao, att);
	  return 6;
      case 11..12:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R1 : -50 ]), ([ R1 : 1 ]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 7;
	  AO("The hit knocks you back, delaying your reactions.\n", 0, 0, 0);
	  AT("Your opponent stumbles.\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), " stumbles.\n" }), ao, att);
	  return 5;
      case 13..14:
	  ATB( ({ CB_OB_BONUS }), ({ ([ R1 : 20 ]) }));
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R1 : -50, R3 : -20 ]), ([ R1 : 1 ]) }));
	  AO("Ouch, that hurts!\n", 0, 0, 0);
	  return 10;
      case 15..16:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R2 : -50, R4 : -30 ]), ([ R2 : 1 ]) }));
	  AO("Arg, the pain makes you slower and slower.\n", 0, 0, 0);
	  AT("A fine blow to the " + hloc[HITLOC_DESC] + ".\n",
	     ao, MSG_SEE, 0);
	  return 20;
      case 17..18:
	  ATB( ({ CB_OB_BONUS }), ({ ([ R1 : 50 ]) }));
	  AO("Ouch, that hurts!\n", 0, 0, 0);
	  return 30;
      case 19:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ time() + 1800 : -50 ]), ([ time() + 1800 : 1 ]) }));
	  AT("A fine blow to the " + hloc[HITLOC_DESC] + ".\n",
	     ao, MSG_SEE, 0);
	  if (!random(3))
	      ao->add_unconscious(40);
	  return 25;
      default:
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 25 ]) }));
	  AOBS("You feel how your " + hloc[HITLOC_DESC] +
	       " got smashed and then the final curtain falls.\n", 0, 0, 0);
	  ATBS("Your blow breaks your opponent's " + hloc[HITLOC_DESC] +
	       ".\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), "'s " + hloc[HITLOC_DESC] + " breaks.\n" }),
	     ao, att);
	  ao->do_die(att);
	  return 25;
    }
}

static int
crit_other(mixed hloc, object ao, object att, int step)
{
    object weap;
	
    switch (step)
    {
      case 0..1:
	  return 3;		/* extra damage */
      case 2..3:
	  AOB(({ CB_STUNNED }), ({ ([ RH : 1 ]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 4;
	  AO("The hit knocks you back, delaying your reactions.\n", 0, 0, 0);
	  return 3;
      case 4..5:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R1 : -10 ]), ([ R1 : 1 ]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 5;
	  return 4;
      case 6..7:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R2 : -10 ]), ([ R1 : 1 ]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 7;
	  AO("The hit knocks you back, delaying your reactions.\n", 0, 0, 0);
	  return 6;
      case 8..9:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R1 : -50 ]), ([ R1 : 1 ]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 7;
	  AO("The hit knocks you back, delaying your reactions.\n", 0, 0, 0);
	  return 5;
      case 10..11:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R1 : -50, R3 : -20 ]), ([ R1 : 1 ]) }));
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 20 ]) }));
	  return 10;
      case 12..13:
	  AOB(({ CB_STUNNED }), ({ ([ R3 : 1 ]) }));
	  if ((weap = ao->query_wield(hloc[HITLOC_SLOT])) &&
	      !weap->move(E(ao)))
	  {
	      AO("You lost your weapon!\n", 0, 0, 0);
	      AT("Your opponent lost the weapon.\n", ao, MSG_SEE, 0);
	      TW(({ QCTNAME(ao), " lost the weapon.\n" }), ao, att);
	  }
	  return 8;
      case 14:
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 50 ]) }));
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R2 : -40, time() + 500 : -10 ]), ([ R2 : 1 ]) }));
	  if ((weap = ao->query_wield(hloc[HITLOC_SLOT])) &&
	      !weap->move(E(ao)))
	  {
	      AO("Ouch! That hurts. You lost your weapon!\n", 0, 0, 0);
	      AT("Your opponent lost the weapon.\n", ao, MSG_SEE, 0);
	      TW(({ QCTNAME(ao), " lost the weapon.\n" }), ao, att);
	  }
	  return 8;
      case 15:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R2 : -35, time() + 500 : -15 ]), ([ R2 : 1 ]) }));
	  ao->set_wound(hloc[HITLOC_SLOT], SLOT_BROKEN);
	  AOBS("*Crack* That blow broke your " + hloc[HITLOC_DESC] +
	       ".\n", 0, MSG_SEE, 0);
	  ATBS("Your blow broke your opponent's " + hloc[HITLOC_DESC] + ".\n",
	       ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), "'s " + hloc[HITLOC_DESC] + " breaks.\n" }),
	     ao, att);
	  return 10;
      case 16:
	  ATB( ({ CB_OB_BONUS }), ({ ([ R1 : 20 ]) }));
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R2 : -50, time() + 500 : -20 ]), ([ R2 : 1 ]) }));
	  ao->set_wound(hloc[HITLOC_SLOT], SLOT_BROKEN);
	  AOBS("*Crack* That blow broke your " + hloc[HITLOC_DESC] +
	       "!\n", 0, MSG_SEE, 0);
	  ATBS("Your blow breaks your opponent's " + hloc[HITLOC_DESC] +
	       ".\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), "'s " + hloc[HITLOC_DESC] + " breaks.\n" }),
	     ao, att);
	  return 10;
      case 17:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
			  ({ ([ R2 : -50, time() + 500 : -50 ]),
				 ([ R2 : 1 ]) }));
	  ao->set_wound(hloc[HITLOC_SLOT], SLOT_SHATTERED);
	  AOBS("*Crack* That blow destroyed your " + hloc[HITLOC_DESC] +
	       "!\n", 0, MSG_SEE, 0);
	  ATBS("Your blow smashed your opponent's " + hloc[HITLOC_DESC] +
	       ".\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), "'s " + hloc[HITLOC_DESC] + " breaks.\n" }),
	     ao, att);
	  return 12;
      case 18:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R2 : -50, R4 : -30 ]), ([ R3 : 1 ]) }));
	  AO("That hurts!\n", 0, 0, 0);
	  return 17;
      case 19:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ time() + 1800 : -50 ]), ([ R12 : 1 ]) }));
	  AO("That hurts!\n", 0, 0, 0);
	  return 15;
      default:
	  ATB( ({ CB_OB_BONUS }), ({ ([ R1 : 25 ]) }));
	  AT("Hey that was a good one.\n", 0, 0, 0);
	  ao->do_die(att);
	  return 0;
    }
}
