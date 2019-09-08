/* **************************************************************************
 * These are the emotion functions. Please add new in alphabetical order.
 * soul_do_cmd(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) 
 * arguments are: 1 - the command argument given by the player
 *                2 - the default adverb
 *                3 - what to write on wrong usage
 *                4 - the respond to the player
 *                5 - the respond to the player, with target chosen
 *                6 - what target sees, if no target 0
 *                7 - what people around see
 *                8 - what people around see with target
 *                9 - emotion type, TARGET means: possible target,
 *                    ADVERB means: possible to give adverb,
 *                    both with bitcombination: ADVERB | TARGET,
 *                    0 is nothing extra
 *               10 - hear flag, should be 1 if emotion makes noise
 * **************************************************************************/

nomask int
ack(string str)
{
    return soul_do_cmd(str, 0, "Ack what?", "Ack!", 0, 0, " goes ack!",
		       0, 0, MSG_SEE);
}

nomask int
beckon(string str)
{
    return soul_do_cmd(str, 0, "Beckon what?", 0,
		       "You @ beckon to #.", " @ beckons to you.", 0,
		       " @ beckons to #.", ADVERB | TARGET, MSG_SEE);
}

nomask int
beg(string str)
{
    string poss;

    poss = this_player()->query_possessive();
    return soul_do_cmd(str, 0, "Beg what?", "You @ beg on your knees.",
		       "You @ beg on your knees in front of #.",
		       " @ begs on " + poss + " knees in front of you.",
		       " @ begs on " + poss + " knees.",
		       " @ begs on " + poss + " knees in front of #.",
		       ADVERB | TARGET, MSG_SEE);
}

nomask int
blush(string str)
{
    return soul_do_cmd(str, "profusely", "Blush what?", "You are blushing @.",
		       "You are blushing @ at #.", " blushes @ at you.",
		       " blushes @.", " blushes @ at #.", ADVERB | TARGET,
		       MSG_SEE);
}

nomask int
bop(string str)
{
    return soul_do_cmd(str, 0, "Whom are you trying to bop?",
		       "You bop yourself on your head.",
		       "You bop # on the head.",
		       " bops you on your head.", " bops " +
		       this_player()->query_objective() + "self on the head.",
		       " bops # on the head.",
		       TARGET | DISTANCE | LIVING, MSG_SEE);
}

nomask int
bow(string str)
{
    return soul_do_cmd(str, "gracefully", "Bow what?",
		       "You bow @.", "You @ bow to #.", " bows @ to you.",
		       " bows @.", " bows @ to #.", ADVERB | TARGET, MSG_SEE);
}

nomask int
brighten(string str)
{
    return soul_do_cmd(str, 0, "Brighten what?",
		       "You think about it, then it dawns on you! "+
		       "Your face brightens!", 0, 0,
		       " thinks about it, then "+
		       this_player()->query_possessive() +
		       " face brightens.", 0, 0, MSG_SEE);
}

nomask int
caress(string str)
{
    return soul_do_cmd(str, "carefully", "Caress whom?", 0, 
		       "You @ caress #.", " @ caresses you.", 0, 
		       " @ caresses #.", ADVERB | TARGET | DISTANCE, MSG_SEE);
}

nomask int
confused(string str)
{
    return soul_do_cmd(str, 0, "Confused what?", "You look very confused.", 0,
		       0, " looks very confused.", 0, 0, MSG_SEE);
}

nomask int
cringe(string str)
{
    return soul_do_cmd(str, 0, "Cringe what?",
		       "You @ cringe in terror.",
		       "You @ cringe in terror from #.",
		       " @ cringes in terror at your feet.",
		       " @ cringes in terror.",
		       " @ cringes at the feet of #.",
		       ADVERB | TARGET, MSG_SEE);
}

nomask int
cuddle(string str)
{
    return soul_do_cmd(str, 0, "Cuddle whom?", 0,
		       "You @ cuddle #.", " @ cuddles you.", 0,
		       " @ cuddles #.", ADVERB | TARGET, MSG_SEE);
}

nomask int
curtsey(string str)
{
    return soul_do_cmd(str, "gracefully", "Curtsey what?",
		       "You curtsey @.", "You curtsey @ to #.",
		       " curtseys @ to you.", " curtseys @.",
		       " curtseys @ to #.", ADVERB | TARGET, MSG_SEE);

}

nomask int
dance(string str)
{
    return soul_do_cmd(str, 0, "Dance with whom?",
		       "You dance @.  Feels silly, doesn't it?",
		       "You @ sweep # across the dance floor.",
		       " @ sweeps you across the dance floor.",
		       " @ dances a jig.",
		       " @ sweeps # across the dance floor.",
		       ADVERB | TARGET | DISTANCE, MSG_SEE);
}

nomask int
despair(string str)
{
    return soul_do_cmd(str, 0, "Despair what?",
		       "You @ tear a lump of hair off your scalp.", 0, 0,
		       " @ tears a lump of hair off " +
		       this_player()->query_possessive() +
		       " scalp in despair.", 0, ADVERB, MSG_SEE);
}

nomask int
dream(string str)
{
    return soul_do_cmd(str, 0, "Dream what?",
		       "You are dreaming something lovely...", 0, 0,
		       " seems to have some nice dreams...", 0, 0, MSG_SEE); 
}

nomask int
duh(string str)
{
    return soul_do_cmd(str, 0, "Duh what?",
		       "Uh, duhhh...  Feeling stupid, eh?",  0, 0,
		       " goes duhhh. Must be feeling dumb.", 0, 0, MSG_SEE);
}

nomask int
eeks(string str)
{
    return soul_do_cmd(str, 0, "Eeks what?", "Eeks!  Oh my gosh!", 0, 0,
		       " goes eeks!", 0, 0, MSG_SEE);
}

nomask int
emotesee(string str)
{
    object tp;

    tp = this_player();
    if (!str)
    {
	tp->catch_tell("Emote what?\n");
	return -1;
    }
#ifdef _LOG_EMOTE
    if (!tp->query_npc() && _ACCESS_LEVEL(geteuid(tp)) < 1)
	syslog("EMOTE", BS(tp->query_name() + " " + str, 76, 1));
#endif
    tp->echo("You emote: " + capitalize(tp->query_name()) + " " + str + "\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " " + str + "\n" }), MSG_SEE, tp);
    return 1;
}

nomask int
eyebrow(string str)
{
    string poss;
    
    poss = this_player()->query_possessive();
    return soul_do_cmd(str, 0, "Raise your eyebrow at whom?",
		       "You raise an eyebrow @.",
		       "You @ raise your eyebrow at #.",
		       " raises " + poss + " eyebrow at you.",
		       " @ raises " + poss + " eyebrow.",
		       " @ raises " + poss +  " eyebrow at #.",
		       ADVERB | TARGET, MSG_SEE);
}

nomask int
fawn(string str)
{
    return soul_do_cmd(str, 0, "Fawn upon whom?", 0, 
		       "You @ fawn upon #.", " @ fawns upon you.", 0,
		       " @ fawns upon #.", ADVERB | TARGET, MSG_SEE);
}

nomask int
flip(string str)
{
    if (str && sscanf(str, "coin%*s"))
    {
	object tp;
	
	if (!MONEY_TOTAL(tp = this_player()))
	{
	    tp->catch_tell("You have no coin.\n");
	    return -1;
	}
	if (!sscanf(str, "coin %s", str))
	    str = 0;
	if (soul_do_cmd(str, 0, "Flip coin how?", "You @ flip a coin.", 0,
			0, " @ flips a coin.", 0, ADVERB, MSG_SEE) == 1)
	{
	    tell_objects(LISTEN(E(tp)),
			 "It lands face " + (random(2) ? "down" : "up") +
			 ".\n", MSG_SEE, tp);
	    return 1;
	}
	return -1;
    }
    return soul_do_cmd(str, 0, "Flip what?", "You @ flip head over heels.", 0,
		       0, " @ flips head over heels.", 0, ADVERB, MSG_SEE);
}

nomask int
fondle(string str)
{
    return soul_do_cmd(str, 0, "Fondle whom?", 0, "You fondle # @.",
		       " @ fondles you.", 0, " @ fondles #.",
		       ADVERB | TARGET, MSG_SEE);
}

nomask int
french(string str)
{
    return soul_do_cmd(str, 0, "French whom?", 0, 
		       "You give # a REAL kiss...it takes a long time...",
		       " @ gives you a deep and passionate kiss...\n" +
		       "It seems to take forever...", 0, 
		       " gives # a deep and passionate kiss.\n" +
		       "It seems to take forever...Sexy, eh?",
		       ADVERB | TARGET | DISTANCE, MSG_SEE);
}

nomask int
frown(string str)
{
    return soul_do_cmd(str, 0, "Frown what?",
		       "You frown @. Is something wrong?", 0, 0,
		       " frowns @.", 0, ADVERB, MSG_SEE);
}

nomask int
fume(string str)
{
    return soul_do_cmd(str, "angrily", "Fume at whom?",
		       "You fume @.",
		       "You fume @ at #. Ooh, that feels good!",
		       " fumes @ at you.", " fumes @.",
		       " fumes @ at #.", ADVERB | TARGET, MSG_SEE);
}

nomask int
glare(string str)
{
    return soul_do_cmd(str, 0, "Glare at whom?", 0, "You glare @ at #.",
		       " @ glares at you.", 0,
		       " @ glares at #.", ADVERB | TARGET, MSG_SEE);
}

nomask int
grin(string str)
{
    return soul_do_cmd(str, 0, "Grin at whom?", "You grin @.",
		       "You grin @ at #.", " grins @ at you.",
		       " grins @."," grins @ at #.", ADVERB | TARGET, MSG_SEE);
}

nomask int
grope(string str)
{
    return soul_do_cmd(str, 0, "Grope whom?", 0, "You @ grope for #.",
		       " @ gropes for you.", 0, " @ gropes for #.",
		       ADVERB | TARGET, MSG_SEE);
}

nomask int
grovel(string str)
{
    return soul_do_cmd(str, 0, "Grovel in front of whom?", 0, 
		       "You fall to the ground in front of #, groveling @.",
		       " falls to the ground and @ grovels in front of you.",
		       0, " falls to the ground and @ grovels in front of #.",
		       ADVERB | TARGET, MSG_SEE);
}

nomask int
hop(string str)
{
    return soul_do_cmd(str, "happily", "Hop what?", "You @ hop around.", 0,
		       0, " @ hops around.", 0, ADVERB, MSG_SEE);
}

nomask int
hug(string str)
{
    return soul_do_cmd(str, 0, "Hug whom?", 0, "You hug # @.", " hugs you @.",
		       0, " hugs # @.", ADVERB | TARGET | DISTANCE, MSG_SEE);
}

nomask int
jump(string str)
{
    return soul_do_cmd(str, "unexpectedly", "Jump what?", "You jump @.", 0, 0,
		       " jumps @.", 0, ADVERB, MSG_SEE);
}

nomask int
kick(string str)
{
    string one, two, *zones;
    object tp;

    tp = this_player();
    if (!str)
    {
	tp->catch_tell("Whom are you trying to kick?\n");
	return -1;
    }

    zones = ({ "leg","buttock", "buttocks","legs","bottom","belly",
	       "back", "abdomen" });
    
    if (str == tp->query_name() || str == "me")
    {
	return soul_do_cmd(str, 0, "Kick whom?", "You try to kick yourself. "+
			   "Very strange!", 0, 0, " tries to kick " +
			   tp->query_objective() +
			   "self. Looks funny.", 0, 0, MSG_SEE);
    }
    
    if (sscanf(str,"%s in %s",one, two) == 2)
    {
        if (!IN_ARRAY(two, zones))
	{
	    tp->catch_tell("Whom are you trying to kick where?\n");
	    return -1;
	} 
	return soul_do_cmd(one, 0, "Kick whom?", 0, "You kick # in the " +
			   two + "!",
			   " kicks you in your " + two + ". OUCH!", 0,
			   " kicks # in the " + two + ".",
			   TARGET | DISTANCE, MSG_SEE);
    }
    return soul_do_cmd(str, 0, "Kick whom?", 0,
		       "You kick #. Yeah, I like that!",
		       " kicks you. OUCH!", 0,
		       " kicks #.", TARGET | DISTANCE, MSG_SEE);
}

nomask int
kiss(string str)
{
    return soul_do_cmd(str, 0, "Kiss whom?", 0, "You kiss # @.",
		       " kisses you @.", 0, " kisses # @.",
		       ADVERB | TARGET | DISTANCE, MSG_SEE);
}

nomask int
lick(string str)
{
    return soul_do_cmd(str, 0, "Lick whom?", 0,
		       "You @ lick #.", " @ licks you.", 0, " @ licks #.", 
		       ADVERB | TARGET | DISTANCE, MSG_SEE);
}

nomask int
listen(string str)
{
    return soul_do_cmd(str, 0, "Listen to whom?", 0,
		       "You @ listen to #.", " @ seems to listen to you.",
		       0, " @ seems to listen to #.", 
		       ADVERB | TARGET | DISTANCE, MSG_SEE);
}

nomask int
love(string str)
{
    return soul_do_cmd(str, 0, "Love whom?", 0,
		       "You @ tell your true feelings to #.",
		       " @ whispers sweet words of love to you.", 0,
		       " whispers softly to #.",
		       ADVERB | TARGET | DISTANCE, MSG_SEE);
}

nomask int
nibble(string str)
{
    return soul_do_cmd(str, 0, "Nibble on whose ear?", 0,
		       "You @ nibble on #'s ear."," @ nibbles on your ear.", 0,
		       " @ nibbles on #'s ear.",
		       ADVERB | TARGET | DISTANCE, MSG_SEE);
}

nomask int
nod(string str)
{
    return soul_do_cmd(str,"solemnly", "Nod what?", "You nod @.",
		       "You nod @ to #.", " nods @ to you.", " nods @.",
		       " nods @ to #.", ADVERB | TARGET, MSG_SEE);
}

nomask int
nudge(string str)
{
    return soul_do_cmd(str, 0, "Nudge whom?", 0, "You @ nudge #. ",
		       " @ nudges you.", 0, " @ nudges #.",
		       ADVERB | TARGET | DISTANCE, MSG_SEE);
}

nomask int
nuzzle(string str)
{
    return soul_do_cmd(str, 0, "Nuzzle whom?", 0, "You @ nuzzle #'s neck.",
		       " @ nuzzles your neck.", 0, " @ nuzzles #'s neck.",
		       ADVERB | TARGET | DISTANCE | LIVING, MSG_SEE);
}

nomask int
offer(string str)
{
    object tp, *list;

    tp = this_player();
    if (!str || !sscanf(str, "hand to %s", str))
    {
	tp->catch_tell("Offer what?\n");
	return -1;
    }
    list = tp->visible(I(E(tp), 2));
    list = parse(str, list, tp, tp->query_step());
    if (list[0]->query_prop("_hand_offer") < time() - 10)
    {
	tp->add_prop("_hand_offer", time());
	return soul_do_cmd(str, 0, "You can't see or reach that.", 0,
			   "You offer # your hand.", " offers you a hand.", 0,
			   " offers # a hand.",
			   TARGET | DISTANCE | LIVING, MSG_SEE);
    }
    list[0]->add_prop("_hand_offer", 0);
    return soul_do_cmd(str, 0, "You can't see or reach that.", 0,
		       "You shake #'s hand.", " shakes your hand.", 0,
		       " shakes #'s hand.",
		       TARGET | DISTANCE | LIVING, MSG_SEE);
}

nomask int
oops(string str)
{
    object tp;

    tp = this_player();
    tp->catch_tell("Oops up!\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ "Oops, ", QTNAME(tp),
		    " seems to have made a mistake.\n" }), MSG_SEE, tp);
    return 1;
}

nomask int
panic(string str)
{
    return soul_do_cmd(str, 0, "Panic what?", "PANIC!", 0, 0, 
		       " panics and looks for a place to hide.", 0,
		       0, MSG_SEE);
}

nomask int
pat(string str)
{
    string one, two, *zones;
    object tp;

    tp = this_player();
    if (!str)
    {
	tp->catch_tell("Whom are you trying to pat?\n");
	return -1;
    }
    
    zones = ({"head","shoulder","shoulders","back","forehead","cheek",
              "cheeks","buttocks","legs","bottom","belly"});
    
    if (str == tp->query_name() || str == "me")
    {
	if (sscanf(str,"%s on %s",one, two) == 2)
	{
	    if (!IN_ARRAY(two, zones))
	    {
		tp->catch_tell("Whom are you trying to pat where?\n");
		return -1;
	    }
	}
	else
	{
	    one = str;
	    two = "head";
	}
	
	return soul_do_cmd(one, 0, "Pat whom?",
			   "You pat yourself on your " + two + ".",
			   0, 0, " pats " + tp->query_objective() +
			   "self on the " + two + ".", 0, 0, MSG_SEE);
    }
    if (sscanf(str,"%s on %s",one, two) == 2)
    {
        if (!IN_ARRAY(two, zones))
	{
	    tp->catch_tell("Whom are you trying to pat where?\n");
	    return -1;
	} 
    
    }
    else
    {
	one = str;
	two = "head";
    }
    return soul_do_cmd(one, 0, "Pat whom?", 0, "You pat # on the " +
		       two + ".",
		       " pats you on your " + two + ".", 0,
		       " pats # on the " + two + ".",
		       TARGET | DISTANCE | LIVING, MSG_SEE);
}

nomask int
peer(string str)
{
    return soul_do_cmd(str, "intently", "Peer at whom?",
		       "You peer @.", "You peer @ at #. Hmm...",
		       " peers @ at you.", " peers @.", " peers @ at #.",
		       ADVERB | TARGET, MSG_SEE);
}

nomask int
pinch(string str)
{
    string one, two, *zones;
    object tp;

    tp = this_player();
    if (!str)
    {
	tp->catch_tell("Whom are you trying to pinch?\n");
	return -1;
    }
    
    zones = ({"head","shoulder","cheek","arm","arms","leg","buttock",
	      "cheeks","buttocks","legs","bottom","belly","nose","ear",
	      "ears"});
    
    if (str == tp->query_name() || str == "me")
    {
	if (sscanf(str,"%s in %s",one, two) == 2)
	{
	    if (!IN_ARRAY(two, zones))
	    {
		tp->catch_tell("Whom are you trying to pinch where?\n");
		return -1;
	    }
	}
	else
	{
	    one = str;
	    two = "arm";
	}
	
	return soul_do_cmd(one, 0, "Pinch whom?",
			   "You pinch yourself in your " + two + ". Ouch!",
			   0, 0, " pinches " + tp->query_objective() +
			   "self in the " + two + ".", 0, 0, MSG_SEE);
    }
    if (sscanf(str,"%s in %s",one, two) == 2)
    {
        if (!IN_ARRAY(two, zones))
	{
	    tp->catch_tell("Whom are you trying to pinch where?\n");
	    return -1;
	} 
    
    }
    else
    {
	one = str;
	two = "arm";
    }
    return soul_do_cmd(one, 0, "Pinch whom?", 0, "You pinch # in the " +
		       two + "!",
		       " pinches you in your " + two + ". Ouch!", 0,
		       " pinches # in the " + two + ".",
		       TARGET | DISTANCE | LIVING, MSG_SEE);
}

nomask int
point(string str)
{
    string *dir;
    object tp;

    tp = this_player();
    if (!str)
    {
	tp->catch_tell("Where do you want to point?\n");
	return -1;
    }
    dir = ({ "north", "south", "west", "east", "northwest", "northeast",
	     "southwest", "southeast", "up", "down" });
		 
    if (IN_ARRAY(str, dir))
	return soul_do_cmd(str, 0, "Point at whom?", "You point " + str + ".",
			   0, 0, " points " + str + ".", 0, 0, MSG_SEE);
    
    if (str == tp->query_name() || str == "me")
	return soul_do_cmd(str, 0, "Point at whom?", "You point at yourself.",
			   0, 0, " points at " + tp->query_objective() +
			   "self.", 0, 0, MSG_SEE);
    return soul_do_cmd(str, 0, "Point at whom?", 0,
		       "You point at #.", " points at you.",
		       0, " points at #.", TARGET, MSG_SEE);
}

nomask int
poke(string str)
{
    string *zones, one, two;
    object tp;

    tp = this_player();
    if (!str)
    {
	tp->catch_tell("Whom do you want to poke?\n");
	return -1;
    }

    zones = ({"eye","ear","thorax","abdomen","shoulder","ribs","nose",
              "stomach","belly"});
  
    if (sscanf(str,"%s in %s",one, two) == 2)
    {
        if (!IN_ARRAY(two, zones))
	{
	    tp->catch_tell("Whom are you trying to poke where?\n");
	    return -1;
	} 
    
    }
    else
    {
	one = str;
	two = "ribs";
    }
    
    return soul_do_cmd(one, 0, "Poke whom?", 0,
		       "You poke # in the " + two + ".",
		       " pokes you in the " + two + ".",
		       0, " pokes # in the " + two + ".",
		       TARGET | DISTANCE | LIVING, MSG_SEE);
}

nomask int
ponder(string str)
{
    return soul_do_cmd(str, 0, "Ponder what?", "You @ ponder the situation.",
		       0, 0, " @ ponders the situation.", 0, ADVERB, MSG_SEE);
}

nomask int
pout(string str)
{
    return soul_do_cmd(str, 0, "Pout what?",
		       "You pout @.  Ah, don't take it so hard.",
		       "You pout @ at #.", " pouts @ at you.",
		       " pouts @.", " pouts @ at #.",
		       ADVERB | TARGET, MSG_SEE);
}

nomask int
puke(string str)
{
    return soul_do_cmd(str, 0, "Puke on whom?",
		       "You @ puke on your shoes.", "You @ puke on #.",
		       " @ pukes all over you.", " doubles over and pukes @.",
		       " @ pukes on #.", ADVERB | TARGET | DISTANCE, MSG_SEE);
}

nomask int
rolleyes(string str)
{
    return soul_do_cmd(str, 0, "Roll your eyes what?",
		       "You roll your eyes in exasperation.", 0, 0, 
		       " rolls " + this_player()->query_possessive() +
		       " eyes in exasperation.", 0, 0, MSG_SEE);
}

nomask int
ruffle(string str)
{
    return soul_do_cmd(str, "playfully", "Ruffle whom?", 0,
		       "You ruffle #'s hair @.",
		       " ruffles your hair @.", 0,
		       " ruffles #'s hair @.",
		       ADVERB | TARGET | DISTANCE | LIVING, MSG_SEE);
}

nomask int
scratch(string str)
{
    return soul_do_cmd(str, 0, "Scratch how?", "You @ scratch your head.",
		       0, 0, " @ scratches " +
		       this_player()->query_possessive() + " head.",
		       0, ADVERB, MSG_SEE);
}

nomask int
shake(string str)
{
    string poss;

    poss = this_player()->query_possessive();
    
    return soul_do_cmd(str, 0, "Shake your head how?",
		       "You @ shake your head in disagreement.",
		       "You @ shake your head at #.",
		       " @ shakes " + poss + " head at you.",
		       " @ shakes " + poss + " head in disagreement.",
		       " @ shakes " + poss + " head at #.",
		       ADVERB | TARGET, MSG_SEE);
}

nomask int
shiver(string str)
{
    return soul_do_cmd(str, 0, "Shiver what?", "You shiver @.", 0, 0,
		       " shivers @.", 0, ADVERB, MSG_SEE);
}

nomask int
shrug(string str)
{
    return soul_do_cmd(str, "helplessly", "Shrug what?",
		       "You shrug @.", "You shrug @ at #.",
		       " shrugs @ at you.", " shrugs @.",
		       " shrugs @ at #.", ADVERB | TARGET, MSG_SEE);
}

nomask int
shudder(string str)
{
    return soul_do_cmd(str, 0, "Shudder what?",	"You shudder @.", 0, 0,
		       " shudders @.", 0, ADVERB, MSG_SEE);
}

nomask int
slap(string str)
{
    object tp;
    
    tp = this_player();
    if (str == tp->query_name() || str == "me")
    {
	return soul_do_cmd(str, 0, "Slap whom?",
			   "You slap yourself.",
			   0, 0, " slaps " + tp->query_objective() +
			   "self.", 0, 0, MSG_SEE);
    }
    return soul_do_cmd(str, 0, "Slap whom?", 0, "You slap # @.",
		       " slaps you @.", 0, " slaps # @.",
		       ADVERB | TARGET | DISTANCE, MSG_SEE);
}

nomask int
smile(string str)
{
    return soul_do_cmd(str, 0, "Smile at whom?", "You smile @.", 
		       "You smile @ at #.", " smiles @ at you.",
		       " smiles @.", " smiles @ at #.",
		       ADVERB | TARGET, MSG_SEE);
}

nomask int
smirk(string str)
{
    return soul_do_cmd(str, 0, "Smirk what?", "You smirk @.", 0, 0,
		       " smirks @.", 0, ADVERB, MSG_SEE);
}

nomask int
squeeze(string str)
{
    return soul_do_cmd(str, "fondly", "Squeeze whom?", 0,
		       "You squeeze # @."," squeezes you @.", 0,
		       " squeezes # @.", ADVERB | TARGET | DISTANCE, MSG_SEE);
}

nomask int
stare(string str)
{
    return soul_do_cmd(str, "dreamily", "Stare at whom?",
		       "You stare into space.", "You stare @ at #.",
		       " stares @ at you.", " stares into space.",
		       " stares @ at #.", ADVERB | TARGET, MSG_SEE);
}

nomask int
steam(string str)
{
    return soul_do_cmd(str, 0, "Steam what?",
		       "Steam comes boiling out of your ears.", 0, 0,
		       "'s face turns purple and steam starts to " +
		       "boil out of " +
		       this_player()->query_possessive() + " ears.", 0, 0,
		       MSG_SEE);
}

nomask int
stretch(string str)
{
    return soul_do_cmd(str, 0, "Stretch how?",
		       "You @ stretch your tired body out.\n" +
		       "Ahh, that feels good.", 0, 0,
		       " @ stretches " +
		       this_player()->query_possessive() + " tired body.",
		       0, ADVERB, MSG_SEE);
}

nomask int
strut(string str)
{
    return soul_do_cmd(str, "proudly", "Strut how?", "You strut @.", 0, 0,
		       " struts @.", 0, ADVERB, MSG_SEE);
}

nomask int
stumble(string str)
{
    return soul_do_cmd(str,"drunkenly", "Stumble how?", "Look where you go!",
		       0, 0, " stumbles @.", 0, ADVERB, MSG_SEE);
}

nomask int
sulk(string str)
{
    return soul_do_cmd(str, 0, "Sulk how?", "You sulk @.", 0, 0,
		       " @ sulks in the corner.", 0, ADVERB, MSG_SEE);
}

nomask int
sweat(string str)
{
    return soul_do_cmd(str, 0, "Sweat how?", "You start to sweat @.", 0, 0,
		       " starts to sweat @.", 0, ADVERB, MSG_SEE);
}

nomask int
tap(string str)
{
    return soul_do_cmd(str, "impatiently", "Tap how?",
		       "You tap your foot @.", 0, 0,
		       " taps " + this_player()->query_possessive() +
		       " foot @.", 0, ADVERB, MSG_SEE);
}

nomask int
think(string str)
{
    return soul_do_cmd(str, 0, "Think what?",
		       "You think about it but fail.", 0, 0,
		       " looks thoughtful.", 0, 0, MSG_SEE);
}

nomask int
tickle(string str)
{
    return soul_do_cmd(str, 0, "Tickle whom?", 0,
		       "You tickle # @.", " tickles you @.", 0,
		       " tickles # @.", ADVERB | TARGET | DISTANCE | LIVING,
		       MSG_SEE);
}

nomask int
tongue(string str)
{
    string poss;

    poss = this_player()->query_possessive();
    return soul_do_cmd(str, 0, "Stick out your tongue at whom?",
		       "You @ stick out your tongue.",
		       "You @ stick out your tongue at #.",
		       " @ sticks out " + poss + " tongue at you.",
		       " @ sticks out " + poss + " tongue.",
		       " @ sticks out " + poss + " tongue at #.",
		       ADVERB | TARGET, MSG_SEE);
}

nomask int
tremble(string str)
{
    return soul_do_cmd(str, 0, "Tremble how?",
		       "You're @ trembling in your boots.", 0, 0,
		       " trembles and quivers @ like a bowl full of jelly.", 0,
		       ADVERB, MSG_SEE);
}

nomask int
twiddle(string str)
{
    return soul_do_cmd(str, 0, "Twiddle what?", "You @ twiddle your thumbs.",
		       0, 0, " @ twiddles " +
		       this_player()->query_possessive() + " thumbs.", 0,
		       ADVERB, MSG_SEE);
}

nomask int
twinkle(string str)
{
    string poss;

    poss = this_player()->query_possessive();
    return soul_do_cmd(str,"merrily","Twinkle at whom?",
		       "You twinkle your eyes @.",
		       "You twinkle your eyes @ at #.",
		       " twinkles " + poss + " eyes @ at you.",
		       " twinkles " + poss + " eyes @.",
		       " twinkles " + poss + " eyes @ at #.",
		       ADVERB | TARGET, MSG_SEE);
}

nomask int
wave(string str)
{
    if (str == "goodbye" || str == "farewell")
    {
	return soul_do_cmd(str, 0,"Wave what?",
			   "You wave " + str + ".", 0, 0,
			   " waves " + str + ".", 0, 0, MSG_SEE);
    }
    return soul_do_cmd(str,"happily","Wave to whom?",
		       "You wave @.", "You wave @ in #'s direction.",
		       " waves @ in your direction.", " waves @.",
		       " waves @ in #'s direction.",
		       ADVERB | TARGET, MSG_SEE);
}

nomask int
wiggle(string str)
{
    return soul_do_cmd(str, 0, "Wiggle how?", "You @ wiggle your bottom.",
		       0, 0, " @ wiggles " +
		       this_player()->query_possessive() + " bottom.", 0,
		       ADVERB, MSG_SEE);
}

nomask int
wink(string str)
{
    return soul_do_cmd(str, "suggestively", "Wink at whom?",
		       "You wink @.", "You wink @ at #.",
		       " winks @ at you.", " winks @.",
		       " winks @ at #.", ADVERB | TARGET, MSG_SEE);
}

nomask int
worship(string str)
{
    string poss;

    poss = this_player()->query_possessive();
    return soul_do_cmd(str, 0, "Worship whom?", 0,
		       "You fall on your knees in front of #.",
		       " falls on " + poss + " knees praises you.", 0,
		       " falls on " + poss + " knees and praises #.",
		       TARGET, MSG_SEE);
}
