/*
 * emotion_main.c
 */

inherit "/system/parse";

#include <access.h>
#include <composite.h>
#include <event.h>
#include <kernel.h>
#include <language.h>
#include <macros.h>
#include <money.h>
#include <properties.h>
#include <state.h>

#define ADVERB   1
#define TARGET   2
#define DISTANCE 4
#define LIVING   8 /* parse only livings */

static mapping adverbs, cmdlist, cmdcheck;

/*
 * Function name: create
 * Description:   initialize some variables
 */
static void
create()
{
    adverbs =
	([
	  'a':
	  ({ "absentmindedly","admiringly","affectionately","aggressively",
	     "aimlessly","amazedly",
	     "amusedly","angrily","annoyedly","anxiously","apologetically",
	     "appreciatively","astonishedly"}),
	  'b':
	  ({ "badly","bashfully","beatifically","bitterly","bombastically",
	     "boredly","boyishly","bouncingly","briefly","brightly",
	     "broadly","brotherly","busily" }),
	  'c':
	  ({ "carefully","calmly","charmingly","childishly","cluelessly",
	     "coldly","completely","confidently","confusedly","contemptuously",
	     "contentedly","crazily","cunningly","courageously","cutely",
	     "curiously","cynically" }),
	  'd':
	  ({ "dangerously","deeply","demonically","depressedly",
	     "desperately","devilishly","dirtily","disappointedly",
	     "disgustedly","dreamily","doubtfully","drunkenly","dumbly" }),
	  'e':
	  ({ "eagerly","ecstatically","egocentrically","egoistically",
	     "endearingly","enigmatically","enthusiastically","erotically",
	     "evilly","excitedly","exhaustedly", }),
	  'f':
	  ({ "fanatically","fatalistically","fatherly","fiercely",
	     "firmly","flirtatiously","fondly","foolishly","friendly",
	     "funny","furtively" }),
	  'g':
	  ({ "gently","gigglishly","girlishly","gleefully","gracefully",
	     "graciously","greedily" }),
	  'h':
	  ({ "handsomely","happily","harmonically","haughtily","heartbrokenly",
	     "heavily","helpfully","helplessly","honestly","hopefully",
	     "hornily","humbly", "hungrily","hurriedly","hysterically" }),
	  'i':
	  ({ "ignorantly","impatiently","inanely","innocently",
	     "inquiringly","insanely","instantly","intently",
	     "interestedly","inwardly","irately","irritatedly",
	     "ironically" }),
	  'j':
	  ({ "jealously","joyfully" }),
	  'k':
	  ({ "kindly","knowingly" }),
	  'l':
	  ({ "lazily","longingly","loudly","lovingly", "lustfully" }),
	  'm':
	  ({ "madly","magically","maniacally","melancholicly",
	     "melodiously","merrily","mischievously","motherly",
	     "musically","mysteriously" }),
	  'n':
	  ({ "nastily","naughtily","nervously","nicely","noisily",
	     "nonchalantly" }),
	  'o':
	  ({ "obviously" }),
	  'p':
	  ({ "painfully","passionately","patiently","patronizingly",
	     "peacefully", "perfectly","personally","petulantly",
	     "philosphically",
	     "physically","piously","playfully","politely","proudly",
	     "professionally","profoundly","profusely","purposefully" }),
	  'q':
	  ({ "questioningly","quickly","quietly" }),
	  'r':
	  ({ "randomly","really","rebelliously","relievedly","reluctantly",
	     "roguishly","rudely" }),
	  's':
	  ({ "sadly","sadistically","sarcastically","satanically",
	     "scornfully","searchingly","secretly","seductively",
	     "sensually","sensuously","seriously",
	     "sexily","shamelessly","sheepishly","shyly","sickly",
	     "silently","sisterly","skillfully","sleepily","slightly",
	     "slowly","slyly","smilingly","smoochily","softly","solemnly",
	     "sorrowfully","speechlessly","strangely","stupidly",
	     "suggestively","suddenly","sweetly" }),
	  't':
	  ({ "tearfully","teasingly","tenderly","terribly","thankfully",
	     "theoretically","thoughtfully","tightly","tiredly",
	     "tragically","truly","trustfully","trustingly" }),
	  'u':
	  ({ "understandingly","unexpectedly","unhappily","unknowingly" }),
	  'v':
	  ({ "viciously","vigorously","virtually","vivaciously" }),
	  'w':
	  ({ "warmly","weakly","wearily","wholeheartedly","wickedly",
	     "wiggily","wildly","wisely","wistfully","worriedly",
	     "wryly" }),
	  'x':
	  ({ }),
	  'y':
	  ({ }),
	  'z':
	  ({ }),
	  ]);
    
    cmdlist = ([
	"ack":"ack",
	"agree":"agree",
	"applaud":"applaud",

	"beckon":"beckon",
	"beg":"beg",
	"blush":"blush",
	"bounce":"bounce",
	"bop":"bop",
	"bow":"bow",
	"brighten":"brighten",
	"burp":"burp",

	"cackle":"cackle",
	"caress":"caress",
	"caw":"caw",
	"cheer":"cheer",
	"choke":"choke",
	"chuckle":"chuckle",
	"clap":"clap",
	"comfort":"comfort",
	"complain":"complain",
	"confuse":"confuse",
	"confused":"confused",
	"congratulate":"congratulate",
	"cough":"cough",
	"cringe":"cringe",
	"cry":"cry",
	"cuddle":"cuddle",
	"curse":"curse",
	"curtsey":"curtsey",
	     
	"dance":"dance",
	"despair":"despair",
	"dream":"dream",
	"duh":"duh",

	"eeks":"eeks",
	"eh":"emotehear",
	"emotehear":"emotehear",
	"emotesee":"emotesee",
	"es":"emotesee",
	"explode":"explode_cmd",
	"excuse":"excuse",
	"eyebrow":"eyebrow",
	     
	"fart":"fart",
	"fawn":"fawn",
	"flip":"flip",
	"fondle":"fondle",
	"french":"french",
	"frown":"frown",
	"fume":"fume",

	"gasp":"gasp",
	"giggle":"giggle",
	"glare":"glare",
	"greet":"greet",
	"grin":"grin",
	"groan":"groan",
	"grope":"grope",
	"grovel":"grovel",
	"growl":"growl",
	"gulp":"gulp",

	"hiccup":"hiccup",
	"hmm":"hmm",
	"hop":"hop",
	"hug":"hug",

	"jump":"jump",

	"kick":"kick",
	"kiss":"kiss",

	"laugh":"laugh",
	"lick":"lick",
	"listen":"listen",
	"love":"love",

	"moan":"moan",

	"nibble":"nibble",
	"nod":"nod",
	"nudge":"nudge",
	"nuzzle":"nuzzle",

	"offer":"offer",
	"oops":"oops",

	"panic":"panic",
	"pat":"pat",
	"peer":"peer",
	"pinch":"pinch",
	"point":"point",
	"poke":"poke",
	"ponder":"ponder",
	"pout":"pout",
	"puke":"puke",
	"purr":"purr",

	"rolleyes":"rolleyes",
	"ruffle":"ruffle",

	"scratch":"scratch",
	"scream":"scream",
	"shake":"shake",
	"shiver":"shiver",
	"shrug":"shrug",
	"shudder":"shudder",
	"sigh":"sigh",
	"slap":"slap",
	"smile":"smile",
	"smirk":"smirk",
	"snap":"snap",
	"snarl":"snarl",
	"sneeze":"sneeze",
	"snicker":"snicker",
	"sniff":"sniff",
	"snore":"snore",
	"snuggle":"snuggle",
	"sob":"sob",
	"spit":"spit",
	"squeeze":"squeeze",
	"stare":"stare",
	"steam":"steam",
	"stomp":"stomp",
	"stretch":"stretch",
	"strut":"strut",
	"stumble":"stumble",
	"sulk":"sulk",
	"sweat":"sweat",

	"tap":"tap",
	"thank":"thank",
	"think":"think",
	"tickle":"tickle",
	"tongue":"tongue",
	"tremble":"tremble",
	"twiddle":"twiddle",
	"twinkle":"twinkle",

	"wave":"wave",
	"whine":"whine",
	"whistle":"whistle",
	"wiggle":"wiggle",
	"wink":"wink",
	"worship":"worship",

	"yawn":"yawn",
	"yodel":"yodel",
	]);

    cmdcheck = ([
	"ack":(WORKING | COMBAT | CONCENTRATE | WALKING | SEARCHING | RESTING |
	       ON_GROUND | MOUNT),
	"agree":(WORKING | COMBAT | CONCENTRATE | WALKING | SEARCHING |
		 RESTING | ON_GROUND | GHOST | MOUNT),
	"applaud":(COMBAT | WALKING | SEARCHING | RESTING |
		   ON_GROUND | MOUNT),
		 
	"beckon":(COMBAT | WALKING | SEARCHING | RESTING | ON_GROUND |
		  GHOST | MOUNT),
	"beg":(COMBAT | GHOST | RESTING | ON_GROUND | MOUNT),
	"blush":(WORKING | SLEEPING | PARALYSED | COMBAT | CONCENTRATE |
		 WALKING | SEARCHING | RESTING | ON_GROUND | HIDING | MOUNT),
	"bounce":(COMBAT | WALKING),
	"bop":(WALKING | MOUNT),
	"bow":(GHOST),
	"brighten":(WORKING | SLEEPING | PARALYSED | COMBAT | CONCENTRATE |
		    WALKING  | SEARCHING | RESTING | ON_GROUND |
		    HIDING | MOUNT),
	"burp":(WORKING | SLEEPING | PARALYSED | COMBAT | CONCENTRATE |
		WALKING | SEARCHING | RESTING | ON_GROUND | MOUNT),

	"cackle":(COMBAT | WALKING | SEARCHING | RESTING | ON_GROUND |
		  WORKING | GHOST | MOUNT),
	"caress":(RESTING | ON_GROUND | MOUNT),
	"caw":(WORKING | PARALYSED | COMBAT | CONCENTRATE | WALKING |
	       SEARCHING | RESTING | ON_GROUND | GHOST | MOUNT),
	"cheer":(WORKING | COMBAT | RESTING | ON_GROUND | MOUNT | WALKING |
		 GHOST),
	"choke":(RESTING | ON_GROUND | MOUNT),
	"chuckle":(WORKING | COMBAT | CONCENTRATE | WALKING | SEARCHING |
		   RESTING | ON_GROUND | GHOST | MOUNT),
	"clap":(WALKING | RESTING | ON_GROUND | MOUNT),
	"comfort":(WORKING | COMBAT | WALKING | RESTING | ON_GROUND |
		   GHOST | MOUNT),
	"complain":(WORKING | COMBAT | WALKING | SEARCHING | RESTING |
		    ON_GROUND | GHOST | MOUNT),
	"confuse":(COMBAT | WALKING | RESTING | ON_GROUND | GHOST | MOUNT),
	"confused":(WORKING | PARALYSED | COMBAT | WALKING | SEARCHING |
		    RESTING | ON_GROUND | MOUNT),
	"congratulate":(WORKING | WALKING | RESTING | ON_GROUND | GHOST |
			MOUNT),
	"cough":(WORKING | SLEEPING | COMBAT | WALKING | SEARCHING | RESTING |
		 ON_GROUND | GHOST | MOUNT),
	"cringe":(SLEEPING | COMBAT | RESTING | ON_GROUND | MOUNT),
	"cry":(WORKING | COMBAT | WALKING | SEARCHING | RESTING | ON_GROUND |
	       GHOST | MOUNT),
	"cuddle":(WALKING | RESTING | ON_GROUND | MOUNT),
	"curse":(WORKING | COMBAT | WALKING | SEARCHING | RESTING | ON_GROUND |
		 GHOST | MOUNT),
	"curtsey":(GHOST),
	     
	"dance":(WALKING),
	"despair":(COMBAT | WALKING | SEARCHING | RESTING |
		   ON_GROUND | MOUNT),
	"dream":(WORKING | SLEEPING | PARALYSED | UNCONSCIOUS | RESTING |
		 ON_GROUND | MOUNT),
	"duh":(WORKING | WALKING | SEARCHING | RESTING | ON_GROUND | MOUNT),
		 
	"eeks":(WORKING | COMBAT | WALKING | SEARCHING | RESTING |
		ON_GROUND | MOUNT),
	"eh":(WORKING | COMBAT | WALKING | SEARCHING | RESTING | ON_GROUND |
	      MOUNT),
	"emotehear":(WORKING | COMBAT | WALKING | SEARCHING | RESTING |
		     ON_GROUND | MOUNT),
	"emotesee":(WORKING | COMBAT | WALKING | SEARCHING | RESTING |
		    ON_GROUND | MOUNT),
	"es":(WORKING | COMBAT | WALKING | SEARCHING | RESTING | ON_GROUND |
	      MOUNT),
	"explode":(COMBAT | WALKING | SEARCHING | RESTING | ON_GROUND | MOUNT),
	"eyebrow":(WORKING | COMBAT | CONCENTRATE | WALKING | SEARCHING |
		   RESTING | ON_GROUND | HIDING | MOUNT),
	"excuse":(WORKING | COMBAT | CONCENTRATE | WALKING | SEARCHING |
		  RESTING | ON_GROUND | GHOST | MOUNT),
	     
	"fart":(WORKING | SLEEPING | PARALYSED | COMBAT | CONCENTRATE |
		WALKING | SEARCHING | RESTING | ON_GROUND | MOUNT),
	"fawn":0,
	"flip":0,
	"fondle":(RESTING | ON_GROUND | MOUNT),
	"french":(RESTING | ON_GROUND | MOUNT),
	"frown":(WORKING | COMBAT | CONCENTRATE | WALKING | SEARCHING |
		 RESTING | ON_GROUND | HIDING | MOUNT),
	"fume":(WORKING | COMBAT | WALKING | SEARCHING | RESTING | ON_GROUND |
		HIDING | MOUNT), 

	"gasp":(WORKING | COMBAT | WALKING | SEARCHING | RESTING | ON_GROUND |
		HIDING | GHOST | MOUNT),
	"giggle":(WORKING | COMBAT | WALKING | SEARCHING | RESTING |
		  ON_GROUND | GHOST | MOUNT),
	"glare":(WORKING | PARALYSED | COMBAT | CONCENTRATE | WALKING |
		 SEARCHING | RESTING | ON_GROUND | HIDING | GHOST | MOUNT),
	"greet":(WORKING | WALKING | RESTING | ON_GROUND | GHOST | MOUNT),
	"grin":(WORKING | SLEEPING | COMBAT | CONCENTRATE | WALKING |
		SEARCHING | RESTING | ON_GROUND | HIDING | GHOST | MOUNT),
	"groan":(WORKING | SLEEPING | UNCONSCIOUS | COMBAT | CONCENTRATE |
		 WALKING | SEARCHING | RESTING | ON_GROUND | GHOST | MOUNT),
	"grope":(RESTING | ON_GROUND | GHOST | MOUNT),
	"grovel":0,
	"growl":(SLEEPING | COMBAT | CONCENTRATE | WALKING |
		 SEARCHING | RESTING | ON_GROUND | GHOST | MOUNT),
	"gulp":(WORKING | COMBAT | CONCENTRATE | WALKING | SEARCHING |
		RESTING | ON_GROUND | HIDING | MOUNT),

	"hiccup":(WORKING | WALKING | SEARCHING | RESTING | ON_GROUND | MOUNT),
	"hmm":(WORKING | COMBAT | CONCENTRATE | WALKING | SEARCHING | RESTING |
	       ON_GROUND | GHOST | MOUNT),
	"hop":(WALKING),
	"hug":(RESTING | ON_GROUND | MOUNT),

	"jump":(COMBAT | WALKING),

	"kick":(COMBAT | MOUNT),
	"kiss":(RESTING | ON_GROUND | MOUNT),

	"laugh":(WORKING | COMBAT | WALKING | SEARCHING | RESTING | ON_GROUND |
		 GHOST | MOUNT),
	"lick":(RESTING | ON_GROUND),
	"listen":(WORKING | COMBAT | WALKING | SEARCHING | RESTING |
		  ON_GROUND | GHOST | MOUNT),
	"love":(RESTING | ON_GROUND | MOUNT),

	"moan":(WORKING | COMBAT | CONCENTRATE | WALKING | SEARCHING |
		RESTING | ON_GROUND | GHOST | MOUNT),

	"nibble":(RESTING | ON_GROUND),
	"nod":(WORKING | COMBAT | CONCENTRATE | WALKING | SEARCHING | RESTING |
	       ON_GROUND | GHOST | MOUNT),
	"nudge":(RESTING | ON_GROUND | MOUNT),
	"nuzzle":(RESTING | ON_GROUND),

	"offer":(RESTING | ON_GROUND | MOUNT),
	"oops":(WORKING | COMBAT | CONCENTRATE | WALKING | SEARCHING |
		RESTING | ON_GROUND | GHOST | MOUNT),

	"panic":(COMBAT | WALKING | RESTING | ON_GROUND | GHOST | MOUNT),
	"pat":(RESTING | ON_GROUND | MOUNT),
	"peer":(WORKING | PARALYSED | COMBAT | CONCENTRATE | WALKING |
		SEARCHING | RESTING | ON_GROUND | HIDING | GHOST | MOUNT),
	"pinch":(RESTING | ON_GROUND | MOUNT),
	"point":(WORKING | COMBAT | CONCENTRATE | WALKING | RESTING |
		 ON_GROUND | HIDING | GHOST | MOUNT),
	"poke":(RESTING | ON_GROUND | MOUNT),
	"ponder":(WORKING | PARALYSED | COMBAT | CONCENTRATE | WALKING |
		  SEARCHING | RESTING | ON_GROUND | HIDING | GHOST | MOUNT),
	"pout":(WORKING | COMBAT | CONCENTRATE | WALKING | SEARCHING |
		RESTING | ON_GROUND | HIDING | MOUNT),
	"puke":(RESTING | ON_GROUND | MOUNT),
	"purr":(WORKING | SLEEPING | COMBAT | CONCENTRATE | WALKING |
		SEARCHING | RESTING | ON_GROUND | MOUNT),

	"rolleyes":(WORKING | COMBAT | WALKING | SEARCHING | RESTING |
		    ON_GROUND | HIDING | GHOST | MOUNT),
	"ruffle":(CONCENTRATE | RESTING | ON_GROUND | MOUNT),

	"scratch":(WORKING | CONCENTRATE | WALKING | SEARCHING | RESTING |
		   ON_GROUND | HIDING | MOUNT),
	"scream":(WORKING | COMBAT | WALKING | RESTING | ON_GROUND | GHOST |
		  MOUNT),
	"shake":(CONCENTRATE | RESTING | ON_GROUND | MOUNT),
	"shiver":(WORKING | SLEEPING | COMBAT | CONCENTRATE | WALKING |
		  SEARCHING | RESTING | ON_GROUND | HIDING | GHOST | MOUNT),
	"shrug":(WORKING | COMBAT | CONCENTRATE | WALKING | SEARCHING |
		 RESTING | ON_GROUND | HIDING | GHOST | MOUNT),
	"shudder":(WORKING | COMBAT | CONCENTRATE | WALKING | SEARCHING |
		   RESTING | ON_GROUND | HIDING | GHOST | MOUNT),
	"sigh":(COMBAT | CONCENTRATE | WALKING | SEARCHING | RESTING |
		WORKING | ON_GROUND | GHOST | MOUNT),
	"slap":(RESTING | ON_GROUND | MOUNT),
	"smile":(WORKING | SLEEPING | COMBAT | CONCENTRATE | WALKING |
		 SEARCHING | RESTING | ON_GROUND | HIDING | GHOST | MOUNT),
	"smirk":(WORKING | COMBAT | CONCENTRATE | WALKING | SEARCHING |
		 RESTING | ON_GROUND | HIDING | GHOST | MOUNT),
	"snap":(WORKING | COMBAT | CONCENTRATE | WALKING | SEARCHING |
		RESTING | ON_GROUND | MOUNT),
	"snarl":(WORKING | COMBAT | CONCENTRATE | RESTING | ON_GROUND | MOUNT),
	"sneeze":(WORKING | COMBAT | WALKING | SEARCHING | RESTING |
		  ON_GROUND | MOUNT),
	"snicker":(WORKING | COMBAT | CONCENTRATE | WALKING | SEARCHING |
		   RESTING | ON_GROUND | GHOST | MOUNT),
	"sniff":(WORKING | COMBAT | CONCENTRATE | WALKING | SEARCHING |
		 RESTING | ON_GROUND | GHOST | MOUNT),
	"snore":(WORKING | SLEEPING | ON_GROUND | RESTING | MOUNT),
	"snuggle":(RESTING | ON_GROUND),
	"sob":(WORKING | COMBAT | CONCENTRATE | WALKING | SEARCHING | RESTING |
	       ON_GROUND | GHOST | MOUNT),
	"spit":(WORKING | WALKING | RESTING | ON_GROUND | MOUNT),
	"squeeze":(RESTING | ON_GROUND),
	"stare":(WORKING | PARALYSED | COMBAT | CONCENTRATE | WALKING |
		 SEARCHING | RESTING | ON_GROUND | HIDING | GHOST | MOUNT),
	"steam":(WORKING | COMBAT | WALKING | SEARCHING | RESTING |
		 ON_GROUND | MOUNT),
	"stomp":(WORKING | COMBAT | WALKING | SEARCHING | RESTING | ON_GROUND),
	"stretch":(WORKING | RESTING | ON_GROUND | MOUNT),
	"strut":(WALKING | MOUNT),
	"stumble":(COMBAT | WALKING),
	"sulk":(COMBAT | CONCENTRATE | WALKING | SEARCHING | RESTING |
		ON_GROUND | HIDING | MOUNT),
	"sweat":(WORKING | SLEEPING | PARALYSED | COMBAT | CONCENTRATE |
		 WALKING | SEARCHING | RESTING | ON_GROUND | HIDING | MOUNT),

	"tap":(WORKING | CONCENTRATE | RESTING | ON_GROUND),
	"thank":(WORKING | CONCENTRATE | WALKING | RESTING | ON_GROUND |
		 GHOST | MOUNT),
	"think":(WORKING | COMBAT | WALKING | SEARCHING | RESTING | ON_GROUND |
		 HIDING | MOUNT),
	"tickle":(RESTING | ON_GROUND | MOUNT),
	"tongue":(WORKING | COMBAT | CONCENTRATE | WALKING | SEARCHING |
		  RESTING | ON_GROUND | HIDING | MOUNT),
	"tremble":(COMBAT | CONCENTRATE | WALKING | SEARCHING |
		   RESTING | ON_GROUND | GHOST),
	"twiddle":(CONCENTRATE | WALKING | RESTING | ON_GROUND |
		   HIDING | GHOST | MOUNT),
	"twinkle":(WORKING | COMBAT | CONCENTRATE | WALKING | RESTING |
		   ON_GROUND | HIDING | MOUNT),

	"wave":(WORKING | WALKING | RESTING | ON_GROUND | GHOST | MOUNT),
	"whine":(WORKING | COMBAT | WALKING | SEARCHING | RESTING | ON_GROUND |
		 GHOST | MOUNT),
	"whistle":(WORKING | COMBAT | WALKING | SEARCHING | RESTING |
		   ON_GROUND | MOUNT),
	"wiggle":(RESTING | ON_GROUND),
	"wink":(COMBAT | CONCENTRATE | WALKING | RESTING | ON_GROUND |
		HIDING | GHOST | MOUNT),
	"worship":0,

	"yawn":(WORKING | COMBAT | WALKING | SEARCHING | SLEEPING | RESTING |
		ON_GROUND | MOUNT),
	"yodel":(WORKING | COMBAT | WALKING | RESTING | ON_GROUND | MOUNT),
	]);
}

/*
 * Function name: get_soul_id
 * Description:   gives a name for this soul
 */
string
get_soul_id()
{
    return "emotion";
}

/*
 * Function name: query_cmdlist
 * Description:   gives a command mapping
 * Returns:       the cmds in a mapping
 */
mapping
query_cmdlist()
{
    return cmdlist;
}

/* 
 * Function name: do_command
 * Description:   Perform the given command, if present.
 * Returns:	  1/0 depending on success.
 */
nomask int
do_command(string cmd, string arg, int state)
{
    string func;
    object tp;
    
    if (!(func = cmdlist[cmd]))
	return 0;
    
    tp = this_player();
#include "../check_state.c"
    
    return call_other(this_object(), func, arg);
}

/*
 * Function name: member_adverb
 * Description:   Returns the expanded adverb string
 * Arguments:     pattern - an abbrevation of an adverb
 * Returns:       the full expanded adverb, 0 else
 */
static string
member_adverb(string pattern)
{
    int    fchar, i, sz;
    string *list;

    if (!pattern || strlen(pattern) < 3)
        return 0;

    fchar = pattern[0];
    list  = adverbs[fchar];

    for (i=0, sz = sizeof(list); i < sz; i++)
	if (pattern == extract(list[i], 0, strlen(pattern)-1))
	    return list[i];
    return 0;
}

/*
 * Function name: soul_check_cmd
 * Description:   Feelings that can be heard by others
 * Arguments:     arg - the feeling argument used by the player
 *                adverb - default adverb
 *                mode   - indicates type (with/out adverb, target, etc.)
 *                tp - the current player
 * Returns:       array
 */
static mixed
soul_check_cmd(string arg, string adverb, int mode, object tp)
{
    int    sz, d;
    string *parts;
    object *list, *tar_list;
    
    if (stringp(arg) && mode)
    {
        if (mode & TARGET)
	{
	    if (arg == "all")
		/* e.g. 'wave all' should only work for livings */
		mode |= LIVING;
	    if (mode & DISTANCE)
		d = tp->query_step();
	    list = I(E(tp), (mode & LIVING ? 2 : 0)) -
		   ({ tp });
	    list = tp->visible(list);
	    if (!sizeof(tar_list = parse(arg, list, tp, d)))
	    {
		parts = explode(arg, " ");
		if ((sz = sizeof(parts)) > 1)
		{
		    adverb = parts[sz-1];
		    arg    = arg[..strlen(arg) - strlen(adverb) - 2];
		    if (!sizeof(tar_list = parse(arg, list, tp, d)))
			return 0;
		}
		else
		    adverb = arg;
	    }
	}
	else
	{
	    adverb = arg;
	    tar_list = ({ });
	}
    }
    else
	tar_list = ({ });

    if (adverb && (!(mode & ADVERB) || !(adverb = member_adverb(adverb))))
        return 0;

    return ({ tar_list, adverb });
}

/*
 * Function name: soul_do_cmd
 * Description:   Feelings that can be heard by others
 * Arguments:     arg           - the command argument used by the player
 *                adverb        - default adverb
 *                fail_msg      - printout for wrong usage
 *                actor_msg     -  message to the emoting player, 0 forces
 *                                target check
 *                act_tar_msg   - message to the emoting player (with target)
 *                target_msg    - message if emotion has target
 *                other_msg     - message for others around
 *                other_tar_msg - message for others around (with target)
 *                mode          - indicates feeling type
 *                                (with/out adverb, target, etc.)
 *                msgmode       - msg mode (MSG_HEAR, MSG_SEE ...)
 * Returns:       1 if success, -1 else
 */
static int
soul_do_cmd(string arg, string adverb, string fail_msg, mixed actor_msg,
	    string act_tar_msg, string target_msg, mixed other_msg,
	    string other_tar_msg, int mode, int msgmode)
{
    int    i, sz, *co;
    string s1, s2;
    object tp, here, *tlist, *other;
    mixed  list;

    tp = this_player();
    if (sizeof(list = soul_check_cmd(arg, adverb, mode, tp)) < 2 ||
        (!actor_msg && !sizeof(list[0])))
    {
        /* fail if false return, or target needed but not given */
        tp->catch_tell(fail_msg + "\n");
        return -1;
    }

    here   = E(tp);
    tlist  = list[0];
    adverb = list[1];

    other = I(here, 2) - tlist - ({ tp });
    
    if (!adverb)
	adverb = "";
    else
	adverb = " " + adverb;
    
    sz = sizeof(tlist);
    if (!actor_msg || sz)
    {
	actor_msg = act_tar_msg;
	other_msg = other_tar_msg;
    }
    if (mode & ADVERB)
    {
	if (sscanf(actor_msg, "%s @%s", s1, s2))
	    actor_msg = s1 + adverb + s2;
	if (sscanf(other_msg, "%s @%s", s1, s2))
	    other_msg = s1 + adverb + s2;
    }
    if (mode & TARGET)
    {
        if (sscanf(actor_msg, "%s#%s", s1, s2))
            actor_msg = ({ s1 }) + COMPOSITE_ARRAY(tlist) + ({ s2 });
	else
	    actor_msg = ({ actor_msg });
	
        if (sscanf(other_msg, "%s#%s", s1, s2))
            other_msg = ({ s1 }) + COMPOSITE_ARRAY(tlist) + ({ s2 });
	else
	    other_msg = ({ other_msg });
	
        if ((mode & ADVERB) && sscanf(target_msg, "%s @%s", s1, s2))
            target_msg = s1 + adverb + s2;
	tp->catch_msg(actor_msg + ({ "\n" }), 0, 0);
    }
    else
    {
	tp->catch_msg(actor_msg + "\n", 0, 0);
	other_msg = ({ other_msg });
    }

    list = QCTNAME(tp);
    
    actor_msg = ({ list, target_msg + "\n" });
    for (i=sz; i--;)
	tlist[i]->catch_msg(actor_msg, tp, msgmode, 480); /* 40 feet */
    
    other_msg = ({ list }) + other_msg + ({ "\n" });
    for (i=sizeof(other); i--;)
	other[i]->catch_msg(other_msg, tp, msgmode, 480); /* 40 feet */
    
    return 1;
}

#include "emotion_hear.c"
#include "emotion_seen.c"
