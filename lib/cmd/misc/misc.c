/*
 * misc_main.c
 */

inherit "/system/parse";
inherit "/base/misc/drop";
inherit "/base/misc/get";
inherit "/base/misc/give";

#include <access.h>
#include <clone.h>
#include <composite.h>
#include <combat.h>
#include <description.h>
#include <event.h>
#include <effects.h>
#include <ingredients.h>
#include <language.h>
#include <liquid.h>
#include <living.h>
#include <macros.h>
#include <magic.h>
#include <material.h>
#include <move.h>
#include <mudtime.h>
#include <properties.h>
#include <race.h>
#include <recipe.h>
#include <skills.h>
#include <slots.h>
#include <state.h>

#define HELPPATH "/doc/help/"

nomask int get(string what, object tp);
nomask varargs int wake(string str, object tp, int f);
nomask void mkhelpindex();

private string  *health, *mana, *fatigue;
private mapping cmdlist, cmdcheck, langletters, helpindex;


/*
 * Function name: create
 * Description:   initialize some variables
 */
static void
create()
{
    health  = ({ "at deaths door", "barely alive",
		 "terribly hurt", "in a very bad shape", "in a bad shape",
		 "hurt", "somewhat hurt", "slightly hurt",
		 "feeling very well" });
    
    mana    = ({ "in a vegetable state", "exhausted",
		 "worn down", "indisposed", "in a bad shape",
		 "degraded", "somewhat degraded", "slightly degraded",
		 "in full vigour" });
    fatigue = ({ "totally exhausted", "somewhat exhausted", "very tired",
		 "tired", "somewhat tired", "very weary", "weary",
		 "somewhat weary", "alert", "very alert", "extremely alert" });
    langletters = ([
		    SK_QUENYA       :({ 'a', 'e', 'o' }),
		    SK_SINDARIN     :({ 'a', 'e', 'h', 'o' }),
		    SK_KHUZDUL      :({ 'h', 'k', 'z' }),
		    SK_BLACK_SPEECH :({ 'g', 'k', 'l', 'u', 'z' }),
		    /* westron is used as default */
		    SK_WESTRON      :({ 'a', 'k', 'o', 'r', 'u' }),
		    ]);
    
    cmdlist = ([
	"?":"help",
	
	"accept":"accept",
	"aid":"aid",
	"alias":"alias",
	"analyse":"analyse",
	"answer":"answer",
	"appraise":"appraise",
	"assist":"assist",
	"attach":"attach",
	"attack":"attack",
	     
	"backstab":"backstab",
	"bug":"bug",
	"butcher":"butcher",
	"buy":"buy",

	"cancel":"cancel",
	"carve":"carve",
	"cast":"cast",
	"cook":"cook",
	"channel":"channel",
	"close":"close",
	"command":"commanding",
	"count":"count",

	"d":"down",
	"detach":"detach",
	"disarm":"disarm",
	"dismount":"dismount",
	"down":"down",
	"drink":"drink",
	"drop":"drop",
	     
	"e":"east",
	"east":"east",
	"eat":"eat",
	"enter":"enter",
	"exa":"examine",
	"examine":"examine",
	"ext":"extinguish",
	"extinguish":"extinguish",

	"fill":"fill",
	"follow":"follow",
	"forget":"forget",
	"from":"from",
	     
	"get":"get",
	"give":"give",
	"grumble":"grumble",
	     
	"help":"help",
	"hide":"hide",
	"hp":"hp",
	     
	"i":"show_inventory",
	"idea":"idea",
	"inventory":"show_inventory",

	"knot":"knot",
	
	"l":"look",
	"language":"language",
	"learn":"learn",
	"learncost":"learncost",
	"light":"light",
	"lock":"lock",
	"talk":"talk",
	"look":"look",

	"mail":"mail",
	"meditate":"meditate",
	"mood":"mood",
	"mount":"mount",
	"mumble":"mumble",

	"n":"north",
	"ne":"northeast",
	"news":"news",
	"north":"north",
	"northeast":"northeast",
	"northwest":"northwest",
	"nw":"northwest",
	
	"open":"open",
	
	"parry":"parry",
	"pick":"pick",
	"pour":"pour",
	"pray":"pray",
	"prepare":"prepare",
	"prompt":"prompt",
	"pull":"pull",
	"push":"push",
	"put":"put",
	     
	"read":"read",
	"remember":"remember",
	"remove":"remove",
	"repair":"repair",
	"rest":"rest",
	"roll":"roll",
	"run":"run",

	"s":"south",
	"save":"save",
	"say":"say",
	"se":"southeast",
	"search":"search",
	"sell":"sell",
	"shoot":"shoot",
	"shout":"shout",
	"shriek":"shriek",
	"sing":"sing",
	"sit":"sit",
	"skills":"skills",
	"skin":"skin",
	"sla":"sla",
	"sleep":"sleep",
	"smash":"smash",
	"sneak":"sneak",
	"south":"south",
	"southeast":"southeast",
	"southwest":"southwest",
	"spells":"spells",
	"stand":"stand",
	"stats":"stats",
	"steal":"steal",
	"step":"step",
	"stop":"stop",
	"sw":"southwest",
	     
	"take":"get",
	"teach":"teach",
	"throw":"throw",
	"toggle":"toggle",
	"turn":"turn",
	"typo":"typo",

	"u":"up",
	"unlock":"unlock",
	"untie":"untie",
	"unwield":"unwield",
	"up":"up",
	"use":"use",
	     
	"w":"west",
	"wake":"wake",
	"walk":"walk",
	"whisper":"whisper",
	"who":"who",
	"wield":"wield",
	"wear":"wear",
	"west":"west",
	"write":"write",
	]);

    /* this mapping defines in which cases the cmd-usage is allowed */
    cmdcheck = ([
	"?":ALL_STATE,
	
	"accept":(COMBAT | CONCENTRATE | RESTING | ON_GROUND | MOUNT),
	"aid":(RESTING | HIDING | ON_GROUND),
	"alias":ALL_STATE,
	"analyse":(RESTING | HIDING | ON_GROUND | MOUNT),
	"answer":(WORKING | COMBAT | RESTING | WALKING | SEARCHING |
		  CONCENTRATE | ON_GROUND | MOUNT),
	"appraise":(WORKING| PARALYSED | COMBAT | WALKING | RESTING | HIDING |
		    GHOST | ON_GROUND | MOUNT),
	"assist":(COMBAT | MOUNT),
	"attach":(COMBAT | CONCENTRATE | RESTING | ON_GROUND | MOUNT),
	"attack":(COMBAT | MOUNT),

	"backstab":HIDING,
	"bug":ALL_STATE,
	"butcher":(RESTING | ON_GROUND | HIDING),
	"buy":0,

	"cancel":ALL_STATE,
	"carve":(RESTING | ON_GROUND | HIDING),
	"cast":(COMBAT | WALKING | MOUNT),
	"cook":(RESTING),
	"channel":(COMBAT | RESTING | WALKING | SEARCHING |
		   CONCENTRATE | GHOST | ON_GROUND | MOUNT),
	"close":(COMBAT | CONCENTRATE | RESTING | WALKING | HIDING |
		 ON_GROUND | MOUNT),
	"command":(WORKING | COMBAT | RESTING | WALKING | SEARCHING |
		   CONCENTRATE | GHOST | ON_GROUND | MOUNT),
	"count":(WORKING | COMBAT | PARALYSED | RESTING | WALKING | HIDING |
		 GHOST | ON_GROUND | MOUNT),

	"d":(COMBAT | CONCENTRATE | GHOST),
	"detach":(COMBAT | CONCENTRATE | RESTING | ON_GROUND | MOUNT),
	"disarm":(COMBAT | RESTING | ON_GROUND | MOUNT),
	"dismount":MOUNT,
	"down":(COMBAT | CONCENTRATE | GHOST),
	"drink":(COMBAT | CONCENTRATE | RESTING | WALKING | HIDING |
		 ON_GROUND | MOUNT),
	"drop":(WORKING | COMBAT | CONCENTRATE | RESTING | WALKING | HIDING |
		ON_GROUND | MOUNT),
	     
	"e":(COMBAT | CONCENTRATE | GHOST | MOUNT),
	"east":(COMBAT | CONCENTRATE | GHOST | MOUNT),
	"eat":(COMBAT | CONCENTRATE | RESTING | HIDING | ON_GROUND | MOUNT),
	"email":ALL_STATE,
	"enter":(COMBAT | CONCENTRATE | GHOST),
	"exa":(WORKING | COMBAT | CONCENTRATE | RESTING | WALKING | SEARCHING |
	       HIDING | GHOST | ON_GROUND | MOUNT),
	"examine":(WORKING | COMBAT | CONCENTRATE | RESTING | WALKING |
		   SEARCHING | HIDING | GHOST | ON_GROUND | MOUNT),
	"ext":(COMBAT | CONCENTRATE | RESTING | WALKING | ON_GROUND | MOUNT),
	"extinguish":(COMBAT | CONCENTRATE | RESTING | WALKING |
		      ON_GROUND | MOUNT),

	"fill":(RESTING | HIDING | ON_GROUND | MOUNT),
	"follow":(COMBAT | CONCENTRATE | GHOST | MOUNT),
	"forget":ALL_STATE,
	"from":ALL_STATE,
	     
	"get":(COMBAT | CONCENTRATE | RESTING | HIDING | ON_GROUND | MOUNT),
	"give":(COMBAT | CONCENTRATE | RESTING | ON_GROUND | MOUNT),
	"grumble":(WORKING | COMBAT | RESTING | WALKING | SEARCHING |
		   CONCENTRATE | GHOST | ON_GROUND | MOUNT),
	     
	"help":ALL_STATE,
	"hide":HIDING,
	"hp":(ALL_STATE ^ GHOST),
		 
	"i":(WORKING | COMBAT | WALKING | SEARCHING | RESTING | HIDING |
	     GHOST | ON_GROUND | MOUNT),
	"idea":ALL_STATE,
	"inventory":(WORKING | COMBAT | WALKING | SEARCHING | RESTING |
		     HIDING | GHOST | ON_GROUND | MOUNT),
		 
	"knot":(RESTING | ON_GROUND | MOUNT | HIDING),
	
	"l":(WORKING | COMBAT | PARALYSED | CONCENTRATE | RESTING | WALKING |
	     SEARCHING | HIDING | GHOST | ON_GROUND | MOUNT),
	"language":ALL_STATE,
	"learn":0,
	"learncost":ALL_STATE,
	"light":(COMBAT | CONCENTRATE | RESTING | ON_GROUND | MOUNT),
	"lock":(RESTING | ON_GROUND | MOUNT),
	"talk":(WORKING | COMBAT | RESTING | WALKING | SEARCHING |
		CONCENTRATE | GHOST | ON_GROUND | MOUNT),
	"look":(WORKING | COMBAT | PARALYSED | CONCENTRATE | RESTING |
		WALKING | SEARCHING | HIDING | GHOST | ON_GROUND | MOUNT),
		 
	"mail":ALL_STATE,
	"meditate":(RESTING | HIDING | ON_GROUND),
	"mood":ALL_STATE,
	"mount":0,
	"mumble":(WORKING | COMBAT | RESTING | WALKING | SEARCHING |
		  CONCENTRATE | GHOST | ON_GROUND | MOUNT),

	"n":(COMBAT | CONCENTRATE | GHOST | MOUNT),
	"ne":(COMBAT | CONCENTRATE | GHOST | MOUNT),
	"news":ALL_STATE,
	"north":(COMBAT | CONCENTRATE | GHOST | MOUNT),
	"northeast":(COMBAT | CONCENTRATE | GHOST | MOUNT),
	"northwest":(COMBAT | CONCENTRATE | GHOST | MOUNT),
	"nw":(COMBAT | CONCENTRATE | GHOST | MOUNT),

	"open":(COMBAT | CONCENTRATE | RESTING | HIDING | ON_GROUND | MOUNT),

	"parry":ALL_STATE,
	"pick":(HIDING | MOUNT),
	"pour":(ON_GROUND | RESTING | SEARCHING),
	"pray":(WORKING | COMBAT | RESTING | WALKING | SEARCHING |
		HIDING | GHOST | ON_GROUND | MOUNT),
	"prepare":ALL_STATE,
	"prompt":ALL_STATE,
	"pull":(COMBAT | CONCENTRATE | RESTING | HIDING | ON_GROUND | MOUNT),
	"push":(COMBAT | CONCENTRATE | RESTING | HIDING | ON_GROUND | MOUNT),
	"put":(COMBAT | CONCENTRATE | RESTING | HIDING | ON_GROUND | MOUNT),
	     
	"read":(RESTING | HIDING | GHOST | ON_GROUND | MOUNT),
	"remember":ALL_STATE,
	"remove":(RESTING | HIDING | ON_GROUND | MOUNT),
	"repair":(RESTING | ON_GROUND),
	"rest":(WORKING | CONCENTRATE | HIDING | ON_GROUND),
	"roll":(CONCENTRATE | RESTING | ON_GROUND | HIDING),
	"run":(COMBAT | CONCENTRATE | GHOST | MOUNT),
		 
	"s":(COMBAT | CONCENTRATE | GHOST | MOUNT),
	"save":ALL_STATE,
	"say":(WORKING | COMBAT | RESTING | WALKING | SEARCHING | CONCENTRATE |
	       GHOST | ON_GROUND | MOUNT),
	"se":(COMBAT | CONCENTRATE | GHOST | MOUNT),
	"search":0,
	"sell":0,
	"shoot":(COMBAT | HIDING | MOUNT),
	"shout":(WORKING | COMBAT | RESTING | WALKING | SEARCHING |
		 ON_GROUND | MOUNT),
	"shriek":(WORKING | COMBAT | RESTING | WALKING | SEARCHING | GHOST |
		  ON_GROUND | MOUNT),
	"sing":(WORKING | COMBAT | RESTING | WALKING | SEARCHING | GHOST |
		ON_GROUND | MOUNT),
	"sit":(WORKING | CONCENTRATE),
	"skills":ALL_STATE,
	"skin":(RESTING | ON_GROUND | HIDING),
	"sla":(COMBAT | PARALYSED | RESTING | WALKING |
	       SEARCHING | HIDING | GHOST | ON_GROUND | MOUNT),
	"sleep":(RESTING | ON_GROUND),
	"smash":(COMBAT | MOUNT),
	"sneak":(HIDING | GHOST),
	"south":(COMBAT | CONCENTRATE | GHOST | MOUNT),
	"southeast":(COMBAT | CONCENTRATE | GHOST | MOUNT),
	"southwest":(COMBAT | CONCENTRATE | GHOST | MOUNT),
	"spells":ALL_STATE,
	"stand":(WORKING | COMBAT | CONCENTRATE | RESTING | HIDING |
		 ON_GROUND),
	"stats":(ALL_STATE ^ GHOST),
	"steal":HIDING,
	"step":(COMBAT | CONCENTRATE | GHOST | MOUNT),
	"stop":ALL_STATE,
	"sw":(COMBAT | CONCENTRATE | GHOST | MOUNT),
	     
	"teach":0,
	"throw":(COMBAT | RESTING | WALKING | MOUNT),
	"toggle":ALL_STATE,
	"turn":(COMBAT),
	"typo":ALL_STATE,

	"u":(COMBAT | CONCENTRATE | GHOST),
	"unlock":(RESTING | ON_GROUND | MOUNT),
	"untie":(RESTING | ON_GROUND | MOUNT | HIDING),
	"unwield":(COMBAT | CONCENTRATE | RESTING | WALKING | HIDING |
		   ON_GROUND | MOUNT),
	"up":(COMBAT | CONCENTRATE | GHOST),
	"use":(COMBAT | RESTING | ON_GROUND | MOUNT),
		 
	"w":(COMBAT | CONCENTRATE | GHOST | MOUNT),
	"wake":(ALL_STATE ^ UNCONSCIOUS),
	"walk":(COMBAT | CONCENTRATE | GHOST | MOUNT),
	"whisper":(WORKING | COMBAT | RESTING | WALKING | SEARCHING |
		   CONCENTRATE | GHOST | ON_GROUND | MOUNT),
	"who":ALL_STATE,
	"wield":(COMBAT | CONCENTRATE | RESTING | WALKING | HIDING |
		 ON_GROUND | MOUNT),
	"wear":(RESTING | HIDING | ON_GROUND | MOUNT),
	"west":(COMBAT | CONCENTRATE | GHOST | MOUNT),
	"write":0,
	]);
    mkhelpindex();
}

/*
 * Function name: get_soul_id
 * Description:   gives a name for this soul
 */
string
get_soul_id()
{
    return "misc";
}

/*
 * Function name: query_cmdlist
 * Description:   gives the command mapping
 * Returns:       the cmds in a mapping
 */
mapping
query_cmdlist()
{
    return cmdlist;
}

/*
 * Function name: query_cmdlist
 * Description:   gives the command check mapping
 * Returns:       the cmds in a mapping with check values
 */
mapping
query_cmdcheck()
{
    return cmdcheck;
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
    
    if (cmd[0] == '\'')
    {
	arg = cmd[1..] + (!arg ? "" : " " + arg);
	cmd = "say";
    }

    if (!(func = cmdlist[cmd]))
	return 0;
    tp = this_player();
#include "../check_state.c"
    return call_other(this_object(), func, arg, tp);
}

/*
 * Function name: check_size
 * Description:   check size of found objects, display fail msg if bad size
 * Arguments:     list - the list of the found objects
 *                msg - fail msg to print
 *                tp - acting player
 * Returns:       1 if size ok, 0 else
 */
static nomask int
check_size(mixed list, string msg, object tp)
{
    if (!sizeof(list))
    {
	tp->catch_tell(msg);
	return 0;
    }
    if (sizeof(list) > 1 ||
	(objectp(list[0]) && list[0]->query_split_size() > 1))
    {
	tp->catch_tell("You need to specify one tool/target.\n");
	return 0;
    }
    return 1;
}

/*
 * Function name: mkhelpindex
 * Description:   search through help files and create new index mapping
 */
nomask void
mkhelpindex()
{
    int    i, j, l;
    string cur, curdir, *contents, *dirs;
    mixed  entry;

    dirs = get_dir(HELPPATH);
    helpindex = ([ ]);
    for (i=sizeof(dirs); i--;)
    {
	curdir = dirs[i];
	if (curdir == "creator")
	    continue;
	if (file_size(HELPPATH + curdir) == -2)
	{
	    curdir += "/";
	    contents = get_dir(HELPPATH + curdir + "*.doc");
	    for (j=sizeof(contents); j--;)
	    {
		cur = contents[j];
		cur = cur[..strlen(cur)-5];
		cur = implode(explode(cur, "_"), " ");
		entry = helpindex[cur];
		if (pointerp(entry))
		    helpindex[cur] += ({ curdir });
		else if (stringp(entry))
		    helpindex[cur] = ({ entry, curdir });
		else
		    helpindex[cur] = curdir;
	    }
	}
    }
}

/*
 * Function name: get_language
 * Description:   help function. Get language name for a language skill
 * Arguments:     skill - the language skill
 * Returns:       string
 */
static nomask string
get_language(int skill)
{
    switch (skill)
    {
      case SK_WESTRON:
	return "westron";
      case SK_ROHERIC:
	return "roheric";
      case SK_ADUNAIC:
	return "adunaic";
      case SK_QUENYA:
	return "quenya";
      case SK_SINDARIN:
	return "sindarin";
      case SK_KHUZDUL:
	return "khuzdul";
      case SK_BLACK_SPEECH:
	return "black speech";
      default:
	return "";
    }
}

/*
 * Function name: modify_language
 * Description:   modifies string if someone hears words
 * Arguments:     lang - the language skill
 *                mod - the said sentence
 *                letters - language typical letters
 *                ob - the listening object
 * Returns:       the modified string
 */
static nomask mixed
modify_language(int lang, string mod, int *letters, object ob)
{
    int i, sz, sk, lsz;

    if (!(sk = ob->query_skill_rank(lang)))
	return 0;
    
    if (sk >= 20)
	return mod;
    lsz = sizeof(letters);
    for (i=0, sz=strlen(mod); i<sz; i+=sk)
    {
	switch(mod[i])
	{
	  case '!'..'/':
	  case ':'..'?':
	    continue;
	  case 'a'..'z':
	  case '0'..'9':
	    mod[i] = letters[random(lsz)]; break;
	  case 'A'..'Z':
	    mod[i] = letters[random(lsz)] ^ 32; break;
	}
    }
    return mod;
}

/*
 * Function name: modify_string
 * Description:   modify a given string with language skill
 * Arguments:     str - the string to modify
 *                lang - the language skill to check
 *                tp - the looking player
 * Returns:       modified string
 */
nomask string
modify_string(string str, int lang, object tp)
{
    int *letters;
    
    if (!(letters = langletters[lang]))
	letters = langletters[SK_WESTRON];
    return modify_language(lang, str, letters, tp);
}

/*
 * Function name: range_attack_check(object target, object tp, object wob, 
 *                                   int *co_tar, int *co_tp)
 * Description:   check various things for ranged attack like throw or shooting
 * Arguments:     target - target object
 *                tp - attacking player
 *                wob - weapon object
 *                co_tar - target coordinates
 *                co_tp - attacker coordinates
 * Returns:       1 if success, -1 else
 */
static nomask int
range_attack_check(object target, object tp, object wob,
		   int *co_tar, int *co_tp)
{
    int max, diff, range;
    
    if (tp == target)
    {
	tp->catch_tell("You shouldn't hurt yourself.\n");
	return -1;
    }
    if (!(co_tar = target->query_coordinates(co_tp)))
	return -1;
    if (distance(co_tp, co_tar, MELEE_DISTANCE))
    {
	tp->catch_tell("You stand too near.\n");
	return -1;
    }
    max = co_tp[0] - co_tar[0];
    max = ABS(max);
    diff = co_tp[1] - co_tar[1];
    diff = ABS(diff);
    if (max < diff)
	max = diff;
    diff = co_tp[2] - co_tar[2];
    diff = ABS(diff);
    if (max < diff)
	max = diff;
    if (!(range = wob->query_weapon_range()))
	range = 200;
    if (max > range * 4 / 3)
    {
	tp->catch_tell("Too far away.\n");
	return -1;
    }
    return (range * 200) / (range * 3) - 10;
}

/*
 * Function name: parse_items
 * Description:   parse a pseudo item
 * Arguments:     command - the wanted item
 *                obj - environment and all visible object for the player
 * Returns:       the descriptions of the items found, else 0
 */
nomask string *
parse_items(string command, object *obj)
{
    int    i, j, f, sz, sz2, num, type;
    string s1, s2, *desc;
    mixed  found, current;

    if (!command || command == "all" || !(sz = sizeof(obj)))
	return 0;
    /* extract number-word if present */
    if (sscanf(command, "%s %s", s1, s2) == 2 && (num = LANG_WNUM(s1)))
	command = s2;
    if (num < 0) 
    {
	/* ordinal number, add all previous matches (from parse) */
	num += get_parse_result();
	if (num > -1)
	    return 0;
    }
    if (!num)         /* match singular or plural */
    {
	type = 3;
	num = 99999999;
    }
    else if (num > 1) /* match only plural */
	type = 2;
    else              /* match only singular */
	type = 1;
    /* check all items */
    for (i=0, found=allocate(sz); i<sz; i++)
    {
	if (!(found[i] = obj[i]->parse_item(command, type)))
	    continue;
	f += sizeof(found[i]);
	if (f == num || f == -num)
	    break;
    }
    if (!f)
	return 0;
    found -= ({ 0 });
    sz = sizeof(found);
    if (num > 0)
    {
	desc = allocate(f);
	for (i=f=0; i<sz; i++)
	{
	    current = found[i];
	    for (j=0, sz2=sizeof(current); j<sz2; j++)
	    {
		desc[f++] = current[j][2];
		if (!(--num))
		    break;
	    }
	}
	return desc - ({ 0 });
    }
    if (f < -num)
	return 0;
    for (i=0; i<sz; i++)
    {
	for (j=0, sz2=sizeof(found[i]); j<sz2; j++)
	    if ((++num) > -1)
		return ({ found[i][j][2] });
    }
    return 0;
}

#include "accept.c"
#include "aid.c"
#include "alias.c"
#include "analyse.c"
#include "answer.c"
#include "appraise.c"
#include "assist.c"
#include "attach.c"
#include "attack.c"
#include "backstab.c"
#include "bug.c"
#include "butcher.c"
#include "buy.c"
#include "cancel.c"
#include "carve.c"
#include "cast.c"
#include "channel.c"
#include "close.c"
#include "cook.c"
#include "communicate.c"
#include "count.c"
#include "detach.c"
#include "disarm.c"
#include "dismount.c"
#include "down.c"
#include "drink.c"
#include "drop.c"
#include "eat.c"
#include "enter.c"
#include "extinguish.c"
#include "fill.c"
#include "follow.c"
#include "forget.c"
#include "from.c"
#include "get.c"
#include "give.c"
#include "help.c"
#include "hide.c"
#include "hp.c"
#include "idea.c"
#include "inventory.c"
#include "knot.c"
#include "language.c"
#include "learn.c"
#include "learncost.c"
#include "light.c"
#include "lock.c"
#include "look.c"
#include "mail.c"
#include "meditate.c"
#include "mood.c"
#include "mount.c"
#include "moving.c"
#include "news.c"
#include "open.c"
#include "parry.c"
#include "pick.c"
#include "pour.c"
#include "pray.c"
#include "prepare.c"
#include "prompt.c"
#include "pull.c"
#include "push.c"
#include "put.c"
#include "read.c"
#include "remember.c"
#include "remove.c"
#include "repair.c"
#include "rest.c"
#include "roll.c"
#include "save.c"
#include "search.c"
#include "sell.c"
#include "shoot.c"
#include "sit.c"
#include "skills.c"
#include "skin.c"
#include "sla.c"
#include "sleep.c"
#include "smash.c"
#include "sneak.c"
#include "spells.c"
#include "stand.c"
#include "stats.c"
#include "steal.c"
#include "stop.c"
#include "teach.c"
#include "throw.c"
#include "toggle.c"
#include "turn.c"
#include "typo.c"
#include "unlock.c"
#include "untie.c"
#include "unwield.c"
#include "up.c"
#include "use.c"
#include "wake.c"
#include "wear.c"
#include "who.c"
#include "wield.c"
#include "write.c"
