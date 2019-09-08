/* electricity criticals */

#include "crit.h"

static int 
check_metal_armor(int slot, object ao)
{
    int    i;
    object *armor;
    
    for (i=sizeof(armor = ao->query_protection(slot)); i--;)
	if (armor[i]->query_prop(OBJ_MATERIAL) >= M_METAL)
	    return 1;
    return 0;
}

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
	AO("A flash of light makes you stumble, but you "+
	   "are still able to parry.\n", 0, MSG_SEE, 0);
	return 4;
      case 6..9:
	AOB(({ CB_DB_BONUS, CB_STUNNED, CB_ACTION_MOD }),
	    ({ ([ R2 : -20 ]), ([ R2 : 1 ]), ([ R2 : -10 ]) }));
	AO("An explosion of light and fire stuns you.\n", 0, MSG_SEE, 0);
	AT("An explosion of light stuns your opponent.\n", ao, MSG_SEE, 0);
	TW(({ QCTNAME(ao), " is stunned by an explosion of "+
		  "light.\n" }), ao, att);
	return 5;
      case 10..12:
	AOB(({ CB_DB_BONUS, CB_STUNNED, CB_ACTION_MOD }),
	    ({ ([ R4 : -45 ]), ([ R4 : 1 ]), ([ R12 : -15 ]) }));
	ao->add_bloodloss(4, hloc[HITLOC_SLOT]);
	AO("You feel as if you were showered with boiled " +
	   "water. Flashes seem to penetrate\nyour brain and " +
	   "blood starts to flow down your neck when the " +
	   "bubbles at your\nskin burst.\n", 0, 0, 0);
	AT("Eruptions of electricity surround the head of your "+
	   "opponent and let the skin\nstart to bubble. Blood "+
	   "starts to flow when the bubbles burst.\n", ao, MSG_SEE, 0);
	TW(({ "Eruptions of electricity surround the head of ",
		  QTNAME(ao), " and let the skin start to " +
		  "bubble. Blood starts to flow as the bubbles "+
		  "burst.\n" }), ao, att);
	return 15;
      case 13..15:
	if (check_metal_armor(hloc[HITLOC_SLOT], ao))
	{
	    AOB(({ CB_DB_BONUS, CB_STUNNED }),
		({ ([ R9 : -50 ]), ([ R9 : 1 ]) }));
	    AO("Flashes of electricity float along your "+
	       "helmet and fingers of cold heat\n"+
	       "are scratching into your face. Bright light "+
	       "explodes in your eyes and the smell\n"+
	       "of burned hair is the last that reach "+
	       "your nose.\n", 0, MSG_SEE, 0);
	    AT("Flashes of electricity float along the helmet "+
	       "of opponent and fingers of\ncold heat are "+
	       "scratching into the face.\n", ao, MSG_SEE, 0);
	    TW(({ "Flashes of electricity float along ",
		      QTNAME(ao), "'s helmet and fingers of cold heat are "+
		      "scratching the face.\n" }), ao, att);
	    if (sizeof(slots = ao->query_slot_type(SLT_NOSE)))
	    {
		AO("Aarrghh, the immense heat scorches "+
		   "your nose.\n", 0, 0, 0);
		AT("The nose is melts away.\n", ao, MSG_SEE, 0);
		TW(({"The immense heat melted away the nose.\n" }), ao, att);
		ao->set_wound(SLT_NOSE + (hloc[HITLOC_SLOT] % 100),
			      SLOT_SHATTERED);
	    }
	    return 5;
	}
	AOB(({ CB_STUNNED, CB_ACTION_MOD }),
	    ({ ([ R4 : 1 ]), ([ R12 : -50 ]) }));
	AO("A ball of bright light explodes right before your "+
	   "eyes and the flashes are\nsending thousands of "+
	   "needletwinges through your head.\n", 0, MSG_SEE, 0);
	AT("A ball of bright light explodes before the eyes of "+
	   "your foe and "+ ao->query_pronoun() +
	   "\nfalls down to the ground.\n", ao, MSG_SEE, 0);
	TW(({ "A ball of bright light explodes right before the "+
		  "eyes of ", QTNAME(ao), "sending ", ao->query_objective(),
		  " to the ground.\n" }), ao, att);
	ao->add_unconscious(20);
	return 10;
      case 16..18:
	AOB(({ CB_ACTION_MOD }),
	    ({ ([ time() + 18000/TIME_SCALE : -50 ]) }));
	AO("Smoke comes out of all your vents as your brain "+
	   "gets grilled by the\nelectricity shock.\n", 0, MSG_SEE, 0);
	AT("Smoke leaves the vents of your target's head while "+
	   "falling down to the\nground.\n", ao, MSG_SEE, 0);
	TW(({ "Smoke leaves the vents of ", QTNAME(ao),
		  " while falling down to the ground.\n" }), ao, att);
	ao->add_unconscious(120);
	return 10;
      case 19:
	ATB(({ CB_OB_BONUS }), ({ ([ R3 : 10 ]) }));
	AO("Your head is surrounded by a sphere of " +
	   "electricity. You feel flashes hit your\nhead and " +
	   "the heat burns your skin and skull to ashes which "+
	   "is blown away\nby a gentle breeze.\n", 0, MSG_SEE, 0);
	AT("In a globe of smoke and ozone the lifeless body "+
	   "drops down. When the\nstinking cloud has vanished "+
	   "you recognize that the head of your foe is no "+
	   "longer\navailable. You couldn't imagine that "+
	   "anybody is able to handle this loss.\n", ao, MSG_SEE, 0);
	TW(({ "In a globe of smoke and ozone the lifeless body of ",
		  QTNAME(ao), " drops down. When the "+
		  "stinking cloud has vanished you recognize "+
		  "that the head is no longer available. "+
		  "You couldn't imagine that anybody "+
		  "is able to handle this loss.\n" }), ao, att);
	ao->do_die(att);
	return 0;
      default:
	ATB(({ CB_OB_BONUS }), ({ ([ R1 : 20 ]) }));
	AO("Your whole nervous system acts like a super" +
	   "conductor before your body bursts\ninto flames " +
	   "and nearly explodes...\nslowly you return to the " +
	   "dust you came from.\n", 0, 0, 0);
	AT("Your foe's nervous system acts like a super" +
	   "conductor! Funny, seems as if he\nis dancing. " +
	   "Finally, providing you with a great light show, " +
	   "the body nearly\nexplodes in flames and is burned " +
	   "to dust.\n", ao, MSG_SEE, 0);
	TW(({ QCTNAME(ao), "'s nervous system acts like a "+
		  "superconductor!! Funny, seems as if ",
		  ao->query_pronoun(), " is dancing. Finaly, "+
		  "providing you with a great light show, the "+
		  "body nearly explodes in flames and is "+
		  "burned to dust.\n" }), ao, att);
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
	if (check_metal_armor(hloc[HITLOC_SLOT], ao))
	{
	    AOB(({ CB_STUNNED }), ({ ([ R2 : 1 ]) }));
	    AO("Electricity flickers over your armor.\n", 0, MSG_SEE, 0);
	    AT("Cold blue flames shortly flicker over your "+
	       "opponent's armor.\n", ao, MSG_SEE, 0);
	    TW(({ "Blue flames shortly flicker over ", QTNAME(ao),
		      "'s armor.\n" }), ao, att);
	    return 5;
	}
	AOB(({ CB_STUNNED }), ({ ([ R1 : 1 ]) }));
	if (ao->query_state() & ON_GROUND)
	    return 3;
	AO("A soft itching delays your reactions.\n", 0, 0, 0);
	return 2;
      case 9..10:
	if (check_metal_armor(hloc[HITLOC_SLOT], ao))
	{
	    AOB(({ CB_ACTION_MOD, CB_STUNNED }),
		({ ([ R4 : -25 ]), ([ R4 : 1 ]) }));
	    AO("Electricity flickers over your armor and an "+
	       "immense heat starts to scorch\nyour skin.\n", 0, MSG_SEE, 0);
	    AT("Cold blue flames flicker over your foe's " +
	       "armor and flames leak at the skin.\n", ao, MSG_SEE, 0);
	    TW(({ "Cold blue flames flicker over ", QTNAME(ao),
		      "'s armor and flames leak at the skin.\n" }), ao, att);
	    return 12;
	}
	AOB(({ CB_DB_BONUS, CB_STUNNED }),
	    ({ ([ R4 : -15 ]), ([ R2 : 1 ]) }));
	AO("An explosion of light and fire stuns you and the "+
	   "hit nearly knocks you down.\n", 0, MSG_SEE, 0);
	AT("An explosion of light and fire hits your opponent "+
	   "at the " + hloc[HITLOC_DESC] + ".\n", ao, MSG_SEE, 0);
	TW(({ "An explosion of light and fire hits ", QTNAME(ao),
		  " at the " + hloc[HITLOC_DESC] + ".\n" }), ao, att);
	return 7;
      case 11..12:
	AOB(({ CB_ACTION_MOD, CB_DB_BONUS, CB_STUNNED }),
	    ({ ([ R4 : -25 ]), ([ R2 : -30 ]), ([ R4 : 1 ]) }));
	AO("Flames crawl over your " + hloc[HITLOC_DESC] +
	   " grilling it to bleeding bubbles.\n", 0, MSG_SEE, 0);
	AT("Your foe starts to bleed when the bubbles at the "+
	   "skin burst.\n", ao, MSG_SEE, 0);
	TW(({QCTNAME(ao), " starts to bleed when the bubbles "+
		 "at the skin burst.\n" }), ao, att);
	ao->add_bloodloss(1, hloc[HITLOC_SLOT]);
	return 15;
      case 13..14:
	if (check_metal_armor(hloc[HITLOC_SLOT], ao))
	{
	    AOB(({ CB_DB_BONUS, CB_STUNNED }),
		({ ([ R2 : -50 ]), ([ R2 : 1 ]) }));
	    AO("Electricity flickers over your armor "+
	       "and boiling bubbles burst with a flow\n"+
	       "of blood.\n", 0, MSG_SEE, 0); 
	    AT("Cold blue flames burn the skin of your " +
	       "opponent and reveal blood.\n", ao, MSG_SEE, 0);
	    TW(({"Cold blue flames burn the skin of ", QTNAME(ao),
		     " and reveal the blood.\n" }), ao, att);
	    ao->add_bloodloss(2, hloc[HITLOC_SLOT]);
	    return 5;
	}
	AOB(({ CB_ACTION_MOD, CB_STUNNED }),
	    ({ ([ R2 : -50 ]), ([ R6 : 1 ]) }));
	AOBS("A blast of energy hits your " + hloc[HITLOC_DESC] +
	     " and bursts your skin.\n", 0, MSG_SEE, 0);
	ATBS("Your blast burns the " + hloc[HITLOC_DESC] +
	     " of your enemy.\n", ao, MSG_SEE, 0);
	TW(({"An energy blast burns the " + hloc[HITLOC_DESC] +
		 " of ", QTNAME(ao), ".\n" }), ao, att);
	ao->add_bloodloss(3, hloc[HITLOC_SLOT]);
	return 5;
      case 15..16:
	AOB(({ CB_DB_BONUS, CB_STUNNED, CB_ACTION_MOD }),
	    ({ ([ R3 : -25 ]), ([ R3 : 1 ]), ([ R6 : -25 ]) }));
	ao->add_bloodloss(3, hloc[HITLOC_SLOT]);
	AOBS("An energy force hits your " + hloc[HITLOC_DESC] +
	     " like a titans fist, scorching your skin\n", 0, MSG_SEE, 0);
	ATBS("The energy force you envoke hits the " + hloc[HITLOC_DESC] +
	     " of your opponent, scorching the skin.\n", ao, MSG_SEE, 0);
	TW(({ QCTNAME(ao), "'s " + hloc[HITLOC_DESC] + " is hit " +
		  "by an energy force that is scorching the skin.\n" }),
	   ao, att);
	return 15;
      case 17..18:
	AOB(({ CB_ACTION_MOD }),
	    ({ ([ (time() + 21600/TIME_SCALE) : -20 ]) }));
	AO("You are hit by an electrical forcefield that "+
	   "knocks you back. The world\nstarts to twist and "+
	   "darkness comes upon you.\n", 0, MSG_SEE, 0);
	AT("Your target is knocked back, lurks around and " +
	   "drops down unconscious.\n", ao, MSG_SEE, 0);
	TW(({QCTNAME(ao), " is knocked back, lurks around and "+
		 "drops down unconscious.\n" }), ao, att);
	ao->add_unconscious(240);
	ao->add_bloodloss(1, hloc[HITLOC_SLOT]);
	return 15;
      case 19:
	if (check_metal_armor(hloc[HITLOC_SLOT], ao))
	{
	    AOBS("A ball of pure energy hits your " + hloc[HITLOC_DESC] +
		 ". Fingers of electricity float over your armor heating " +
		 "it to a red glowing mass of metal. You try to remove " +
		 "it with your hands, but it's already burning through " +
		 "your breast and scorches your heart and lunge to tiny " +
		 "crisps.\n", 0, MSG_SEE, 0);
	    ATBS("Your enemy's armor starts to glow and it's "+
		 "burning right through the " + hloc[HITLOC_DESC] +
		 ", boiling and grilling all the inner organs.\n",
		 ao, MSG_SEE, 0);
	    TW(({ QCTNAME(ao), "'s armor starts to glow and " +
		      "burns right through the " + hloc[HITLOC_DESC] +
		      ", boiling and grilling all the inner organs.\n" }),
	       ao, att);
	    ao->add_unconscious(90);
	    ao->do_die(att);
	    return 0;
	}
	AOBS("An incredible heat makes your " + hloc[HITLOC_DESC] +
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
	AO("A stroke of pure energy hits you and devides you "+
	   "into two halfs faster than\nyou could send a last "+
	   "prayer or twinkling with your eyes.\n", 0, MSG_SEE, 0); 
	AT("The blast of energy you envoked cuts your target " +
	   "in two halfs and continues\nto a point beyond.\n",
	   ao, MSG_SEE, 0);
	TW(({"The blast of energy cuts ", QTNAME(ao), " in two "+
		 "halfs and continues to a point beyond.\n" }), ao, att);
	ao->do_die(att);
	/* spell goes on beyond target */
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
	if (check_metal_armor(hloc[HITLOC_SLOT], ao))
	{
	    AOB(({ CB_DB_BONUS, CB_STUNNED }),
		({ ([ R2 : -15 ]), ([ R2 : 1 ]) }));
	    AO("The metal you wear begins to buzz and the " +
	       "smell of danger is in the air.\n", 0, MSG_HEAR, 0);
	    AT("Your enemy's armor starts to buzz.\n", ao, MSG_HEAR, 0);
	    TW(({ QCTNAME(ao), "'s armors starts to buzz.\n" }), ao, att);
	    return 5;
	}
	AOB(({ CB_STUNNED }), ({ ([ R1 : 1 ]) }));
	AO("You are hit by a minor electricity shock.\n", 0, 0, 0);
	return 3;
      case 6..7:
	AOB( ({ CB_DB_BONUS, CB_STUNNED }),
	     ({ ([ R2 : -10 ]), ([ R1 : 1 ]) }));
	if (ao->query_state() & ON_GROUND)
	    return 7;
	AO("A blast of energy makes you stumbling.\n", 0, 0, 0);
	AT("Your blast makes your enemy stumbling.\n", ao, MSG_SEE, 0);
	TW(({ "A blast of energy makes ", QTNAME(ao), " stumbling.\n" }),
	   ao, att);
	return 6;
      case 8..9:
	if (drop_hold(hloc[HITLOC_SLOT], ao))
	{
	    AOB(({ CB_DB_BONUS, CB_STUNNED }),
		({ ([ R2 : -25 ]), ([ R2 : 1 ]) }));
	    AO("You are hit by an explosion of electricity, "+
	       "which scorches your skin and causes\nyour " +
	       "blood flow. Cause of the pain you drop " +
	       "everything you are holding\nin your hands.\n",
	       0, MSG_SEE, 0);
	    AT("Your opponent is scorched by an explosion of "+
	       "electricity, starts to bleed\nand drops all "+
	       "holding in the hands.\n", ao, MSG_SEE, 0);
	    TW(({ QCTNAME(ao), " is scorched by an explosion of "+
		      "electricity, starts to bleed and drops " +
		      "all holding in the hands.\n" }), ao, att);
	    ao->add_bloodloss(2, hloc[HITLOC_SLOT]);
	    return 10;
	}
	AOB(({ CB_STUNNED }), ({ ([ R6 : 1 ]) }));
	AO("A blast of electricity hits you, delaying your "+
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
	AO("An explosion of light and fire suddenly stuns "+
	   "you and makes your eyes burn.\n", 0, 0, 0);
	AT("Your opponent is stunned by an explosion of "+
	   "light and fire.\n", ao, MSG_SEE, 0);
	TW(({QCTNAME(ao), " is stunned by an explosion of light "+
		 "and fire.\n" }), ao, att);
	return 20;
      case 12..13:
	if (check_metal_armor(hloc[HITLOC_SLOT], ao))
	{
	    AOBS("Pure energy seems to crawl up your " + hloc[HITLOC_DESC] +
		 ". You start to quiver in convulsions and it turns " +
		 "dark before your eyes.\n", 0, MSG_SEE, 0); 
	    AT("Your opponent drops down convulsively.\n", ao, MSG_SEE, 0);
	    TW(({ QCTNAME(ao), " drops down convulsively.\n" }), ao, att);
	    ao->add_unconscious(4000/TIME_SCALE);
	    return 10;
	}
	AOB(({ CB_DB_BONUS, CB_STUNNED }),
	    ({ ([ R6 : -20 ]), ([ R9 : 1 ]) }));
	AO("A flash of energy seems to crawl up your spine and " +
	   "your heart takes some\nextra beats this minute.\n", 0, 0, 0);
	AT("Your opponent is hit by your electrical force.\n",
	   ao, MSG_SEE, 0);
	TW(({ QCTNAME(ao), " is hit by an electrical force.\n" }), ao, att);
	return 15;
      case 14..15:
	if (weap = ao->query_wield(hloc[HITLOC_SLOT]))
	{
	    if (!weap->query_prop(OBJ_MAGIC) && !random(2))
	    {
		AOB(({ CB_ACTION_MOD }), ({([ R6 : -25 ])}));
		AO("Suddenly your weapon is surrounded by " +
		   "blue flashes and starts to melt away.\n", 0, MSG_SEE, 0);
		AT("Your attack makes the weapon of your "+
		   "opponent melting down.\n", ao, MSG_SEE, 0);
		TW(({ "Suddenly ", QTNAME(ao), "'s weapon melts.\n" }),
		   ao, att);
		remove_object(weap);
		return 20;
	    }
	    AOB(({ CB_ACTION_MOD }), ({ ([ R6 : -15 ]) }));
	    AO("A sudden pain makes you loosing your weapon.\n",
	       0, MSG_SEE, 0);
	    AT("Your opponent lost the weapon.\n", ao, MSG_SEE, 0);
	    TW(({ QCTNAME(ao), " lost the weapon.\n" }), ao, att);
	    weap->set_coordinates(ao->query_coordinates());
	    weap->move(E(ao));
	}
	return 15;
      case 16:
	if (check_metal_armor(hloc[HITLOC_SLOT], ao))
	{
	    AOB(({ CB_DB_BONUS, CB_STUNNED }),
		({ ([ R6 : -50 ]), ([ R6 : 1 ]) }));
	    AOBS("Electricity flickers over your armor "+
		 "and boiling bubbles burst with a flow "+
		 "of blood at your " + hloc[HITLOC_DESC] + ".\n", 0,
		 MSG_SEE, 0);
	    ATBS("Electricity flickers over your opponent's "+
		 "armor and boiling bubbles burst at the " +
		 hloc[HITLOC_DESC] + ".\n", ao, MSG_SEE, 0);
	    TW(({ "Electricity flickers over the armor of ",
		      QTNAME(ao), " and boiling bubbles burst at the "+
		      hloc[HITLOC_DESC] + ".\n" }), ao, att); 
	    ao->add_bloodloss(3, hloc[HITLOC_SLOT]);
	    return 12;
	}
	AOBS("A sudden hit of energy hits your " +
	     hloc[HITLOC_DESC] + ". A sharp pain emitts from "+
	     "the bone and then you can't feel your " +
	     hloc[HITLOC_DESC] + " anymore.\n", 0, MSG_SEE, 0);
	ATBS("You hit the " + hloc[HITLOC_DESC] + " of your enemy " +
	     "with a bolt of concentrated energy.\n", ao, MSG_SEE, 0);
	TW(({ QCTNAME(att), " hits the "+ hloc[HITLOC_DESC] + " of ",
		  QTNAME(ao), " with a bolt of concentrated energy.\n" }),
	   ao, att);
	ao->set_wound(hloc[HITLOC_SLOT], SLOT_UNUSABLE);
	ao->add_unconscious(30);
	return 18;
      case 17:
	AOB(({ CB_ACTION_MOD, CB_STUNNED }),
	    ({ ([ R24 : -40 ]), ([ R4 : 1 ]) }));
	AOBS("An energy force hits your " + hloc[HITLOC_DESC] +
	     " like a titan's fist, scorches your skin "+
	     "and ripps your flesh.\n", 0, MSG_SEE, 0);
	ATBS("The energy force you envoke hits the " + hloc[HITLOC_DESC] +
	     " of your target, ripping the flesh and scorching " +
	     "the skin.\n", ao, MSG_SEE, 0);
	TW(({ "An energy force hits the " + hloc[HITLOC_DESC] +
		  " of ", QTNAME(ao), ", ripping the flesh and " +
		  "scorching the skin.\n" }), ao, att);
	ao->add_bloodloss(3, hloc[HITLOC_SLOT]);
	return 15;
      case 18:
	if (check_metal_armor(hloc[HITLOC_SLOT], ao))
	{
	    AOB(({ CB_ACTION_MOD, CB_STUNNED }),
		({ ([ R24 : -35 ]), ([ R9 : 1 ]) }));
	    AOBS("You are surrounded by a sphere of " +
		 "electricity. You feel flashes hit your " +
		 hloc[HITLOC_DESC] + " and the heat burns your skin " +
		 "and flesh.\n", 0, MSG_SEE, 0);
	    ATBS("Your foe is surrounded by a sphere of " +
		 "electricity. Flashes are hitting the " +
		 hloc[HITLOC_DESC] + ".\n", ao,MSG_SEE, 0);
	    TW(({ QCTNAME(ao), " is surrounded by a sphere of " +
		      "electricity. Flashes are hitting the " +
		      hloc[HITLOC_DESC] +".\n" }), ao, att);
	    ao->add_bloodloss(6, hloc[HITLOC_SLOT]);
	    return 10;
	}
	AOBS("You are surrounded by a sphere of "+
	     "electricity. You feel flashes hit your " +
	     hloc[HITLOC_DESC] + " and the heat burns your "+
	     "skin and flesh.\n", 0, MSG_SEE, 0);
	ATBS("Your foe is surrounded by a sphere of "+
	     "electricity. Flashes are hitting the " +
	     hloc[HITLOC_DESC] + ".\n", ao, MSG_SEE, 0);
	TW(({ QCTNAME(ao), " is surrounded by a sphere of "+
		  "electricity. Flashes are hitting the "+
		  hloc[HITLOC_DESC] +".\n" }), ao, att);
	ao->do_die(att);
	return 15;
      case 19:
	ATB(({ CB_OB_BONUS }), ({ ([ R1 : 15 ]) }));
	AOBS("You feel an incredible heat makes your " +
	     hloc[HITLOC_DESC] + " burst and crawls further in "+
	     "your veins. But then it seems the pain gets "+
	     "weaker and the heat colder and nearly all your "+
	     "life, although it could have last a little "+
	     "longer in your opinion, passes your inner eye. "+
	     "And then there is nothing...\n", 0, MSG_SEE, 0);
	ATBS("With the " + hloc[HITLOC_DESC] + " totally "+
	     "burned your foe's lifeless body drops to " +
	     "the ground.\n", ao, MSG_SEE, 0);
	TW(({" With the "+ hloc[HITLOC_DESC] +" totally burned ",
		 QTNAME(ao), "'s lifeless body drops to the ground.\n" }),
	   ao, att);
	ao->do_die(att);
	/* all around (10 feet) need to be stunned */
	return 0;
      default:
	ATB(({ CB_OB_BONUS }), ({ ([ R1 : 25 ]) }));
	AO("Your whole nervous system acts like a super" +
	   "conductor before your body bursts\ninto flames " +
	   "and nearly explodes...\nslowly you return to " +
	   "the dust you came from.\n", 0, 0, 0);
	AT("Your foe's nervous system acts like a super" +
	   "conductor! Funny, seems as if\nhe is dancing. " +
	   "Finally, providing you with a great light show, " +
	   "the body nearly\nexplodes in flames and is burned " +
	   "to dust.\n", ao, MSG_SEE, 0);
	TW(({ QCTNAME(ao), "'s nervous system acts like a "+
		  "superconductor!! Funny, seems as if " +
		  ao->query_pronoun() + " is dancing. Finally, "+
		  "providing you with a great light show, the "+
		  "body nearly explodes in flames and is "+
		  "burned to dust.\n" }), ao, att);
	ao->do_die(att);
	remove_object(ao);
	/* Body turns to dust and cannot be resurrected */
	return 0;	
    }
}
