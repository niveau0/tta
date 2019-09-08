/*
 * puncture criticals
 */

#include "crit.h"

static int
crit_head(mixed hloc, object ao, object att, int step)
{
    int     i, sz, flag, *slots;
    object  *armor;
    
    switch (step)
    {
      case 0..2:
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 10 ]) }));
	  return 2;
      case 3..5:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R2 : -15 ]), ([ R2 : 1 ]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 4;
	  AO("The hit makes you stumble, you still can parry "+
	     "but your attack is lost.\n", 0, 0, 0);
	  AT("Your opponent stumbles.\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), " stumbles.\n" }), ao, att);
	  return 3;
      case 6..8:
	  AOB(({ CB_STUNNED }),({ ([ R1 : 1 ]) }));
	  AO("The pain slowed your reactions for a moment.\n", 0, MSG_SEE, 0);
	  return 4;
      case 9..11:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R4 : -25 ]), ([ R2 : 1 ]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 5;
	  AO("The hit nearly knocks you down and the pain slows "+
	     "your reactions.\n", 0, MSG_SEE, 0);
	  return 4;
      case 12..14:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R12 : -25 ]), ([ R3 : 1 ]) }));
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 25 ]) }));
	  AO("Ouch, that hurts! A sharp pain passes through "+
	     "your body and you can feel\nyour blood flow.\n", 0, 0, 0);
	  ao->add_bloodloss(2, hloc [HITLOC_SLOT]); 
	  return 4;
      case 15..17:
	  /* one eye destroyed */
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R24 : -50 ]), ([ R4 : 1 ]) }));
	  ao->add_bloodloss(3, hloc [HITLOC_SLOT]);
	  if (sz = sizeof(slots = ao->query_connections(hloc[HITLOC_SLOT])))
	  {
	      for (i=0; i<sz; i++)
		  if (SLOT_TYPE(slots[i]) == SLT_EYE)
		  {
		      flag = 1;
		      ao->set_wound(slots[i], SLOT_SHATTERED);
		      break;
		  }
	      if (flag)
	      {
		  AO("Arg, the hit destroyed one of your eyes.\n", 0, 0, 0);
		  AT("Your blow destroyed an eye.\n", ao, MSG_SEE, 0);
		  TW(({ QCTNAME(ao), "'s eyes got damaged.\n" }), ao, att);
	      }
	  }
	  if (!random(3))
	  {
	      if (!(ao->query_state() & ON_GROUND))
		  AO("The hit knocks you down and it turns dark around you.\n",
		     0, 0, 0);
	      ao->add_unconscious(20);
	  }
	  return 5;
      case 18..19:
	  armor = ao->query_protection(hloc[HITLOC_SLOT]);
	  for (i=sizeof(armor); i--;)
	      if (armor[i]->query_prop(OBJ_MATERIAL) >= M_METAL)
	      {
		  AOB(({ CB_DB_BONUS, CB_STUNNED }),
		      ({ ([ R24 : -25 ]), ([ R12 : 1 ]) }) );
		  ao->add_bloodloss(1, hloc[HITLOC_SLOT]);
		  AOBS("Luckily you wear good armor! Otherwise "+
		       "this hit would have smashed your " +
		       hloc[HITLOC_DESC] + "!\n", 0, MSG_SEE, 0);
		  if (!random(5))
		      ao->add_unconscious(40);
		  return 5;
	      }
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 25 ]) }));
	  AOBS("Argh, your opponent's attack pierces deep through "+
	       "your " + hloc[HITLOC_DESC] + "!\n", 0, MSG_SEE, 0);
	  ATBS("You pierced deep into your opponent's " +
	       hloc[HITLOC_DESC] + ".\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), "'s " + hloc[HITLOC_DESC] +
		    " got pierced by the attack.\n" }), ao, att);
	  ao->do_die(att);
	  return 0;
      default:
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 25 ]) }));
	  AOBS("You feel something penetrate your "+
	       hloc[HITLOC_DESC] + "\nand then the final curtain falls!\n",
	       0, 0, 0);
	  ATBS("What a hit! You penetrate your opponent's " +
	       hloc[HITLOC_DESC] + ".\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), "'s " + hloc[HITLOC_DESC] +
		    " got pierced by the attack.\n" }), ao, att);
	  ao->do_die(att);
	  return 0;
    }
}

static int
crit_torso(mixed hloc, object ao, object att, int step)
{
    int    i, sz;
    object *armor;
	
    switch (step)
    {
      case 0..2:
	  return 1;		/* extra damage */
      case 3..5:
	  AOB(({ CB_STUNNED }), ({ ([ RH : 1 ]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 2;
	  AO("The hit knocks you back, delaying your reactions.\n", 0, 0, 0);
	  return 1;
      case 6..8:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R1 : -10 ]), ([ R1 : 1 ]) }));
	  return 2;
      case 9..11:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R1 : -25 ]), ([ R1 : 1 ]) }));
	  AO("You are stunned, cough.\n", 0, MSG_SEE, 0);
	  return 3;
      case 12..14:
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 20 ]) }));
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R1 : -25 ]), ([ R1 : 1 ]) }));
	  ao->add_bloodloss(1, hloc[HITLOC_SLOT]);
	  if (ao->query_state() & ON_GROUND)
	      return 4;
	  AO("The pain stuns you! You lost your attack, but "+
	     "you are still able to parry.\n", 0, 0, 0);
	  AT("Your opponent looks stunned.\n", ao, MSG_SEE, 0);
	  TW( ({ QCTNAME(ao), " looks stunned.\n" }), ao, att);
	  return 3;
      case 15..17:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
			  ({ ([ R1 : -20 ]), ([ R4 : 1 ]) }));
	  ao->add_bloodloss(1, hloc[HITLOC_SLOT]);
	  AO("Ouch, that hurts!\n", 0, 0, 0);
	  return 3;
      case 18..19:
	  armor = ao->query_protection(hloc[HITLOC_SLOT]);
	  for (i=sizeof(armor); i--; i++)
	      if (armor[i]->query_prop(OBJ_MATERIAL) >= M_METAL)
	      {
		  AOB(({ CB_DB_BONUS, CB_STUNNED }),
		      ({ ([ R4 : -30 ]), ([ R4 : 1 ]) }));
		  AOBS("Ouch! That would have smashed your " +
		       hloc[HITLOC_DESC] + "! Luckily you wear good armor.\n",
		       0, MSG_SEE, 0);
		  ao->add_bloodloss(2, hloc[HITLOC_SLOT]);
		  return 5;
	      }
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 15 ]) }));
	  AOBS("You see your own blood spatter as your opponent "+
	       "hits your " + hloc[HITLOC_DESC] + ".\n", 0, MSG_SEE, 0);
	  AT("Blood spatters as you hit where you imagine an artery.\n",
	     0, 0, 0);
	  ao->add_bloodloss(10, hloc[HITLOC_SLOT]);
	  return 10;
      default:
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 15 ]) }));
	  AO("Your opponent hits your body deadly.\n", 0, 0, 0);
	  AT("You directly hit where you imagined a lifeimportant part.\n",
	     0, 0, 0);
	  ao->do_die(att);
	  return 0;
    }
}

static int
crit_other(mixed hloc, object ao, object att, int step)
{
    object weap;
	
    switch (step)
    {
      case 0..1:
	  return 1;		/* extra damage */
      case 2..4:
	  AOB(({ CB_STUNNED }), ({ ([ RH : 1 ]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 2;
	  AO("The hit knocks you back, delaying your reactions.\n", 0, 0, 0);
	  return 1;
      case 5..6:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R1 : -10 ]), ([ R1 : 1 ]) }));
	  return 2;
      case 7..8:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R1 : -25 ]), ([ R1 : 1 ]) }));
	  AO("You are stunned, cough.\n", 0, MSG_SEE, 0);
	  AT("Your opponent looks stunned.\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), " looks stunned.\n" }), ao, att);
	  return 2;
      case 9..10:
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 20 ]) }));
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R1 : -25, R3 : -15 ]), ([ R2 : 1 ]) }));
	  AT("Your opponent looks stunned.\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), " looks stunned.\n" }), ao, att);
	  return 2;
      case 11..12:
	  ao->add_bloodloss(1, hloc[HITLOC_SLOT]);
	  return 2;
      case 13..14:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R3 : -5 ]), ([ R1 : 1 ]) }));
	  ao->add_bloodloss(2, hloc[HITLOC_SLOT]);
	  return 2;
      case 15..16:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R2 : -15, time() + 250 : -15 ]), ([ R2 : 1 ]) }));
	  ao->add_bloodloss(3, hloc[HITLOC_SLOT]);
	  ao->set_wound(hloc[HITLOC_SLOT], SLOT_BROKEN);
	  AOBS("Ouch, the hit breakes your " + hloc [HITLOC_DESC] +
	       "!\n", 0, MSG_SEE, 0);
	  ATBS("Your hit breaks your opponent's " + hloc[HITLOC_DESC] +
	       ".\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), "'s " + hloc[HITLOC_DESC] + " breaks.\n" }),
	     ao, att);
	  return 2;
      case 17..18:
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 20 ]) }));
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R2 : -30, time() + 300 : -25 ]), ([ R2 : -30 ]) }));
	  ao->add_bloodloss(4, hloc[HITLOC_SLOT]);
	  ao->set_wound(hloc[HITLOC_SLOT], SLOT_BROKEN);
	  AOBS("Ouch, the hit cracks your " + hloc[HITLOC_DESC] +
	       "!\n", 0, MSG_SEE, 0);
	  ATBS("Your hit breaks your opponent's " + hloc[HITLOC_DESC] +
	       ".\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), "'s " + hloc[HITLOC_DESC] + " breaks.\n" }),
	     ao, att);
	  return 5;
      default:
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 30 ]) }));
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R3 : -50, time() + 1000 : -20 ]), ([ R3 : 1 ]) }));
	  ao->add_bloodloss(5, hloc[HITLOC_SLOT]);
	  ao->set_wound(hloc[HITLOC_SLOT], SLOT_SHATTERED);
	  AOBS("Arrgh, the hit shatters your " + hloc[HITLOC_DESC] +
	       "!\n", 0, MSG_SEE, 0);
	  ATBS("You shatter your opponent's " + hloc[HITLOC_DESC] +
	       ".\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), "'s " + hloc[HITLOC_DESC] + " breaks.\n" }),
	     ao, att);
	  return 5;
    }
}
