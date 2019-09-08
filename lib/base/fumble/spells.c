#include "fumble.h"

/*
 * Function name: fumble_non_attack
 * Description:   handles nonattack spell fumbles
 * Arguments:     caster - the casting player/object
 *                roll - fumble roll
 * Returns:       damage or -1 if no mana is lost
 */
nomask int
fumble_non_attack(object caster, int roll)
{
    string  realm, spell, list;
    mapping prop;

    switch (roll)
    {
      case 0..20:
	  C("For a short moment you fear that you would have forgotten " +
	    "the magical words, but then then you\nenvoke the magic " +
	    "correctly.\n", 0, 0, 0);
	  TE(({ "For a short moment ", QTNAME(caster), " looks a "+
		    "little confused.\n" }), MSG_SEE, 0); 
	  return 0;
      case 21..60:
	  C("The mental pressure gets too strong. You feel somehow muddled " +
	    "for a moment\nand the spell is lost.\n", 0, 0, 0);
	  TE(({ "Perplexity shimmers on ", QTNAME(caster), "'s face.\n" }),
	     MSG_SEE, 0);
	  return -1;
      case 61..80:
	  CB(({ CB_DB_BONUS, CB_STUNNED }),
	     ({ ([ R1 : -10 ]), ([ R1 : 1 ]) }) );
	  C("You cast the spell, but nothing happens which is very " +
	    "confusing to you.\n", 0, 0, 0);
	  TE(({ QCTNAME(caster), " seems to have finished the casting, " +
		    "but something must have went wrong, cause " +
		    caster->query_pronoun() + " looks really surprised.\n" }),
	     MSG_SEE, 0);
	  return -1;
      case 81..95:
	  CB(({ CB_DB_BONUS, CB_STUNNED }),
	     ({ ([ R2 : -15 ]), ([ R2 : 1 ]) }) );
	  C("A subconscious fear paralyses you and you messed up " +
	    "the spell.\n", 0, 0, 0);
	  TE(({ "Suddenly ", QTNAME(caster), "'s eyes brighten with " +
		    "fear and " + caster->query_possessive() + " movement " +
		    "slows down.\n" }), MSG_SEE, 0); 
	  return 0;
      case 96..100:
	  CB(({ CB_DB_BONUS, CB_STUNNED }),
	     ({ ([ R12 : -25 ]), ([ R4 : 1 ]) }) );
	  C("Your mouth produces only flattering noises and "+
	    "you failed to cast the spell.\n", 0, 0, 0);
	  TE(({ QCTNAME(caster), "'s tongue seems to be tumid, cause he "+
		    "only produces flattering noises.\n" }), MSG_SEE, 0);
	  return 5;
      case 101..125:
	  CB(({ CB_DB_BONUS, CB_STUNNED }),
	     ({ ([ R3 : -50 ]), ([ R3 : 1 ]) }) );
	  C("The enforcement of magic power delays your reactions. The " +
	    "spell strays\nand travels to a point far away.\n", 0, 0, 0);
	  TE(({ "The magical power ", QTNAME(caster), " tried to envoke "+
		    "strays and vanishes.\n" }), MSG_SEE, 0);
	  return 10;
      case 126..150:
	  CB(({ CB_ACTION_MOD, CB_DB_BONUS, CB_STUNNED }),
	     ({ ([ R12 : -50 ]), ([ R4 : -25 ]), ([ R4 : 1 ]) }) );
	  C("An apparently inconvenient distraction causes you to cast a " +
	    "totally useless\nspell and makes you feel dizzy.\n", 0, 0, 0);
	  TE(({ QCTNAME(caster), " finishes the casting and the air "+
		    "around him starts to blurr. Then you hear a noise "+
		    "as if an old giant takes its last breath.\n" }),
	     MSG_SEE, 0);
	  return 5;
      case 151..175:
	  CB(({ CB_ACTION_MOD, CB_DB_BONUS, CB_STUNNED }),
	     ({ ([ R12 : -50 ]), ([ R6 : -50 ]), ([ R6 : 1 ]) }) );
	  C("You have to pay toll to the severe strain and so you misfire " +
	    "the spell.\n", 0, 0, 0);
	  TE(({ QCTNAME(caster), " looks very concentrated and and some "+
		    "sweat is running down the forehead. Suddenly "+
		    caster->query_possessive() + " face warps from the "+
		    "strain and strange eruptions of energy emit "+
		    "from the body. Maybe you should take a "+
		    "step beside...?\n" }), MSG_SEE, 0);
	  return 5;
      case 176..185:
	  C("Your lips are forming some words and your mind pictures of the " +
	    "spell as a\ncruel pain makes you falling down and blood drips " +
	    "down from your nose.\n", 0, 0, 0);
	  TE(({ QCTNAME(caster), "'s lips are forming some words, but "+
		    caster->query_pronoun() +" suddenly falls down to "+
		    "the ground and some blood flows out of "+
		    caster->query_possessive() +" nose.\n" }), MSG_SEE, 0);
	  caster->add_bloodloss(3, SLT_NOSE+1);
	  return 8;
      case 186..191:
	  CB(({ CB_ACTION_MOD }),
	     ({ ([ time() + 43200/TIME_SCALE : -50 ]) }) );
	  C("Your lips are forming some words and your mind pictures of " +
	    "the spell as a\ncruel pain makes you falling down and " +
	    "lose conscious.\n", 0, 0, 0);
	  TE(({ QCTNAME(caster), "'s lips are forming some words, but "+
		    caster->query_pronoun() +" suddenly falls down to "+
		    "the ground.\n" }), MSG_SEE, 0);
	  caster->add_unconscious(21600/TIME_SCALE);
	  return 10;
      case 192..195:
	  C("You internalize the spell and bright flashes explode before " +
	    "your inner eyes.\nYou feel nearly facilitated when it's " +
	    "getting dark around you.\n", 0, 0, 0);
	  TE(({ "Suddenly ", QTNAME(caster), " screams and drops down "+
		    "unconscious with pain in the eyes.\n" }), MSG_SEE, 0); 
	  caster->add_unconscious(43200/TIME_SCALE);
	  if (mappingp(prop = caster->query_prop(LIVE_KNOWN_SPELLS)))
	  {
	      spell = object_name(previous_object());
	      sscanf(spell, SPELL_DIR + "/%s/%*s/%s/%*s", realm, list);
	      if (prop[realm])
		  prop[realm][list] = 0;
	      caster->add_prop(LIVE_KNOWN_SPELLS, prop);
	  }
	  return 25;
      case 196..250:
	  CB(({ CB_ACTION_MOD }),
	     ({ ([ time() + 259200/TIME_SCALE : -50 ]) }) );
	  C("As you tried to cast the spell you fumble and the feedback\n" +
	    "of magic powers plowes through your head and knocks you to " +
	    "the ground.\n", 0, 0, 0);
	  TE(({ QCTNAME(caster), " looks confused, stumbles around and "+
		    "down to the ground.\n" }), MSG_SEE, 0);
	  caster->add_state(ON_GROUND);
	  if (mappingp(prop = caster->query_prop(LIVE_KNOWN_SPELLS)))
	  {
	      spell = object_name(previous_object());
	      sscanf(spell, SPELL_DIR + "/%s/%*s/%s/%*s", realm, list);
	      if (prop[realm])
		  prop[realm][list] = 0;
	      caster->add_prop(LIVE_KNOWN_SPELLS, prop);
	  }
	  return 20;
      case 251..300:
	  CB(({ CB_ACTION_MOD, CB_DB_BONUS, CB_STUNNED }),
	     ({ ([ time() + 86400/TIME_SCALE : -50 ]),
		    ([ R12 : -50 ]), ([ R12 : 1 ]) }) );
	  C("As you start to concentrate on the spell the mental pressure " +
	    "increases more\nand more. Blood starts to flow out of eyes, "+
	    "ears and nose...\nand then a large fist seems to squeeze " +
	    "your brain and you lose your conscious.\n", 0, 0, 0);
	  TE(({ "Looking in ", QTNAME(caster), "'s face you can see the "+
		    "mental pressure increasing and then blood starts "+
		    "to flow over the face, dripping out of "+
		    caster->query_possessive() +" eyes, ears and "+
		    "nose.\n" }), MSG_SEE, 0);
	  /* lose spell, only relearnable after one year */
	  caster->add_unconscious(4800/TIME_SCALE);
	  caster->add_bloodloss(3, SLT_HEAD+1);
	  return 25;
      default:
	  C("Uarg, you feel the magic power inside of you growing like " +
	    "never before, the\nmagic you envoke is strong enough to " +
	    "compete with the power of the valar...\nand then there is only "+
	    "darkness...\n", 0, 0, 0);
	  TE(({ QCTNAME(caster), "'s face brightens and the eyes gleam. " +
		    "Waves of incredible strong magic radiate from "+
		    caster->query_possessive() +" body and with "+
		    "maniac laughter " + caster->query_pronoun() + 
		    " suddenly dies.\n" }), MSG_SEE, 0);
	  caster->do_die(this_object());
	  return 0;
    }
}

/*
 * Function name: fumble_attack
 * Description:   handles attack spell fumbles
 * Arguments:     caster - the casting player/object
 *                roll - fumble roll
 * Returns:       damage or -1 if no mana is lost
 */
nomask int
fumble_attack(object caster, int roll)
{
    int     i, sz, *slots;
    string  realm, spell, list;
    mapping prop;

    switch (roll)
    {
      case 0..20:
	  C("The strain is too much for you... You lose the spell.\n",0,0,0);
	  TE(({ "For a short moment ", QTNAME(caster), " looks a "+
		    "little bit confused.\n" }), MSG_SEE, 0); 
	  return -1;
      case 21..30:
	  CB( ({ CB_ACTION_MOD }), ({([ R1 : -50 ]) }) );
	  C("Mild mental lapse causes indecision. You feel somehow muddled " +
	    "for a moment\nand the spell is lost.\n", 0, 0, 0);
	  TE(({ QCTNAME(caster), " seems to be undecided about what to "+
		    "do next.n"}), MSG_SEE, 0);
	  return -1;
      case 31..40:
	  CB(({ CB_DB_BONUS, CB_STUNNED }),
	     ({ ([ R1 :-25 ]), ([ R1 : 1 ]) }));
	  C("A mental lapse makes you only react stagnantly and you " +
	    "lose the spell.\n", 0, 0, 0);
	  TE(({ QCTNAME(caster), "'s thoughts seem to drift apart and "+
		    caster->query_pronoun() + " reacts stagnantly.\n" }),
	     MSG_SEE, 0);
	  return -1;
      case 41..60:
	  CB(({ CB_DB_BONUS, CB_STUNNED }),
	     ({ ([ R1 : -30 ]), ([ R1 : 1 ]) }) );
	  C("Hmm, you cannot remember the spell exactly and so you throw " +
	    "a totally\nineffectual spell.\n", 0, 0, 0);
	  TE(({ QCTNAME(caster), " finishes the casting, but nothing "+
		    "of sense, exept some strange noises and colored "+
		    "flashes, happens.\n" }), MSG_SEE, 0);
	  return 0;
      case 61..75:
	  CB(({ CB_DB_BONUS, CB_STUNNED }),
	     ({ ([ R2 : -35 ]), ([ R2 : 1 ]) }) );
	  C("A subconscious fear paralyses you and you messed up the " +
	    "spell.\n", 0, 0, 0);
	  TE(({ QCTNAME(caster), "'s eyes brighten fearfully and "+
		    caster->query_possessive() + " movement seems to be "+
		    "slowed down.\n"}), MSG_SEE, 0);
	  return 2;
      case 76..90:
	  CB(({ CB_DB_BONUS, CB_STUNNED }),
	     ({ ([ R3 : -40 ]), ([ R3 : 1 ]) }) );
	  C("You feel your face getting warped by the strain and then " +
	    "you can't stand\nit anymore and you misfire the spell.\n",
	    0, 0, 0);
	  TE(({ "The face of ", QTNAME(caster), " gets warped by the "+
		    "mental strain and suddenly an eruption of energie "+
		    "emits from the hands without causing any harm.\n" }),
	     MSG_SEE, 0);
	  return 5;
      case 91..95:
	  CB(({ CB_DB_BONUS, CB_STUNNED }),
	     ({ ([ R6 : -50 ]), ([ R6 : 1 ]) }) );
	  C("The mental pressure makes you feel that your head will burst " +
	    "right\nnow and you collapse shrugging to the ground.\n", 0, 0, 0);
	  TE(({ "Suddenly ", QTNAME(caster), " collapse shrugging to "+
		    "the ground.\n"}), MSG_SEE, 0);
	  return 10;
      case 96..100:
	  caster->add_unconscious(43200/TIME_SCALE);
	  C("As you tried to cast the spell at your target you fumble and " +
	    "the feedback\nof magic powers plowes through your head and " +
	    "knocks you out.\n", 0, 0, 0);
	  TE(({ "It seems that ", QTNAME(caster), " did something wrong "+
		    "cause even you can feel the magic powers when "+
		    caster->query_pronoun() +" is knocked out.\n" }),
	     MSG_SEE, 0);
	  return 20;
      case 101..150:
	  CB(({ CB_DB_BONUS, CB_STUNNED }),
	     ({ ([ R2 : -50 ]), ([ R1 : 1 ]) }) );
	  C("The enforcement of magic power delays your reactions. The " +
	    "spell strays\nand travels to a point near your original " +
	    "target.\n", 0, 0, 0);
	  TE(({ "The magic power ", QTNAME(caster), " envoked strays "+
		    "and travels to a point it should not have traveled "+
		    "to, if you interpret the mien correctly.\n" }),
	     MSG_SEE, 0);
	  /* spell may hit someone/something else */
	  return 10;
      case 151..175:
	  C("A strong tickling next to your nose makes you lose your " +
	    "concentration and\nyou cast the spell right in the opposite " +
	    "direction. Due to this mistake your\nconnection to the "+
	    "resources of magic powers is cut and you were hit by a\n" +
	    "magical overflow.\n", 0, 0, 0);
	  TE(({ "A nerv next to ", QTNAME(caster), "'s nose tickles and "+
		    "seems to break the concentration. The energie flow "+
		    "surrounding "+ caster->query_objective() +" is cut "+
		    "and "+ caster->query_pronoun() +" falls down "+
		    "unconscious.\n" }), MSG_SEE, 0);
	  caster->add_unconscious(21600/TIME_SCALE);
	  /* spell goes opposite direction */
	  return 25;
      case 176..185:
	  CB(({ CB_ACTION_MOD }),
	     ({ ([ time() + 3628800/TIME_SCALE : -50 ]) }) );
	  C("You internalize the spell and feel something in your brain " +
	    "is damaged. As\nyou try to figure out what has happened, " +
	    "you realize that something is\nwrong with your knowledge "+
	    "about the magic realms and you feel somehow tired.\n", 0, 0, 0);
	  TE(({ QCTNAME(caster) ," squeezes "+ caster->query_possesive() +
		    "eyes in pain and then suddenly shakes the head "+
		    "as if "+ caster->query_pronoun() +" does not know "+
		    "what has happened.\n"}), MSG_SEE, 0);
	  if (mappingp(prop = caster->query_prop(LIVE_KNOWN_SPELLS)))
	  {
	      spell = object_name(previous_object());
	      sscanf(spell, SPELL_DIR + "/%s/%*s/%s/%*s", realm, list);
	      if (prop[realm])
		  prop[realm][list] = 0;
	      caster->add_prop(LIVE_KNOWN_SPELLS, prop);
	  }
	  return 30;
      case 186..200:
	  if (sz = sizeof(slots = caster->query_slot_type(SLT_LEG)))
	  {
	      for (i=0; i<sz; i++)
		  caster->set_wound(slots[i], SLOT_UNUSABLE);
	      C("A sharp pain explodes in your brain and runs down your " +
		"spine to your feet,\nparalysing everything from the waist "+
		"down.\n", 0, 0, 0);
	      TE(({ "Suddenly ", QTNAME(caster), " groans painfully "+
			"and then desperately scratches and pinches "+
			"the legs.\n" }), MSG_SEE, 0);
	      caster->command("groan");
	      return 0;
	  }
	  C("A sudden pain whips through your body and makes you quiver " +
	    "like jelly before\na beneficially darkness comes over " +
	    "your mind.\n", 0, 0, 0);
	  caster->add_unconscious(10000/TIME_SCALE);
	  return 10;
      case 201..250:
	  CB( ({ CB_ACTION_MOD }),
			       ({ ([ time() + 86400/TIME_SCALE : -75 ]) }) );
	  C("A sudden pain whips through your body and makes you quiver " +
	    "like jelly before\na beneficially darkness comes over " +
	    "your mind.\n", 0, 0, 0);
	  caster->add_unconscious(21600/TIME_SCALE);
	  return 40;
      case 251..300:
	  caster->add_unconscious(21000/TIME_SCALE);
	  C("As you start to concentrate on the spell the mental pressure " +
	    "increases more\nand more. Blood starts to flow out of eyes, " +
	    "ears and nose...\nand then a large fist seems to squeeze your " +
	    "brain and you lose your conscious.\n", 0, 0, 0);
	  TE(({ "Looking in ", QTNAME(caster), "'s face you can see the "+
		    "mental pressure increasing and then blood starts "+
		    "to flow over the face, dripping out of "+
		    caster->query_possessive() +" eyes, ears and nose.\n" }),
	     MSG_SEE, 0);
	  caster->do_die();
	  return 50;
      default:
	  C("Uarg, you feel the magic power inside of you growing like " +
	    "never before, the\nmagic you envoke is strong enough to " +
	    "compete with the power of the valar...\nand then the final "+
	    "curtain falls...\n", 0, 0, 0);
	  TE(({QCTNAME(caster), "'s face brightens and the eyes gleam.\n" +
		   "Waves of incredible strong magic radiate from "+
		   caster->query_possessive() +" body and with "+
		   "maniac laughter " + caster->query_pronoun() + 
		   " suddenly dies.\n" }), MSG_SEE, 0);
	  caster->do_die(this_object());
	  return 0;
    }
}
