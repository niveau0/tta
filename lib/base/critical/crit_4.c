/* heat criticals */

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
	AO("A feeling of heat makes you stumble, you still can parry, but "+
	   "your attack is\nlost.\n", 0, 0, 0);
	return 2;
      case 3..5:
	AOB(({ CB_STUNNED }), ({ ([ R2 : 1 ]) }));
	if (ao->query_state() & ON_GROUND)
	    return 5;
	AO("An suddenly upcomming heat, feeling like a fever, irritates "+
	   "you, but you\nare still able to parry.\n", 0, MSG_SEE, 0);
	return 4;
      case 6..9:
	AOB(({ CB_DB_BONUS, CB_STUNNED, CB_ACTION_MOD }),
	    ({ ([ R2 : -20 ]), ([ R2 : 1 ]), ([ R2 : -10 ]) }));
	AO("An explosion of heat in front of your face makes you " +
	   "stumble.\n", 0, MSG_SEE, 0);
	AT("An explosion of heat in the face of your foe " +
	   "stuns " + ao->query_objective() +
	   ".\n", ao, MSG_SEE, 0);
	TW(({ "An explosion of heat stuns ",
		  QTNAME(ao), ".\n" }), ao, att);
	return 5;
      case 10..12:
	AOB(({ CB_DB_BONUS, CB_STUNNED, CB_ACTION_MOD }),
	    ({ ([ R4 : -45 ]), ([ R4 : 1 ]), ([ R12 : -15 ]) }));
	ao->add_bloodloss(4, hloc[HITLOC_SLOT]);
	AO("Whow! You suddenly have the feeling that the temperature "+
	   "around\nyou increases about some dozen degrees. Then you "+
	   "realize\nthat your "+ hloc[HITLOC_DESC] +
	   " feels very hot and some parts of your skin turn\n"+
	   "black and the smell of burned flesh is in the wind, " +
	   "while blood starts to flow cooling your wounds.\n", 0, 0, 0);
	AT("Suddenly the temperature between you and your opponent "+
	   "starts\nto increase. The air between you starts to flicker " +
	   "and some parts of your\nfoe's skin turn black and burst "+
	   "with blood flowing.\n", ao, MSG_SEE, 0);
	TW(({ "The air in front of ", QTNAME(ao),
		  " starts to flicker, and suddenly some parts of " +
		  ao->query_possessive() + " skin turn black "+
		  "and burst a second later.\n" }), ao, att);
	return 15;
      case 13..15:
	AOB(({ CB_STUNNED, CB_ACTION_MOD }),
	    ({ ([ R4 : 1 ]), ([ R12 : -50 ]) }));
	AO("A sudden heat seems to smolder your brain. After a " +
	   "few moments the\nworld gets dark.\n", 0, MSG_SEE, 0);
	AT("The air around your foe's head starts to flicker like the "+
	   "horizon of\na desert. Then "+ ao->query_pronoun() +" distorts "+ 
	   ao->query_possessive() +" eyes and drops down\nto the ground.\n", 
	   ao, MSG_SEE, 0);
	TW(({ "A flickering sphere surrounds the "+ hloc[HITLOC_DESC] +
		  " of ", QTNAME(ao), " sending "+ ao->query_objective() +
		  " to the ground.\n" }), ao, att);
	ao->add_unconscious(20);
	return 10;
      case 16..18:
	AOB(({ CB_ACTION_MOD }),
	    ({ ([ time() + 18000/TIME_SCALE : -50 ]) }));
	AO("You feel a deadly heat coming up between your ears. Something " +
	   "is burning\nyour brain and you hope you will ever return " +
	   "to the living when it gets dark.\n", 0, MSG_SEE, 0);
	AT("Something strange happens to your foe"+
	   ". "+capitalize(ao->query_pronoun())+" suddenly looks "+
	   "completely lost and\nthen "+ao->query_possessive()+
	   " complete "+ hloc[HITLOC_DESC] +
	   " is covered with a small bubbles of burned skin\nsegregating "+
	   "a smelling liquid.\n", ao, MSG_SEE, 0); 
	TW(({ "Suddenly the "+ hloc[HITLOC_DESC] +
		  " of ", QTNAME(ao), " is engulfed "+
		  "by a small ball of fire.\n" }), ao, att);
	ao->add_unconscious(120);
	return 10;
      case 19:
	ATB(({ CB_OB_BONUS }), ({ ([ R3 : 10 ]) }));
	AO("A ball of fire surrounds your "+ hloc[HITLOC_DESC] +
	   " and a second later you can't move your eyes - they are " +
	   "totally dried and burned. The deadly heat creeps inside your "+
	   hloc[HITLOC_DESC] + " and the " +
	   "last thing you feel is that everything above your neck " +
	   "turns to lava.\n", 0, MSG_SEE, 0);
	AT(({ "A ball of fire surrounds the "+ hloc[HITLOC_DESC] +" of "
		  , QTNAME(ao) ,
		  ". When it clears ... funny, the "+ hloc[HITLOC_DESC] +
		  " of your foe looks like as if it turned to ashes\nand "+
		  "you think it's not a good idea to be that "+
		  "hotblooded.\n"}), ao, MSG_SEE, 0);
	TW(({ "A ball of fire surrounds the "+ hloc[HITLOC_DESC] +
		  " of ", QTNAME(ao),
		  ". When it clears you recognize that the "+
		  hloc[HITLOC_DESC] +" is completely burned to ashes and "+
		  "you think it's not too healthy to be that "+
		  "hotblooded.\n" }), ao, att);
	ao->do_die(att);
	return 0;
      default:
	ATB(({ CB_OB_BONUS }), ({ ([ R1 : 20 ]) }));
	AO("A sea of flames hits you and embeds your whole body. Then you "+
	   "feel the\nheat penetrate your veins while your blood starts "+
	   "boiling and your hair\nsmolders. Your eyeballs dried and the "+
	   "last thing you see is your skin bursting\nin hot bubbles. And "+
	   "then you feel the pain! Getting stronger and stronger\ntill a "+
	   "gracious blacking reaches your tortured brain.\n", 0, 0, 0);
	AT(({ "Suddenly the body of your foe is hit by a sea of flames "+
		  "that embeds "+ ao->query_possessive() +" whole "+
		  "body. Inmid the flames you only can see "+ 
		  ao->query_possessive() +" silhouette turning darker and "+
		  "the wide opened eyes, showing great astonishment of "+
		  "what happens. When the flames go out you see that "+
		  ao->query_possessive() +" body has totally burned to "+
		  "ashes. And after the next gentle breeeze only a nasty "+
		  "smell remainds of what has happened.\n"}), ao, MSG_SEE,0);
	TW(({ "Suddenly the body of ", QTNAME(ao) ," is hit by a sea of "+
		  "flames that embeds "+ ao->query_possessive() +" whole "+
		  "body. Inmid the flames only a silhouette and wide open "+
		  "eyes, showing great astonishment of what happens, can "+
		  "be seen. When the flames went go out you see that "+
		  ao->query_possessive() +" body has totally burned to "+
		  "ashes. And after the next gentle breeze only a nasty "+
		  "smell remainds of what has happend.\n"}), ao, att);
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
	AO("A feeling of heat makes you stumble, you still can parry, but "+
	   "your attack is\nlost.\n", 0, 0, 0);
	return 3;
      case 6..8:
	AOB(({ CB_STUNNED }), ({ ([ R1 : 1 ]) }));
	if (ao->query_state() & ON_GROUND)
	    return 3;
	AO("An suddenly upcomming heat, feeling like a fever, irritates "+
	   "you, but you\nare still able to parry.\n", 0, MSG_SEE, 0);
	return 2;
      case 9..10:
	AOB(({ CB_DB_BONUS, CB_STUNNED }),
	    ({ ([ R4 : -15 ]), ([ R2 : 1 ]) }));
	AO("An explosion of heat inside your chest makes you stumble.\n", 
	   0, MSG_SEE, 0);
	AT("Your opponent suddenly seems to have difficulties in breathing "+
	   "and a spasm in the chest stuns "+ ao->query_objective() +".\n",
	   ao, MSG_SEE, 0);
	TW(({"A sudden spasm in the chest stuns ", QTNAME(ao), ".\n"}),
	   ao, att);
	return 7;
      case 11..12:
	AOB(({ CB_ACTION_MOD, CB_DB_BONUS, CB_STUNNED }),
	    ({ ([ R4 : -25 ]), ([ R2 : -30 ]), ([ R4 : 1 ]) }));
	AO("Lances of fire pierce into your " + hloc[HITLOC_DESC] +
	   "scorching the flesh. The immense heat makes you feel like "+
	   "being thrown into a vulcano. Your skin tightens and rips, but "+
	   "starts to flow and evaporates nearly in the same time. You "+
	   "wreathe in pain and your tortured muscles just wound do what "+
	   "you want.\n", 0, MSG_SEE, 0);
	AT("Lances of fire pierce into the "+ hloc[HITLOC_DESC] + "of your "+
	   "foe. With the face warped in pain "+ ao->query_pronoun() +
	   " wreathes around while the blood evaporates as fast as it is "+
	   "flowing out the ripping skin cause of the immense heat.\n", 
	   ao, MSG_SEE, 0);
	TW(({"Lances of fire pierce into the "+ hloc[HITLOC_DESC] + "of ",
		 QTNAME(ao), "With the face warped in pain "+ 
		 ao->query_pronoun() +" wreathes around while the blood "+
		 "evaporates as fast as it is flowing out the ripping "+
		 "skin cause of the immense heat.\n"}), ao, att); 
	ao->add_bloodloss(1, hloc[HITLOC_SLOT]);
	return 15;
      case 13..14:
	AOB(({ CB_ACTION_MOD, CB_STUNNED }),
	    ({ ([ R2 : -50 ]), ([ R6 : 1 ]) }));
	AOBS("A rain of flaming drops suddenly appear from nowhere and hit "+
	     "you at the "+ hloc[HITLOC_DESC] +", leaving spots of burned "+
	     "flesh on it.\n", 0, MSG_SEE, 0);
	ATBS("Suddenly a rain of flaming drops appear from nowhere and hit "+
	     "your opponent at the "+ hloc[HITLOC_DESC] +" leaving spots "+
	     "of burned flesh.\n", ao, MSG_SEE, 0);
	TW(({"Suddenly a rain of flaming drops appear from nowhere and hit ",
		 QTNAME(ao), " at the "+ hloc[HITLOC_DESC] +" leaving "+
		 "spots of burned flesh.\n" }), ao, att);
	ao->add_bloodloss(3, hloc[HITLOC_SLOT]);
	return 5;
      case 15..16:
	AOB(({ CB_DB_BONUS, CB_STUNNED, CB_ACTION_MOD }),
	    ({ ([ R3 : -25 ]), ([ R3 : 1 ]), ([ R6 : -25 ]) }));
	ao->add_bloodloss(3, hloc[HITLOC_SLOT]);
	AOBS("A deadly heat giving you the feeling of being shifted right "+
	     "into hell. The heat scorches your "+ hloc[HITLOC_DESC] +
	     " and completely dries your skin. With the next move the skin"+
	     "rips and blood starts to flow.\n", 0, MSG_SEE, 0);
	ATBS("The deadly heat you envoked hits your opponent at the "+
	     hloc[HITLOC_DESC] +" and scorches his skin. As "+
	     ao->query_pronoun() +" moves again the affected parts rip and "+
	     "start to bleed.\n", ao, MSG_SEE, 0);
	TW(({ QCTNAME(ao), "'s "+ hloc[HITLOC_DESC] +"is suddenly scorched "+
		  "by an immense heat that dried "+ ao->query_possessive() +
		  "skin that rips with "+ ao->query_possessive() +" next "+
		  "move causing blood dropping down.\n" }), ao, att);
	return 15;
      case 17..18:
	AOB(({ CB_ACTION_MOD }),
	    ({ ([ (time() + 21600/TIME_SCALE) : -20 ]) }));
	AO("You have the feeling that some very important organs " +
	   "are thrown into boiling water. The immense pain makes you\nwant "+
	   "to leave this world and the a gracious darkness comes upon you."+
	   "\n", 0, MSG_SEE, 0);
	AT("Your target is knocked back, lurks around and " +
	   "drops down unconscious.\n", ao, MSG_SEE, 0);
	TW(({QCTNAME(ao), " is knocked back, lurks around and "+
		 "drops down unconscious.\n" }), ao, att);
	ao->add_unconscious(240);
	ao->add_bloodloss(1, hloc[HITLOC_SLOT]);
	return 15;
      case 19:
	AOBS("An immense heat makes your " + hloc[HITLOC_DESC] +
	     " burst and destroys some vital organs. But then " +
	     "it seems the pain gets weaker and the heat colder " +
	     "and nearly all your life, although it could have " +
	     "last a little longer in your opinion, passes " +
	     "your inner eye. And then there is nothing...\n", 0,
	     MSG_SEE, 0);
	ATBS("With the " + hloc[HITLOC_DESC] + " totally burned your " +
	     "foe's lifeless body drops to the ground.\n", ao, MSG_SEE, 0);
	TW(({ "With the " + hloc[HITLOC_DESC] + " totally burned, ",
		  QTNAME(ao), "'s lifeless body drops to the ground.\n" }),
	   ao, att);
	ao->do_die(att);
	return 0;
      default:
	ATB(({ CB_OB_BONUS }), ({ ([ R1 : 25 ]) }));
	AOBS("An incredible blast of fire hits your "+ hloc[HITLOC_DESC] +
	     ". Flickering flames spread over your body at a great speed, "+
	     "turning your flesh to ashes. All your hair and your skin "+
	     "vanish in a hundredth of a second. When the pain growing "+
	     "nearly beyond the point of endurability your seething blood "+
	     "erupts with the might of a vulcano and you conk out before "+
	     "you can make a final stroke.\n", 0, MSG_SEE, 0);
	ATBS("An incredible blast of fire hits your opponent at the "+
	     hloc[HITLOC_DESC] +". Flickering flames spread over the hole "+
	     "body at a great speed. With the "+ hloc[HITLOC_DESC] + 
	     " totally burned your foe's lifeless body drops to the ground."+
	     "\n", ao, MSG_SEE, 0);
	TW(({"An incredible blast of fire hits the "+ hloc[HITLOC_DESC] +
		 " of ", QTNAME(ao), ". Flickering flames spread over "+
		 ao->query_possessive() +" hole body at a great speed. "+
		 "Finally, with the "+ hloc[HITLOC_DESC] +" totally burned, ",
		 QTNAME(ao), "'s lifeless body drops to the ground.\n" }),
	   ao, att);
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
	AO("You are hit by a minor heat shock.\n", 0, 0, 0);
	return 3;
      case 6..7:
	AOB( ({ CB_DB_BONUS, CB_STUNNED }),
	     ({ ([ R2 : -10 ]), ([ R1 : 1 ]) }));
	if (ao->query_state() & ON_GROUND)
	    return 7;
	AO("A blast of heat makes you stumbling.\n", 0, 0, 0);
	AT("Your blast makes your enemy stumbling.\n", ao, MSG_SEE, 0);
	TW(({ "A blast of heat makes ", QTNAME(ao), " stumbling.\n" }),
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
