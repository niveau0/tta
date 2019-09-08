/*
 * impact criticals
 */

#include "crit.h"

static int
knocked_back(object ao, int inches)
{
    return 0;
}


static void
deadly_damage_init(object ao)
{
    if(ao->query_hp()>1)
	ao->set_hp(1);
}

    
/* The Living ao is unconscious and would die after <counter> seconds
   if it wouldn't get enough hps back to get it over 1 hp.
   a message would be printed the first round, after the living awakes
   again
 */

static void
deadly_damage(object ao, object att, int counter)
{
    if(ao->query_hp() >= 1)
    {
	ao->add_prop("_is_dying_",0);
	return;
    }
    if(counter)
    {
	if(ao->query_state() != UNCONSCIOUS && !(ao->query_prop("_is_dying_")))
	{
	    AO("You feel like you would die very soon, someone "+
	       "should take care \nof your wounds very soon.\n",0,0,0);
	    TW(({ QCTNAME(ao), " has a very pale face, ", 
		      ao->query_pronoun()," looks like ",
		      ao->query_pronoun(),"'s in great danger.\n"}),ao,0);
	    ao->add_prop("_is_dying_",1);
	}
	counter - (R1 - time());
    }    
    else
	if(ao->query_state() == UNCONSCIOUS)
	    ao->do_die(att);
    call_out("deadly_damage",counter, ao, att, counter);
}

static int
crit_head(mixed hloc, object ao, object att, int step)
{
    int     i, sz, stepwidth, add_damage, slottype, hslot;
    object *armor;
    
    add_damage = 0;
    stepwidth = ao->query_step();
    armor = ao->query_protection(hloc[HITLOC_SLOT]);    
    
    switch (step)
    {
      case 0..3:
	  AOB(({ CB_ACTION_MOD , CB_STUNNED}),
	      ({ ([R2 : -20]), ([R6 : 1]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 6;
	  AO("The hits makes you stumble back one step, "+
	     "though you can still parry, \nyou're unable "+
	     "to attack at the moment.\n",0 ,0 ,0);
	  AT("Your opponent stumbles backwards one step.\n",
	     ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), " stumbles backwards one step.\n" }),
	     ao, att);
	  add_damage = knocked_back(ao, stepwidth);
	  return 5 + add_damage;
      case 4..7:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R2 : -30]), ([R3 : 1]) }));
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 13 ]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 8;
	  AO("Ouch ! The hit strikes your head and you hear bells ringing "+
	     "within your head.\n Your legs cannot hold your weight, and "+
	     "you drop on your knees.\n",0,0,0);
	  AT("Yeah, your opponent drops to " + ao->query_possessive() +
	     " knees after your hit.\n",
	     ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), " drops to ",ao->query_possessive(),
		    " knees after the hit of ", QTNAME(att)}), ao, att);
	  ao->add_state(ON_GROUND);
	  return 7;
      case 8..10:
	  if (sizeof(armor))
	  {
	      AOB(({ CB_DB_BONUS }), ({ ([R6 : 40 ]) }));
	      ATB(({ CB_OB_BONUS }), ({ ([ R2 : 15 ]) }));
	      AO("A powererfull strike hits your head, you stumble "+
		 "backwards.\nYou wounder what would have happened "+
		 "if you hadn't worn armor\nto protect your head.\n",
		 0,0,0);
	      AT("Your hit lets your opponent stumble backwards.\n",
		 ao, MSG_SEE, 0);
	      TW(({QCTNAME(ao), "stumbles backwards after the hit of ",
		       QTNAME(att)}),ao , att);
	      add_damage = knocked_back(ao, 3*stepwidth);
	  }
	  else
	  {
	      AO("Argh, your head got hit by the forceful hit. It "+
		 "becomes dark around \nyou when you fell to the "+
		 "ground.\n",0,0,0);
	      AT("Good one! You hit the head of your opponent "+
		 "with a mighty \nblow, make " + ao->query_possessive()+
		 " drop unconscious to the ground.\n",ao, MSG_SEE, 0);
	      TW(({"With a mighty blow ", QTNAME(att),
		 " hits the head of ",QTNAME(ao), ",\n",
		       "letting ", ao->query_possessive(),
		       " drop unconsicious ,to the ground.\n"}),
		 ao, att);
	      ao->add_unconscious(10800);
	  }
	  return 7 + add_damage;
      case 12..13:
	   /* nose destroyed */
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R24 : -50 ]), ([ R24 : 1 ]) }));
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 5 ]) }));
	  if (sizeof(ao->query_slot_type(SLT_NOSE)))
	  {
	      ao->set_wound(SLT_NOSE + (hloc[HITLOC_SLOT] % 100),
			    SLOT_SHATTERED);
	      AO("Arg, the hit smashed your nose.\n", 0, MSG_SEE, 0);
	      AT("You smashed your opponent's nose.\n", ao, MSG_SEE, 0);
	      TW( ({ QCTNAME(ao), "'s nose got smashed.\n" }), ao, att);
	  }
	  return 10;
      case 14..16:
	  switch (random(4))
	  {
	    case 0:
		hslot = SLT_ARM;
		break;
	    case 1:
		hslot = SLT_HAND;
		break;
	    case 2:
		hslot = SLT_LEG;
		break;
	    default:
		hslot = SLT_FOOT;
		break;
	  }
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R24 : -50 ]), ([ R24 : 1 ]) }));
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 10 ]) }));
	  hslot = hslot + random(sizeof(ao->query_slot_type(hslot)));
	  if (!IN_ARRAY(hslot, ao->query_wounds()))
	  {
	      ao->set_wound(hslot , SLOT_UNUSABLE);
	      AOBS("A painfully strike hits your neck. Seconds later "+
		   "you lose control over your " +
		   ao->query_slot_desc(hslot+1) +
		   ".\n", 0, 0, 0);
	  }
	  else
	  {
	      AO("A painfully strike hits your neck.\n",0,0,0);
	  }
	  AT("You can see the pain on the face of your opponent "+
	     "as your strike \nhits "+
	     ao->query_possessive() + " neck.\n",ao, MSG_SEE,0);
	  TW(({ QCTNAME(att), "hits the neck of ", QTNAME(ao),
		    " and ", ao->query_possesive(), 
		    "face is filled with pain.\n"}), ao, att);
	  if (!random(3))
	      ao->add_unconscious(40);
	  return 17;
      case 17..18:
	  AOB(({ CB_ACTION_MOD, CB_STUNNED }),
	      ({ ([ R15 : -55 ]), ([R15 : 1]) }));
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 10 ]) }));
	  AO("You have the fealing your head got smashed into pieces "+
	     "by this hit.\nThe world became dark around you.\n",0,0,0);
	  AT("You hit the head of your opponent with a crushing "+
	     "sound.\n", ao, MSG_SEE, 0);
	  TW(({QCTNAME(att), " hit's the head of ",
		   QTNAME(ao), "with a crushing sound, knocking ",
		   ao->query_pronoun(), "unconscious to the ground.\n"}),
	     ao, att);
	  ao->add_unconscious(R15-time());
	  for (i=sizeof(armor); i--;)
	      if (armor[i]->query_prop(OBJ_MATERIAL) >= M_METAL)
	      {
		  call_out("deadly_damage_init", (RH-time()), ao);
		  call_out("deadly_damage", (R1 - time()),ao, att,
			  (R15 - time()));
		  return 17;
	      }
	  return 17;
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
    object *armor;
    int i, sz, hslot;
    
    armor = ao->query_protection(hloc[HITLOC_SLOT]);
    
    switch (step)
    {
      case 1..2:
	  return 4;     /* extra damage */
      case 3..5:
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 5 ]) }));
	  AOB(({ CB_ACTION_MOD, CB_STUNNED }),
	      ({([ R2 : -10]), ([ RH : 1 ]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 6;
	  AO("The hit knocks you back, delaying your reactions.\n", 0, 0, 0);
	  return 5;
      case 6..7:
	  AOB(({ CB_STUNNED }),({ ([ R6 : 1 ]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 3;
	  AO("You got hit into the side. You feel very uncomfortable.\n",
	     0,0,0);
	  AT("You hit your opponent hard in the side.\n",ao, MSG_SEE, 0);
	  TW(({QCTNAME(att), " hits ",QTNAME(ao), " hard in the side.\n"}),
	     ao, att);
	  return 2;
      case 8..9:
	   AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R5 : -15 ]), ([ R4 : 1 ]) }));
	  ATB(({ CB_OB_BONUS }),
	      ({ ([ R1 : 25 ]) }));
	   if (ao->query_state() & ON_GROUND)
	      return 8;
	   AO("The hit lets you stumble backwards. You cannot keep your " +
	      "balance\n", 0, 0, 0);
	   AT("You opponent stumbles backwards after your hit and falls " +
	      "to the ground.\n",
	     ao, MSG_SEE, 0);
	  TW(({QCTNAME(ao), " stumbles back after the hit of ",
		   QTNAME(att), " and falls to the ground.\n"}),
	     ao, att);
	  ao->add_state(ON_GROUND);
	  return 7;
      case 10..13:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R5 : -25 ]), ([ R3 : 1 ]) }));
	  ATB(({ CB_OB_BONUS }),
	      ({ ([ R1 : 20 ]) })); 
	  switch (random(2))
	  {
	    case 0:
		hslot = SLT_FOOT;
	    default:
		hslot = SLT_LEG;
	  }
	  hslot = hslot + random(sizeof(ao->query_slot_type(hslot)));
	  if (!IN_ARRAY(hslot, ao->query_wounds()))
	  {
	      ao->set_wound(hslot , SLOT_BROKEN);
	      AOBS("The hit sends you to the ground and if it isn't enough, "+
		   "you fell unlucky on your " +
		   ao->query_slot_desc(hslot+1) + ". You feel the bone break" +
		   ", sending pain through your nerves.\n",0,0,0);
	      ATBS("Your hit sends your opponent to the ground. And "+
		   "additionally you see his " +ao->query_slot_desc(hslot+1) +
		   " break.\n",ao, MSG_SEE, 0);
	      TW(({ QCTNAME(att), "'s hit sends ", QTNAME(ao),
			"to the ground. As ", ao->query_pronoun(),
			" strikes the ground, you see his ",
			ao->query_slot_desc(hslot +1), " break.\n"}),ao, att);
	      if (!random(3))
		  ao->add_unconscious(20);
	  }
	  else
	  {    
	      AO("The hit sends you to the ground.\n",0,0,0);
	      AT("Your hit sends your opponent to the ground.\n",
		 ao,MSG_SEE,0);
	      TW(({ QCTNAME(att), "'s hit sends ", QTNAME(ao),
			"to the ground.\n"}),ao, att);
	  }
	  return 8;
      case 14..16:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R5 : -40 ]), ([ R5 : 1 ]) }));
	  hslot = SLT_LEG + random(sizeof(ao->query_slot_type(SLT_LEG)));
	  if (!IN_ARRAY(hslot, ao->query_wounds()))
	  {
	      ao->set_wound(hslot , SLOT_UNUSABLE);
	      AOBS("The strike hits your " + hloc[HITLOC_DESC] +
		   ". You're unable to keep your balance and fall" +
		   "to the ground. Your " +
		   ao->query_slot_desc(hslot+1) + " feels somehow"+
		   " paralysed.\n",0,0,0);
	  }
	  else
	      AOBS("The strike hits your " + hloc[HITLOC_DESC] +
		   ". You're unable to keep your balance and fall" +
		   "to the ground.\n",0,0,0);
	  AT("Unable to keep his balance your opponent stumbles " +
	     "backwards and falls to the ground.\n",ao,MSG_SEE,0);
	  TW(({QCTNAME(ao), "stumbles back and falls to the ground ",
		   "after the hit of ",QTNAME(att), ".\n"}),ao, att);
	  ao->add_state(ON_GROUND);
	  if (!random(3))
	      ao->add_unconscious(40);
	  return 5;
      case 17..18:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R12 : -50 ]), ([ R12 : 1 ]) }));
	  hslot = SLT_ARM + random(sizeof(ao->query_slot_type(SLT_ARM)));
	  if (!IN_ARRAY(hslot, ao->query_wounds()))
	  {
	      ao->set_wound(hslot , SLOT_BROKEN);
	      AOBS("A powerful blow hits your "+ hloc[HITLOC_DESC] +
		   " and throws you to the ground. You feel a sharp"+
		   "pain running through your "+
		   ao->query_slot_desc(hslot+1) + ".\n",0,0,0);
	  }
	  else
	      AOBS("A powerful blow hits your "+ hloc[HITLOC_DESC] +
		   " and throws you to the ground.\n",0,0,0);
	  ATBS("With a powerful blow you hit your opponent's "+
	       hloc[HITLOC_DESC] + ", throwing "+
	       ao->query_pronoun() +
	       " unconscious to the ground.\n",ao, MSG_SEE,0);
	  TW(({ "With a powerfull blow ",QTNAME(att)," hits ",
		    QTNAME(ao), "throwing ",ao->query_pronoun(),
		    " to the ground.\n"}), ao, att);
	  for (i=sizeof(armor); i--;)
	      if (armor[i]->query_prop(OBJ_MATERIAL) >= M_METAL)
		  ao->add_unconscious(R15-time());
	      else
		  ao->add_unconscious(10800);
	  return 18;
      default:
	  ATB(({ CB_OB_BONUS }), ({ ([ R1 : 25 ]) }));
	  AOBS("A enormous blow hits your "+ hloc[HITLOC_DESC] +
	     ", smashing it into pieces. You try to breath, but feel only"+
	     "pain when doing this. It becomes dark around you.\n",
	     0,0,0);
	  ATBS("As you hit the " + hloc[HITLOC_DESC] + ", smashing it "+
	       "with a crunshing sound into pieces.\n", ao, MSG_SEE, 0);
	  TW(({QCTNAME(ao), "'s ", hloc[HITLOC_DESC]," got smashed with ",
		   "a crunshing sound by ", QTNAME(att), "hit.\n"}),ao, att);
	  ao->do_die(att);
	  return 25;
    }
}

static int
crit_other(mixed hloc, object ao, object att, int step)
{
    object weap, *armor;
    int i, flag;
    flag = 0;
    
    switch (step)
    {
      case 0..1:
	  return 4;       /* extra damage */
      case 2..3:
	  return 6;       /* extra damage */
      case 4..5:
	  AOB(({ CB_STUNNED }), ({ ([RH : 1]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 6;
	  AO("You are knocked back by the hit, your reactions become "+
	     "delayed.\n", 0, 0, 0);
      case 6..7:
	  AOB(({ CB_STUNNED }), ({ ([RH : 1]) }));
	  if (ao->query_state() & ON_GROUND)
	      return 6;
	  AO("You are knocked back by the hit, your reactions become "+
	     "delayed.\n", 0, 0, 0);
      case 8..9:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R3 : -15 ]), ([ R2 : 1 ]) }));
	  ATB(({ CB_OB_BONUS }),
	      ({ ([ R1 : 25 ]) }));
	   if (ao->query_state() & ON_GROUND)
	       return 8;
	   AO("You hardly can keep your balance after the hit!\n",0,0,0);
	   AT("Your opponent stumbles.\n", ao, MSG_SEE, 0);
	   TW(({ QCTNAME(ao), " stumbles.\n" }), ao, att);
	   return 6;
      case 10..11:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R5 : -15 ]), ([ R4 : 1 ]) }));
	  ATB(({ CB_OB_BONUS }),
	      ({ ([ R1 : 25 ]) }));
	   if (ao->query_state() & ON_GROUND)
	      return 8;
	   AO("The hit lets you stumble. You cannot keep your " +
	      "balance\n", 0, 0, 0);
	   AT("You opponent stumbles after your hit and falls " +
	      "to the ground.\n",
	     ao, MSG_SEE, 0);
	  TW(({QCTNAME(ao), " stumbles after the hit of ",
		   QTNAME(att), " and falls to the ground.\n"}),
	     ao, att);
	  ao->add_state(ON_GROUND);
	  return 7;
      case 12..13:
	  AOB(({ CB_STUNNED }), ({ ([ R3 : 1 ]) }));
	  if ((weap = ao->query_wield(hloc[HITLOC_SLOT])) &&
	      !weap->move(E(ao)))
	  {
	      AO("You lost your "+ weap->query_name()+ "!\n", 0, 0, 0);
	      ATBS("You smashed the " + weap->query_name() +
		   " out of the hands of your opponent.\n", ao, MSG_SEE, 0);
	      TW(({ QCTNAME(ao), " lost the weapon.\n" }), ao, att);
	  }
	  return 8;
      case 14:
	  armor = ao->query_protection(hloc[HITLOC_SLOT]);
	  if (sizeof(armor))
	  {
	      AOB(({ CB_DB_BONUS, CB_STUNNED }),
		  ({ ([ R12 : -50 ]), ([ R12 : 1 ]) }));
	      AOBS("That mighty blow would have broken your " +
		   hloc[HITLOC_DESC] + ", but luckily your armor " +
		   "absorbs most of the damage.\n", 0, MSG_SEE, 0);
	      ATBS("With a powerful blow you hit the " +
		   hloc[HITLOC_DESC] + " of your opponent, but the " +
		   "armor absorbed most of the damage.\n", ao,
		   MSG_SEE, 0);
	      TW(({ QCTNAME(att), " hits ", QTNAME(ao),
			" with a mighty blow, but it seems the armor "+
			"absorbed most of the damage.\n" }), ao, att);
	      if (!random(5))
		  ao->add_unconscious(40);
	      return 13;
	  }
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R12 : -50 ]), ([ R12 : 1 ]) }));
	  AOBS("With a loud *crack* that hit broke your " +
	       hloc[HITLOC_DESC] +".\n", 0, MSG_SEE, 0);
	  ATBS("Your blow broke your opponent's " +
	       hloc[HITLOC_DESC] + ".\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(att), "'s hit broke the ",
		    hloc[HITLOC_DESC],
		    " of ", QTNAME(ao), ".\n" }), ao, att);
	  if (!random(5))
	      ao->add_unconscious(40);
	  return 20;
      case 15:
	  armor = ao->query_protection(hloc[HITLOC_SLOT]);
	  for (i=sizeof(armor); i--;)
	      if (armor[i]->query_prop(OBJ_MATERIAL) >= M_METAL)
		  flag = 1;  
	  if (flag)
	  {
	      AOB(({ CB_DB_BONUS, CB_STUNNED }),
		  ({ ([ R12 : -50 ]), ([ R12 : 1 ]) }));
	      AOBS("That mighty blow would have broken your " +
		   hloc[HITLOC_DESC] + ", but luckily your armor " +
		   "absorbs most of the damage.\n", 0, MSG_SEE, 0);
	      ATBS("With a powerful blow you hit the " +
		   hloc[HITLOC_DESC] + " of your opponent, but the " +
		   "armor absorbed most of the damage.\n", ao,
		   MSG_SEE, 0);
	      TW(({ QCTNAME(att), " hits ", QTNAME(ao),
			" with a mighty blow, but it seems the armor "+
			"absorbed most of the damage.\n" }), ao, att);
	      if (!random(5))
		  ao->add_unconscious(40);
	      return 15;
	  }
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R12 : -50 ]), ([ R12 : 1 ]) }));
	  AOBS("With a loud *crack* that hit broke your " +
	       hloc[HITLOC_DESC] +".\n", 0, MSG_SEE, 0);
	  ATBS("Your blow broke your opponent's " +
	       hloc[HITLOC_DESC] + ".\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(att), "'s hit broke the ",
		    hloc[HITLOC_DESC],
		    " of ", QTNAME(ao), ".\n" }), ao, att);
	  if (!random(5))
	      ao->add_unconscious(40);
	  return 20;
      case 16:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R2 : -45, time() + 500 : -40 ]),
		     ([ R2 : 1 ]) }));
	  ao->set_wound(hloc[HITLOC_SLOT], SLOT_BROKEN);
	  AOBS("You hear a loud crack from within your " + hloc[HITLOC_DESC] +
	       "!\n", 0, MSG_SEE, 0);
	  ATBS("With a loud crack the " + hloc[HITLOC_DESC] + " of your "+
	       " opponent breaks.\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), "'s " + hloc[HITLOC_DESC] + " breaks with ",
		    " a load crack.\n" }),
	     ao, att);
	  if (!random(3))
	      ao->add_unconscious(40);
	  return 10;
      case 17:
	  AOB(({ CB_DB_BONUS, CB_STUNNED }),
	      ({ ([ R2 : -45, time() + 500 : -40 ]),
		     ([ R2 : 1 ]) }));
	  ao->set_wound(hloc[HITLOC_SLOT], SLOT_SHATTERED);
	  AOBS("A sharp pain runs through your body " + 
	       " after the hit, leaving back your "+ hloc[HITLOC_DESC] +
	       " destroyed!\n", 0, MSG_SEE, 0);
	  ATBS("You smashed the " + hloc[HITLOC_DESC] + " of your "+
	       " opponent with a crunshing sound.\n", ao, MSG_SEE, 0);
	  TW(({ QCTNAME(ao), "'s " + hloc[HITLOC_DESC] + " becomes smashed ",
		    " in a crunshing sound.\n" }),
	     ao, att);
	  if (!random(3))
	      ao->add_unconscious(40);
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
	  ATB(({CB_OB_BONUS}), ({ ([R1: 25]) }));
	  AO("The world becomes dark around you, and then it's over.\n",0,0,0);
	  AT("You got the final hit against your enemy.\n", 0, 0, 0);
	  ao->do_die(att);
	  return 0;
    }
}
