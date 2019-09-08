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
agree(string str)
{
    return soul_do_cmd(str, 0, "Agree how?", "You agree @.",
		       "You agree @ with #.", " agrees @ with you.",
		       " agrees @.", " agrees @ with #.",
		       ADVERB | TARGET | LIVING, MSG_HEAR);
}

nomask int
applaud(string str)
{
    return soul_do_cmd(str, 0, "Applaud how?", "You applaude @.",
		       "You @ applaude to #.", " @ applaudes to you.",
		       " applauds @."," @ applaudes to #.", ADVERB | TARGET,
		       MSG_HEAR);
}

nomask int
bounce(string str)
{
    return soul_do_cmd(str, 0, "Bounce how?",
		       "You @ bounce around. BOING!", 0, 0,
		       " @ bounces around.", 0, ADVERB, MSG_HEAR);
}

nomask int
burp(string str)
{
    return soul_do_cmd(str, "rudely", "Burp how?",
		       "You burp @.  Excuse yourself!", 0, 0,
		       " burps @.", 0, ADVERB, MSG_HEAR);
}

nomask int
cackle(string str)
{
    return soul_do_cmd(str, "gleefully", "Cackle how?", "You cackle @.", 0, 0,
		       " throws " + this_player()->query_possessive()+
		       " head back and cackles @!", 0, ADVERB,
		       MSG_HEAR);
}

nomask int
caw(string str)
{
    return soul_do_cmd(str, 0, "Caw how?", "You caw @ totally out of tune.",
		       0, 0, " @ caws totally out of tune.", 0, ADVERB,
		       MSG_HEAR);
}

nomask int
cheer(string str)
{
    return soul_do_cmd(str, 0, "Cheer how?",
		       "You cheer @.", "You @ cheer up #.", 
		       " @ tries to cheer you up.",
		       " cheers @.", " @ tries to cheer up #.",
		       ADVERB | TARGET | LIVING, MSG_HEAR);
}

nomask int
choke(string str)
{
    return soul_do_cmd(str, 0, "Choke how?",
		       "You choke @. Cough, cough...",
		       "You @ choke #'s neck.", 
		       " @ chokes your neck.", "'s face slowly darkens as "+
		       this_player()->query_pronoun()+ " chokes @.",
		       " @ chokes #'s neck.",
		       ADVERB | TARGET | DISTANCE | LIVING, MSG_HEAR);
}

nomask int
chuckle(string str)
{
    return soul_do_cmd(str, "politely", "Chuckle how?", "You chuckle @.", 0,
		       0, " chuckles @.", 0, ADVERB, MSG_HEAR);
}

nomask int
clap(string str)
{
    return soul_do_cmd(str, "appreciatively", "Clap how?", "You clap @.", 0,
		       0," claps @.", 0, ADVERB, MSG_HEAR);
}

nomask int
comfort(string str)
{
    return soul_do_cmd(str, 0, "Comfort whom?", 0,
		       "You @ comfort #.", " @ comforts you.", 0,
		       " @ comforts #.", ADVERB | TARGET, MSG_HEAR);
}

nomask int
complain(string str)
{
    return soul_do_cmd(str, 0, "Complain to whom?", 0, 
		       "You @ complain about the miserable state "+
		       "of things to #.",
		       " @ complains about the miserable state "+
		       "of things to you.", 0, " @ complains to #.", 
		       ADVERB | TARGET, MSG_HEAR);
}

nomask int
confuse(string str)
{
    return soul_do_cmd(str, 0, "Confuse whom?", 0,
		       "You @ try to confuse #... But fail...",
		       " @ tries in vain to confuse you.", 0,
		       " @ tries in vain to confuse #.", ADVERB | TARGET,
		       MSG_HEAR);
}

nomask int
congratulate(string str)
{
    return soul_do_cmd(str, 0, "Congratulate whom?", 0, 
		       "You @ congratulate #.", " @ congratulates you.", 0,
		       " @ congratulates #.", ADVERB | TARGET, MSG_HEAR);
}

nomask int
cough(string str)
{
    return soul_do_cmd(str, "noisily", "Cough how?",
		       "You cough @. Cover your mouth when you do that!", 0,
		       0, " coughs @.", 0, ADVERB, MSG_HEAR);
}

nomask int
cry(string str)
{
    return soul_do_cmd(str, 0, "Cry how?", "Waaaaah!", 0, 0,
		       " bursts into tears.", 0, 0, MSG_HEAR);
}

nomask int
curse(string str)
{
    return soul_do_cmd(str, 0, "Curse whom?", "You curse @! $&%&%^*&",
		       "You @ curse #.", " @ curses you.",
		       " curses @! $%&%^*&", " @ curses #.",
		       ADVERB | TARGET, MSG_HEAR);
}

nomask int
emotehear(string str)
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
		 ({ QCTNAME(tp), " " + str + "\n" }), MSG_HEAR, tp, 1000);
    return 1;
}

nomask int
excuse(string str)
{
    object tp;

    tp = this_player();
    return soul_do_cmd(str, 0, "Excuse how?", "You @ excuse yourself.",
		       0, 0, " @ excuses "+tp->query_objective()+"self for "+
		       tp->query_possessive()+" rude behaviour.", 0,
		       ADVERB, MSG_HEAR);
}

nomask int
explode_cmd(string str)
{
    return soul_do_cmd(str, 0, "Explode at whom?",
		       "You hold your breath and explode in anger!",
		       "You explode in anger at #!",
		       " explodes with anger at you!",
		       " explodes with anger.", " explodes with anger at #!",
		       TARGET, MSG_HEAR);
}

nomask int
fart(string str)
{
    return soul_do_cmd(str, 0, "Fart how?", "You fart loudly. How rude!", 0, 0,
		       " lets off a real rip-roarer.", 0, 0, MSG_HEAR);
}

nomask int
gasp(string str)
{
    return soul_do_cmd(str, 0, "Gasp how?", "You gasp in astonishment.", 0, 0,
		       " gasps in astonishment!", 0, 0, MSG_HEAR);
}

nomask int
giggle(string str)
{
    return soul_do_cmd(str, "happily", "Giggle how?", "You giggle @.", 0, 0,
		       " giggles @.", 0, ADVERB, MSG_HEAR);
}

nomask int
greet(string str)
{
    return soul_do_cmd(str, 0, "Greet whom?", 0,
		       "You @ greet #.  How friendly!",
		       " @ smiles and greets you!", 0, " @ greets #.",
		       ADVERB | TARGET, MSG_HEAR);
}

nomask int
groan(string str)
{
    return soul_do_cmd(str, "painfully", "Groan how?", "You groan @.", 0, 0,
		       " groans @.", 0, ADVERB, MSG_HEAR);
}

nomask int
growl(string str)
{
    return soul_do_cmd(str, 0, "Growl at whom?", "You growl @.",
		       "You growl @ at #.", " growls @ at you.",
		       " growls @.", " growls @ at #.", ADVERB | TARGET,
		       MSG_HEAR);
}

nomask int
gulp(string str)
{
    return soul_do_cmd(str,"nervously", "Gulp how?", "You gulp @.", 0, 0,
		       " gulps @.", 0, ADVERB, MSG_HEAR);
}

nomask int
hiccup(string str)
{
    return soul_do_cmd(str, 0, "Hiccup how?", "Hic!", 0, 0,
		       " hiccups.", 0, 0, MSG_HEAR);
}

nomask int
hmm(string str)
{
    return soul_do_cmd(str, 0, "Hmm what?", "Hmmmmm.", 0, 0, " goes hmmmm.",
		       0, 0, MSG_HEAR);
}

nomask int
laugh(string str)
{
    return soul_do_cmd(str, 0, "Laugh how?",
		       "You laugh @.", "You laugh @ with #.",
		       " laughs @ with you.", " laughs @.",
		       " laughs @ at #.", ADVERB | TARGET, MSG_HEAR);
}

nomask int
moan(string str)
{
    return soul_do_cmd(str, 0, "Moan how?","You moan @.", 0, 0,
		       " moans @.", 0, ADVERB, 0);
}

nomask int
purr(string str)
{
    return soul_do_cmd(str, "contentedly", "Purr how?", "You purr @.",
		       0, 0, " purrs @.", 0, ADVERB, MSG_HEAR);
}

nomask int
scream(string str)
{
    object tp;

    tp = this_player();
    tp->catch_tell("Arrrrrrrrrgh.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " screams loudly.\n" }), MSG_HEAR, tp, 3900);
    
    return 1;
}

nomask int
sigh(string str)
{
    return soul_do_cmd(str,"deeply","Sigh how?", "You sigh @.",
		       "You sigh @ at #.", " sighs @ at you.",
		       " sighs @.", " sigh @ at #.", ADVERB | TARGET,
		       MSG_HEAR);
}

nomask int
snap(string str)
{
    return soul_do_cmd(str, 0, "Snap how?", "You @ snap your fingers.", 0, 0,
		       " @ snaps " + this_player()->query_possessive() +
		       " fingers.", 0, ADVERB, MSG_HEAR);
}

nomask int
snarl(string str)
{
    return soul_do_cmd(str, 0, "Snarl at whom?", 0, "You @ snarl at #.",
		       " @ snarls at you.", 0, " @ snarls at #.",
		       ADVERB | TARGET, MSG_HEAR);
}

nomask int
sneeze(string str)
{
    return soul_do_cmd(str, 0, "Sneeze how?", "You sneeze @.  *Hatshuu*!",
		       0, 0, " sneezes @.", 0, ADVERB, MSG_HEAR);
}

nomask int
snicker(string str)
{
    return soul_do_cmd(str, 0, "Snicker how?", "You snicker @.", 0, 0,
		       " snickers @.", 0, ADVERB, MSG_HEAR);
}

nomask int
sniff(string str)
{
    return soul_do_cmd(str, 0, "Sniff how?", "You sniff @.",
		       "You sniff @ at #.", " sniffs at you.",
		       " sniffs @.", " sniffs @ at #.",
		       ADVERB | TARGET, MSG_HEAR);
}

nomask int
snore(string str)
{
    return soul_do_cmd(str, "loudly", "Snore how?", "You snore @.", 0, 0,
		       " snores @.", 0, ADVERB, MSG_HEAR);
}

nomask int
snuggle(string str)
{
    return soul_do_cmd(str, 0, "Snuggle up to whom?", 0, 
		       "You @ snuggle up to #."," @ snuggles up to you.", 0,
		       " @ snuggles up to #.",
		       ADVERB | TARGET | DISTANCE, MSG_HEAR);
}

nomask int
sob(string str)
{
    return soul_do_cmd(str, 0, "Sob how?", "You sob @.", "You sob @ at #.",
		       " sobs @ at you.", " sobs @.", " sobs @ at #.",
		       ADVERB | TARGET, MSG_HEAR);
}

nomask int
spit(string str)
{
    return soul_do_cmd(str, 0, "Spit at whom?", "You @ spit on the ground.",
		       "You @ spit on #.", " @ spits on you!",
		       " @ spits on the ground.", " @ spits on #.",
		       ADVERB | TARGET | DISTANCE, MSG_HEAR);
}

nomask int
stomp(string str)
{
    return soul_do_cmd(str,"angrily","Stomp how?",
		       "You stomp your feet @ on the ground.", 0, 0,
		       " stomps " + this_player()->query_possessive() +
		       " feet @ on the ground.", 0, ADVERB, MSG_HEAR);
}

nomask int
thank(string str)
{
    return soul_do_cmd(str,0,"Thank whom?", 0, "You @ thank #.",
		       " @ thanks you.", 0, " @ thanks #.", ADVERB | TARGET,
		       MSG_HEAR);
}

nomask int
whine(string str)
{
    return soul_do_cmd(str,"petulantly","Whine at whom?",
		       "You whine @.", "You whine @ to #.",
		       " whines @ to you.", " whines @.",
		       " whines @ to #.", ADVERB | TARGET, MSG_HEAR);
}

nomask int
whistle(string str)
{
    return soul_do_cmd(str,"appreciatively","Whistle at whom?",
		       "You whistle @.", "You whistle @ at #.",
		       " whistles @ at you."," whistles @.",
		       " whistles @ at #.", ADVERB | TARGET, MSG_HEAR);
}

nomask int
yawn(string str)
{
    return soul_do_cmd(str,0,"Yawn how?","My, what big teeth you have!",
		       0, 0, " yawns @.", 0, ADVERB, MSG_HEAR);
}

nomask int
yodel(string str)
{
    return soul_do_cmd(str,"happily","Yodel how?",
		       "You yodel @.   Yodelii yodeluu!", 0, 0,
		       " @ yodels a merry tune.", 0, ADVERB, MSG_HEAR);
}
