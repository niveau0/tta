/* cold criticals */

#include "crit.h"

static int 
drop_hold(int slot, object ao)
{
    int    i, *co;
    object env, *hold;
    
    co = ao->query_coordinates();
    env = E(ao);
    for (i=sizeof(hold = ao->query_hold(SLT_ALL)); i--;)
    {
	hold[i]->reset_split();
	hold[i]->set_coordinates(co);
	hold[i]->move(env);
    }
    return 1;
}

static int
crit_head(mixed hloc, object ao, object att, int step)
{
    int *slots;

    switch (step)
    {
      case 0..2:
	AOB(({ CB_STUNNED }), ({ ([ RH : 1 ]) }));
	if (ao->query_state() & ON_GROUND)
	    return 3;
	AO("The hit makes you stumble, you still can parry "+
	   "but your attack is lost.\n", 0, 0, 0);
	return 2;
      case 3..5:
	AOB(({ CB_STUNNED }), ({ ([ R2 : 1 ]) }));
	if (ao->query_state() & ON_GROUND)
	    return 5;
	AO("A shower of cold makes you stumble, but you "+
	   "are still able to parry.\n", 0, MSG_SEE, 0);
	return 4;
      case 6..9:
	AOB(({ CB_DB_BONUS, CB_STUNNED, CB_ACTION_MOD }),
	    ({ ([ R2 : -20 ]), ([ R2 : 1 ]), ([ R2 : -10 ]) }));
	AO("A feeling of deadly cold stuns you.\n", 0, MSG_SEE, 0);
	AT("Your opponent shivers and looks completely lost ... " +
	   "Seems that " + ao->query_pronoun() +
	   " is stunned!\n", ao, MSG_SEE, 0);
	TW(({ QCTNAME(ao), " shivers and is then stunned.\n" }), ao, att);
	return 5;
      case 10..12:
	AOB(({ CB_DB_BONUS, CB_STUNNED, CB_ACTION_MOD }),
	    ({ ([ R4 : -45 ]), ([ R4 : 1 ]), ([ R12 : -15 ]) }));
	ao->add_bloodloss(4, hloc[HITLOC_SLOT]);
	AO("Whow! You suddenly have the feeling that the temperature "+
	   "around\nyou sinks about some dozen degrees. Then you "+
	   "realize\nthat your "+ hloc[HITLOC_DESC] +
	   " feels very cold and some parts of "+
	   "your skin turn\nto ice. A few seconds later they burst "+
	   "and begin to bleed.\n", 0, 0, 0);
	AT("Suddenly the temperature between you and your opponent "+
	   "starts\nto sink. Some ice crystals begin to grow in the " +
	   "hair of your\nopponent and some parts of his " +
	   "skin turn to ice\nand burst a few seconds later. " +
	   "\n", ao, MSG_SEE, 0);
	TW(({ "Some ice crystals begin to grow in the hair of ",
		  QTNAME(ao), ". Suddenly some parts of " +
		  ao->query_possessive()+ " skin seem to freeze "+
		  "and burst a second later.\n" }), ao, att);
	return 15;
      case 13..15:
	AOB(({ CB_STUNNED, CB_ACTION_MOD }),
	    ({ ([ R4 : 1 ]), ([ R12 : -50 ]) }));
	AO("You recognize that something is wrong with your brain. " +
	   "Something\nfreezes it and the world gets suddenly very "+
	   "dark.\n", 0, MSG_SEE, 0);
	AT("A shimmering sphere surrounds the head of your foe. " +
	   "Then "+ ao->query_pronoun() + " rolls " +
	   +ao->query_possessive()+ " eyes and "+
	   "falls down to the ground.\n", ao, MSG_SEE, 0);
	TW(({ "A shimmering sphere surrounds the "+ hloc[HITLOC_DESC] +
		  " of ", QTNAME(ao), " sending ", ao->query_objective(),
		  " to the ground.\n" }), ao, att);
	ao->add_unconscious(20);
	return 10;
      case 16..18:
	AOB(({ CB_ACTION_MOD }),
	    ({ ([ time() + 18000/TIME_SCALE : -50 ]) }));
	AO("You feel a deadly cold between your ears. Something " +
	   "fried your\nbrain and you hope you will ever return " +
	   "to the living when it gets dark.\n", 0, MSG_SEE, 0);
	AT("Something strange happens to your foe"+
	   ". "+capitalize(ao->query_pronoun())+" suddenly looks "+
	   "completely lost and\nthen "+ao->query_possessive()+
	   " complete "+ hloc[HITLOC_DESC] +
	   " is covered with a layer of small "+
	   "snowcrystals.\n", ao, MSG_SEE, 0); 
	TW(({ "Suddenly the "+ hloc[HITLOC_DESC] +
		  " of ", QTNAME(ao), " is engulfed "+
		  "by a small snow cloud.\n" }), ao, att);
	ao->add_unconscious(120);
	return 10;
      case 19:
	ATB(({ CB_OB_BONUS }), ({ ([ R3 : 10 ]) }));
	AO("A cloud of frost surrounds your "+ hloc[HITLOC_DESC] +
	   " and a second later you can't move your eyes - they are " +
	   "frozen. The deadly cold creeps inside your "+
	   hloc[HITLOC_DESC] + " and the " +
	   "last thing you feel is that everything above your neck " +
	   "turns to ice.\n", 0, MSG_SEE, 0);
	AT(({ "A cloud of cold surrounds the "+ hloc[HITLOC_DESC] +" of "
		  , QTNAME(ao) ,
		  ". When it clears ... funny, the "+ hloc[HITLOC_DESC] +
		  " of your foe seems " +
		  "to be a fine worked iceblock and you think it's not a " +
		  "good idea to be that cool.\n"}), ao, MSG_SEE, 0);
	TW(({ "A cloud of cold surrounds the "+ hloc[HITLOC_DESC] +
		  " of ", QTNAME(ao),
		  ". When it clears you recognize that the "+
		  hloc[HITLOC_DESC] +" is " +
		  "completely frozen to an iceblock and you think " +
		  "it's not too healthy to be that cool.\n" }), ao, att);
	ao->do_die(att);
	return 0;
      default:
	ATB(({ CB_OB_BONUS }), ({ ([ R1 : 20 ]) }));
	AO("A bolt of cold hits you at the forehead. Then you " +
	   "recognize\nthat the coldcreeps into your body and you "+
	   "feel that every\npart becomes colder and colder. After a " +
	   "few moments you can't\nmove your frozen eyes, then the "+
	   "mouth, then the neck, then ....\nNow you know that your " +
	   "last second has come when everything goes dark.\n", 0, 0, 0);
	AT(({ "Suddenly ", QTNAME(ao) ," seems to stiften and you "+
		  "recognize something strange happen. Only in a few "+
		  "moments "+ ao->query_possessive() +" body turns to ice "+
		  "and when he falls to the ground it splits into thousands "+
		  "of pieces and nobody will be able to stick this puzzle "+
		  "together again.\n"}) , ao, MSG_SEE, 0); 
	TW(({ "Suddenly the body of ", QTNAME(ao) ," seems to stiften " +
		  "and only a few moments later he is completely " +
		  "converted into ice. When he falls to the ground " +
		  "he splits into thousends of pieces and nobody " +
		  "will be able to stick them together again." +
		  "\n" }), ao, att);
	ao->do_die(att);
	remove_object(ao);
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
	AOB( ({ CB_STUNNED }), ({ ([ R1 : 1 ]) }));
	if (ao->query_state() & ON_GROUND)
	    return 4;
	AO("The hit knocks you back, delaying your reaction.\n", 0, 0, 0);
	return 3;
      case 6..8:
	AOB(({ CB_STUNNED }), ({ ([ R1 : 1 ]) }));
	if (ao->query_state() & ON_GROUND)
	    return 3;
	AO("A deadly cold delays your reactions.\n", 0, 0, 0);
	return 2;
      case 9..10:
	AOB(({ CB_DB_BONUS, CB_STUNNED }),
	    ({ ([ R4 : -15 ]), ([ R2 : 1 ]) }));
	AO("A feeling of deadly cold inside your entrails " +
	   "hits you back.\n", 0, MSG_SEE, 0);
	AT("A icy fist hits your foe "+
	   "at the " + hloc[HITLOC_DESC] + ".\n", ao, MSG_SEE, 0);
	TW(({ "A icy fist hits ", QTNAME(ao),
		  " at the " + hloc[HITLOC_DESC] + ".\n" }), ao, att);
	return 7;
      case 11..12:
	AOB(({ CB_ACTION_MOD, CB_DB_BONUS, CB_STUNNED }),
	    ({ ([ R4 : -25 ]), ([ R2 : -30 ]), ([ R4 : 1 ]) }));
	AO("A cloud of ice surrouns your " + hloc[HITLOC_DESC] +
	   ". Some very sharp ice crystals cut you in the skin" +
	   ".\n", 0, MSG_SEE, 0);
	AT("A cloud of ice surrounds your foe. When it clears " +
	   ao->query_pronoun()+ " bleeds on the " +
	   hloc[HITLOC_DESC] + ".\n", ao, MSG_SEE, 0);
	TW(({"A cloud of ice surrounds "
		 ,QTNAME(ao),". When it clears " + ao->query_pronoun() +
		 " bleeds on the " + hloc[HITLOC_DESC] +
		 ".\n" }), ao, att);
	ao->add_bloodloss(1, hloc[HITLOC_SLOT]);
	return 15;
      case 13..14:
	AOB(({ CB_ACTION_MOD, CB_STUNNED }),
	    ({ ([ R2 : -50 ]), ([ R6 : 1 ]) }));
	AOBS("Some ice crystals formed like daggers appear " +
	     "from nothing and hit you in the " +
	     hloc[HITLOC_DESC] + ".\n", 0, MSG_SEE, 0);
	ATBS("Suddenly some daggerlike formed icecrystals appear " +
	     "from nothing and hit your enemy in the " +
	     hloc[HITLOC_DESC] + ".\n", ao, MSG_SEE, 0);
	TW(({"Suddenly some daggerlike formed icecrystals appear " +
		 "from nothing and hit ", QTNAME(ao),
		 " in the " + hloc[HITLOC_DESC] +
		 ".\n" }), ao, att);
	ao->add_bloodloss(3, hloc[HITLOC_SLOT]);
	return 5;
      case 15..16:
	AOB(({ CB_DB_BONUS, CB_STUNNED, CB_ACTION_MOD }),
	    ({ ([ R3 : -25 ]), ([ R3 : 1 ]), ([ R6 : -25 ]) }));
	ao->add_bloodloss(3, hloc[HITLOC_SLOT]);
	AOBS("A deadly cold covers the skin of your " +
	     hloc[HITLOC_DESC] + ". Suddenly these parts break " +
	     "open and begin to bleed.\n", 0, MSG_SEE, 0);
	ATBS("You envoked a deadly cold that hits the " +
	     hloc[HITLOC_DESC] + " of your opponent, " +
	     "freezing the skin. With the next move of your foe " +
	     "these parts break open and begin to bleed." +
	     "\n", ao, MSG_SEE, 0);
	TW(({ QCTNAME(ao), "'s " + hloc[HITLOC_DESC] + "'s skin is " +
		  "suddenly frozen. With the next move these parts " +
		  "break open and begin to bleed.\n" }),
	   ao, att);
	return 15;
      case 17..18:
	AOB(({ CB_ACTION_MOD }),
	    ({ ([ (time() + 21600/TIME_SCALE) : -20 ]) }));
	AO("You have the feeling that some very important organs " +
	   "turned into\nice in this moment - and you are right! " +
	   "The immense pain makes you\nwant to leave this world " +
	   "and so shall it be.\n", 0, MSG_SEE, 0);
	AT("Your target is knocked back. "+capitalize(ao->query_pronoun())+
	   " coughs some times and with every cough " +
	   ao->query_pronoun()+ " breathes some snowflakes out" +
	   ".\n", ao, MSG_SEE, 0);
	TW(({QCTNAME(ao), " is knocked back, coughs and "+
		 "drops down unconscious.\n" }), ao, att);
	ao->add_unconscious(240);
	ao->add_bloodloss(1, hloc[HITLOC_SLOT]);
	return 15;
      case 19:
	AOBS("An incredible cold makes your " + hloc[HITLOC_DESC] +
	     " burst and destroys some vital organs. But then " +
	     "it seems the pain gets weaker and the cold warmer " +
	     "and nearly all your life, although it could have " +
	     "last a little longer in your opinion, passes " +
	     "your inner eye. And then there is nothing...\n", 0,
	     MSG_SEE, 0);
	ATBS("With the " + hloc[HITLOC_DESC] + " totally converted " +
	     "into ice your foe's lifeless body drops " +
	     "to the ground.\n", ao, MSG_SEE, 0);
	TW(({ "With the " + hloc[HITLOC_DESC] + " totally converted " +
		  "into ice, ", QTNAME(ao), "'s lifeless body drops " +
		  "to the ground.\n" }),
	   ao, att);
	ao->do_die(att);
	return 0;
      default:
	ATB(({ CB_OB_BONUS }), ({ ([ R1 : 25 ]) }));
	AO("This seems to be the end. With a mighty clash your " +
	   "complete body\nturns into ice. Funny, you can't even " +
	   "move your eyes and a second later\nit gets very dark." +
	   "\n", 0, MSG_SEE, 0); 
	AT("This seems to be an unnatural event. Within a second " +
	   "the body of your foe\nseems to absorb any " +
	   "heat from " + ao->query_possessive() +
	   " surrounding. But nonetheless " + ao->query_possessive() +
	   " body turns\nto ice and falls lifeless to the " +
	   "ground.\n",ao, MSG_SEE, 0);
	TW(({"This seems to be an unnatural event. Within a second " +
		 "the body of ", QTNAME(ao) ," seems to absorb any " +
		 "heat from " + ao->query_possessive() +
		 " surrounding. But nonetheless " + ao->query_possessive() +
		 " body turns to ice and falls lifeless to the " +
		 "ground.\n" }), ao, att);
	ao->do_die(att);
	/* im umkreis von x metern auch schaden ? */
	return 0;
    }
}

static int
crit_other(mixed hloc, object ao, object att, int step)
{
    int    sz, *co, *slots;
    object weap;
    
    switch (step)
    {
      case 0..1:
	return 2;		/* extra damage */ 
      case 2..3:
	AOB(({ CB_STUNNED }), ({ ([ RH : 1 ]) }));
	if (ao->query_state() & ON_GROUND)
	    return 3;
	AO("The hit knocks you back, delaying your reactions.\n", 0, 0, 0);
	return 2;
      case 4..5:
	AOB(({ CB_STUNNED }), ({ ([ R1 : 1 ]) }));
	AO("You are hit by a minor cold shock.\n", 0, 0, 0);
	return 3;
      case 6..7:
	AOB( ({ CB_DB_BONUS, CB_STUNNED }),
	     ({ ([ R2 : -10 ]), ([ R1 : 1 ]) }));
	if (ao->query_state() & ON_GROUND)
	    return 7;
	AO("A blast of cold makes you stumbling.\n", 0, 0, 0);
	AT("Your blast makes your enemy stumbling.\n", ao, MSG_SEE, 0);
	TW(({ "A blast of cold makes ", QTNAME(ao), " stumbling.\n" }),
	   ao, att);
	return 6;
      case 8..9:
	if (drop_hold(hloc[HITLOC_SLOT], ao))
	{
	    AOB(({ CB_DB_BONUS, CB_STUNNED }),
		({ ([ R2 : -25 ]), ([ R2 : 1 ]) }));
	    AO("You are hit by a deadly cold, "+
	       "which scorches your skin and causes\nyour " +
	       "blood flow. Cause of the pain you drop " +
	       "everything you are holding\nin your hands.\n",
	       0, MSG_SEE, 0);
	    AT("Your opponent is scorched by an explosion of "+
	       "cold, starts to bleed\nand drops all "+
	       "holding in the hands.\n", ao, MSG_SEE, 0);
	    TW(({ QCTNAME(ao), " is scorched by an explosion of "+
		      "ice and snow, starts to bleed and drops " +
		      "all holding in the hands.\n" }), ao, att);
	    ao->add_bloodloss(2, hloc[HITLOC_SLOT]);
	    return 10;
	}
	AOB(({ CB_STUNNED }), ({ ([ R6 : 1 ]) }));
	AO("A blast of cold hits you, delaying your "+
	   "reactions, makes your muscles\ncontract " +
	   "uncontrolled and bursts your skin.\n", 0, 0, 0);
	AT("Your opponent's skin bursts and the muscles " +
	   "contract uncontrolled.\n", ao, MSG_SEE, 0);
	TW(({"The skin of ", QTNAME(ao), " bursts and the muscles " +
		 "contract uncontrolled.\n" }), ao, att);
	ao->add_bloodloss(1, hloc[HITLOC_SLOT]);
	return 10;
      case 10..11:
	AOB(({ CB_DB_BONUS, CB_STUNNED }),
	    ({ ([ R6 : -50 ]), ([ R12 : 1 ]) }));
	AO("An explosion of ice and snow suddenly stuns "+
	   "you and freezes your eyes.\n", 0, 0, 0);
	AT("Your opponent is stunned by an explosion of "+
	   "ice and snow.\n", ao, MSG_SEE, 0);
	TW(({QCTNAME(ao), " is stunned by an explosion of ice "+
		 "and snow.\n" }), ao, att);
	return 20;
      case 12..13:
	AOB(({ CB_DB_BONUS, CB_STUNNED }),
	    ({ ([ R6 : -20 ]), ([ R9 : 1 ]) }));
	AO("An unknown, but deadly cold hits you at your " +
	   hloc[HITLOC_DESC] + ", making you\nshiver and think " +
	   " about death.\n", 0, 0, 0);
	AT("Your opponent is hit by your cold force.\n",
	   ao, MSG_SEE, 0);
	TW(({ QCTNAME(ao), " is hit by a cold force.\n" }), ao, att);
	return 15;
      case 14..15:
	if (weap = ao->query_wield(hloc[HITLOC_SLOT]))
	{
	    if (!weap->query_prop(OBJ_MAGIC) && !random(2))
	    {
		AOB(({ CB_ACTION_MOD }), ({([ R6 : -25 ])}));
		AO("Suddenly your weapon is surrounded by " +
		   "an icy lair, shudders and explodes\n" +
		   "in thousand pieces.", 0, MSG_SEE, 0);
		AT("Your attack makes the weapon of your "+
		   "opponent explode.\n", ao, MSG_SEE, 0);
		TW(({ "Suddenly ", QTNAME(ao), "'s weapon explodes.\n" }),
		   ao, att);
		remove_object(weap);
		return 20;
	    }
	    AOB(({ CB_ACTION_MOD }), ({ ([ R6 : -15 ]) }));
	    AO("A sudden cold makes you loosing your weapon.\n",
	       0, MSG_SEE, 0);
	    AT("Your opponent lost the weapon.\n", ao, MSG_SEE, 0);
	    TW(({ QCTNAME(ao), " lost the weapon.\n" }), ao, att);
	    weap->set_coordinates(ao->query_coordinates());
	    weap->move(E(ao));
	}
	return 15;
      case 16:
	AOBS("A sudden hit of deadly cold hits your " +
	     hloc[HITLOC_DESC] + ". A sharp pain emitts from "+
	     "the bone and then you can't feel your " +
	     hloc[HITLOC_DESC] + " anymore.\n", 0, MSG_SEE, 0);
	ATBS("You hit the " + hloc[HITLOC_DESC] + " of your enemy " +
	     "with a bolt of concentrated cold.\n", ao, MSG_SEE, 0);
	TW(({ QCTNAME(att), " hits the "+ hloc[HITLOC_DESC] + " of ",
		  QTNAME(ao), " with a bolt of concentrated cold.\n" }),
	   ao, att);
	ao->set_wound(hloc[HITLOC_SLOT], SLOT_UNUSABLE);
	ao->add_unconscious(30);
	return 18;
      case 17:
	AOB(({ CB_ACTION_MOD, CB_STUNNED }),
	    ({ ([ R24 : -40 ]), ([ R4 : 1 ]) }));
	AOBS("A cold force hits your " + hloc[HITLOC_DESC] +
	     " like a titan's fist, splatters your skin "+
	     "and freezes your flesh from the bone.\n", 0, MSG_SEE, 0);
	ATBS("The cold force you envoke hits the " + hloc[HITLOC_DESC] +
	     " of your target, freezing the flesh and splattering " +
	     "the skin.\n", ao, MSG_SEE, 0);
	TW(({ "A cold force hits the " + hloc[HITLOC_DESC] +
		  " of ", QTNAME(ao), ", freezing the flesh and " +
		  "splattering the skin.\n" }), ao, att);
	ao->add_bloodloss(3, hloc[HITLOC_SLOT]);
	return 15;
      case 18:
	AOBS("You are surrounded by a sphere of "+
	     "cold. You feel the deadly cold creeping from your " +
	     hloc[HITLOC_DESC] + " to your your heart, which " +
	     "suddenly stops beating...\n", 0, MSG_SEE, 0);
	ATBS("Your foe is surrounded by a sphere of "+
	     "cold. " + capitalize(ao->query_possessive()) + " " +
	     hloc[HITLOC_DESC] + " seems to be converted " +
	     "into ice.\n", ao, MSG_SEE, 0);
	TW(({ QCTNAME(ao), " is surrounded by a sphere of "+
		  "cold. "+ capitalize(ao->query_possessive()) + " " +
		  hloc[HITLOC_DESC] +" seems to be converted " +
		  "into ice.\n" }), ao, att);
	ao->do_die(att);
	return 15;
      case 19:
	ATB(({ CB_OB_BONUS }), ({ ([ R1 : 15 ]) }));
	AOBS("You feel an incredible cold makes your " +
	     hloc[HITLOC_DESC] + " burst and crawls further in "+
	     "your body. You feel your organs turning to ice " +
	     "and then there is nothing...\n", 0, MSG_SEE, 0);
	ATBS("With the " + hloc[HITLOC_DESC] + " totally "+
	     "converted into ice your foe's lifeless body drops to " +
	     "the ground.\n", ao, MSG_SEE, 0);
	TW(({" With the "+ hloc[HITLOC_DESC] +" totally converted " +
		 "into ice ", QTNAME(ao),
		 "'s lifeless body drops to the ground.\n" }),
	   ao, att);
	ao->do_die(att);
	/* all around (10 feet) need to be stunned */
	return 0;
      default:
	ATB(({ CB_OB_BONUS }), ({ ([ R1 : 25 ]) }));
	AO("Beginning from your " + hloc[HITLOC_DESC] +
	   " you feel your whole body converting into\nice. " +
	   "After a few moments your vision becomes unclear and " +
	   "then\nthere is nothing then darkness ...\n", 0, 0, 0);
	AT("The " + hloc[HITLOC_DESC] + " of your foe is hit by " +
	   "your cold blast. Only in a few seconds " +
	   ao->query_possessive() + "\nwhole body converts into ice. " +
	   "When he looses balance and falls\nlifeless to the ground " +
	   "he splits into thousands pieces ...\n", ao, MSG_SEE, 0);
	TW(({ QCTNAME(ao), "'s " + hloc[HITLOC_DESC] +
		  " is suddenly converted into ice. Only a few " +
		  "moments later the whole body glitters like an " +
		  "ice statue, but when he falls lifeless to the " +
		  "ground, he splits into thousends pieces.\n" }), ao, att);
	ao->do_die(att);
	remove_object(ao);
	/* Body turns to dust and cannot be resurrected */
	return 0;	
    }
}
