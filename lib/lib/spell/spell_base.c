inherit "/system/parse";

#include <combat.h>
#include <composite.h>
#include <macros.h>
#include <magic.h>
#include <state.h>
#include <properties.h>
#include <event.h>
static int    level;
static object caster, *targets;
static mixed  data;

/*
 * Function name: query_desc
 * Description:   gives spell description string, used for online help
 * Returns:       string
 */
string
query_desc()
{
    return "No help available for this spell.\n";
}

/*
 * Function name: query_caster
 * Description:   gives caster of this spell
 * Returns:       object
 */
object
query_caster()
{
    return caster;
}

/*
 * Function name: query_targets
 * Description:   gives current affected targets of this spell
 * Returns:       object array
 */

object *
query_targets()
{
    targets -= ({ 0 });    
    return targets;
}

/*
 * Function name: query_cast_level
 * Description:   gives the level the spell was casted at
 * Returns:       integer
 */
int
query_cast_level()
{
    return level;
}

/*
 * Function name: query_level
 * Description:   gives base level of the spell without modifications
 * Returns:       integer
 */
int
query_level()
{
    return 0;
}

/*
 * Function name: query_data
 * Description:   gives additional casting spell data, this is different
 *                for every spell
 * Returns:       mixed
 */
mixed
query_data()
{
    return data;
}

/*
 * Function name: query_spell_data
 * Description:   gives spell configuration data
 * Returns:       integer array
 */
int *
query_spell_data()
{
    return 0;
}

/*
 * Function name: spell_effect(int level, mixed targets, object tp,
 *                             string extra)
 * Description:   redefinable spell effect function, called if spell is
 *                successfully casted
 * Arguments:     level - the casting level
 *                targets - all targeted objects
 *                tp - the caster
 *                extra - optional extra information
 */
varargs void
spell_effect(int level, mixed targets, object tp, string extra)
{
}

int
test_success(object tp, int clev, int knownlev, int type, int prep)
{
    return 0;
}

int
query_rr_mod()
{
    return 0;
}

object *
resist(object tp, object *targets, int level, int type)
{
    return ({ });
}

/*
 * Function name: query_similar_code
 * Description:   gives a unique code id of the spell to forbid accumulative
 *                use
 * Returns:       mixed id
 */
mixed
query_similar_code()
{
    return 0;
}

/*
 * Function name: query_no_targets 
 * Description:   get the number of possible targets according to the
 *                casting level
 * Arguments:     level - the casting level
 * Returns:       integer
 */
int
query_no_targets(int level)
{
    return 0;
}

/*
 * Function name: message
 * Description:   default spell message
 * Arguments:     tp - caster
 *                tar - spell targets
 */
void
message(object tp, object *tar)
{
    if (sizeof(tar) && tar[0] != tp)
    {
	int *r;
	
	if ((r = query_spell_data())[SP_RANGE] == SP_TOUCH)
	{
	    mixed qctname;
	
	    qctname = QCTNAME(tp);
	    tp->catch_tell("You touch " + tar[0]->query_objective() + ".\n");
	    tar[0]->catch_msg(({ qctname, " touches you.\n" }), tp,
			      MSG_SEE);
	    tell_objects(LISTEN(E(tp)) - ({ tp, tar[0] }),
			 ({ qctname, " touches ", QTNAME(tar[0]),
			    ".\n" }), MSG_SEE, tp);
	}
	else if (r[SP_RANGE] > 0 && r[SP_TARGETS] == 1)
	{
	    mixed qctname;
	    
	    qctname = QCTNAME(tp);
	    tp->catch_tell("You point at " + tar[0]->query_objective() +
			   ".\n");
	    tar[0]->catch_msg(({ qctname, " points at you.\n" }), tp,
			      MSG_SEE);
	    tell_objects(LISTEN(E(tp)) - ({ tp, tar[0] }),
			 ({ qctname, " points at ", QTNAME(tar[0]),
			    ".\n" }), MSG_SEE, tp);
	}
    }
}

/*
 * Function name: filter_similar
 * Description:   filter out targets affected by similar spell
 * Arguments:     tar - all targets that should be checked
 *                tp - casting player 
 * Returns:       array of still affected objects
 */
static nomask object *
filter_similar(object *tar, object tp)
{
    int    i, j, sz, sz2;
    object *spells, *affected;
    mixed  code;
    
    if (!(code = query_similar_code()))
	return tar;
    
    for (i=0, sz=sizeof(tar), affected=allocate(sz); i<sz; i++)
    {
	if (sz2 = sizeof(spells = tar[i]->query_spells()))
	{
	    for (j=0; j<sz2; j++)
		if (spells[j]->query_similar_code() == code)
		{
		    affected[i] = tar[i];
		    tar[i] = 0;
		    break;
		}
	}
    }
    affected -= ({ 0 });
    if ((sz = sizeof(affected)) == 1)
    {
	if (affected[0] == tp)
	    tp->catch_msg( ({ "You seem to be already affected.\n" }),
			   0, MSG_SEE);
	else
	    tp->catch_msg(({ QCTNAME(affected[0]),
				 " seems to be already affected.\n" }),
			  0, MSG_SEE);
    }
    else if (sz > 1)
	tp->catch_msg(({ "But " }) + COMPOSITE_ARRAY(affected) +
	({ " seem to be already affected.\n" }), 0, 0);
    
    return tar - ({ 0 });
}

/*
 * Function name: store_spell
 * Description:   store spell data for detect/dispel/etc. spells
 * Arguments:     c - the caster
 *                l - level spell was casted on
 *                t - all affected targets
 *                d - optional spell data
 */
varargs void
store_spell(object c, int l, object *t, mixed d)
{
    caster  = c;
    level   = l;
    targets = t;
    data    = d;
}

/*
 * Function name: directed_spell
 * Description:   check hit for a directed spell
 * Arguments:     tp - the caster
 *                tar - target
 *                atype - attack type (skill)
 *                mod - roll modifier (e.g. elemental protection)
 * Returns:       difference ob-db, or -1 if missed
 */
static int
directed_spell(object tp, object tar, int atype, int mod)
{
    int diff;
    
    if ((diff = (tp->cb_query_missile_ob(atype, 0) -
		 tar->cb_query_missile_db() + roll_dice(-1) - mod)) < 0)
	return -1;
    return diff;
}

/*
 * Function name: directed_damage(object tp, object tar, int diff, int clev,
 *                                mapping at_modifier, mapping ob_modifier)
 * Description:   compute and do damage for a directed spell
 * Arguments:     tp - the caster
 *                tar - target
 *                diff - difference ob-db
 *                clev - casting level
 *                at_data - attack data mapping
 * Returns:       integer damage , -1 if missed
 */
static void
directed_damage(object tp, object tar, int diff, int clev, mapping at_data)
{
    int multi, damage;
    
    damage = DAMAGE_LIVING(tp, tar, diff, at_data, tar->cb_random_hitloc());
    if (tar) /* target might get lost by critical */
    {
	if (!(multi = clev / query_level()))
	    multi = 1;
	tar->check_death(damage * multi, this_object());
    }
}

/*
 * Function name: test_summon
 * Description:   test if a summoned object could materialize
 *                the spell fails if an object is inside the summon area
 * Arguments:     sumco - the summoned objects base coordinates
 *                range - the size of the summoned object
 *                obj - all possible resisting targets
 * Returns:       1 if successful, 0 if spell failed
 */
int
test_summon(int *sumco, int *range, object *obj)
{
    int i, sz;
    
    for (i=0, sz = sizeof(obj); i<sz; i++)
    {
	if (in_range(obj[i]->query_coordinates(), sumco, range))
	    return 0;
    }
    return 1;
}

/*
 * Function name: parse_targets(string str, int max, int dist, int ttype,
 *                              int *co, object tp)
 * Description:   parse visible spell targets
 * Arguments:     str - string to parse
 *                max - maximum number of targets allowed
 *                dist - maximum distance to target
 *                ttype - target type (only livings, objects, etc. )
 *                level - casting level
 *                co - players coordinates
 *                tp - casting player
 * Returns:       array of valid targets
 */
varargs object *
parse_targets(string str, int max, int dist, int ttype, int level,
	      int *co, object tp)
{
    int    i, sz, step;
    object env, *obj, *tar;
    
    if (!tp)
	return 0;
    env = E(tp);
    if (!str || str == "me")
    {
	if (ttype == SP_ALL_TARGETS || ttype == SP_ALL_LIVINGS)
	    obj = ({ tp });
    }
    else
    {
	if (ttype == SP_ALL_TARGETS)
	    obj = I(tp, 4) + I(env, 0);
	else if (ttype == SP_ALL_LIVINGS)
	    obj = I(env, 2);
	else if (ttype == SP_ALL_OBJECTS)
	    obj = I(tp, 4) + I(env, 1);
	obj = tp->visible(obj);
	obj = parse(str, obj, (ttype == SP_ALL_OBJECTS ? 0 : tp));
    }
    if (!(sz = sizeof(obj)))
    {
	tp->catch_tell("Cast on whom/what/where?\n");
	return 0;
    }
    if (dist == SP_SELF)
    {
	if (obj[0] == tp)
	    return ({ tp });
	tp->catch_tell("You may only cast the spell on yourself.\n");
	return 0;
    }
    step = tp->query_step();
    tar = allocate(sz);
    if (dist == SP_TOUCH)
    {
	if (direct_approach(tp, obj[0]))
	    tar[0] = obj[0];
    }
    else
    {
	for (i=0; i<sz; i++)
	{
	    if (!distance(co, obj[i]->query_coordinates(), dist))
		continue;
	    tar[i] = obj[i];
	}
    }	
    tar -= ({ 0 });
    
    if ((max == SP_LEVEL_TARGETS) && !(max = query_no_targets(level)))
	max = level;
    
    if ((max != SP_LEVEL_RANGE) && (sz = sizeof(tar)) > max)
	tar = tar[..max-1];
    obj -= tar;
    if (sizeof(obj))
	tp->catch_msg(({ "The spell is not powerful enough to affect or " +
			     "reach " }) +
		      COMPOSITE_ARRAY(obj) + ({ ".\n" }), 0, 0);
    return tar;
}

/*
 * Function name: parse_dir_string
 * Description:   parse a direction string of the form 2n3e
 * Arguments:     str - the direction string
 *                tp - the player that has given the string
 *                co - player coordinates
 *                range - spell range
 * Returns:       array of coordinates
 *                0 if no valid direction
 */
int *
parse_dir_string(string str, object tp, int *co, int range)
{
    int    d1, d2, *res;
    string dir1, dir2;
    object room, here;

    if (!tp)
	return 0;
    if (!(res = DIRDESC_TO_INT(str)))
    {
	tp->catch_tell("This is no valid direction (e.g. 3n4e).\n");
	return 0;
    }
    res = ({ res[1] * 12 + co[0], res[0] * 12 + co[1], co[2] });
    if (!distance(co, res, range))
    {
	tp->catch_tell("That's too far. The distance could be " + (range/12) +
		       " feet here.\n");
	return 0;
    }
    here = E(tp);
    if ((room = here->query_leaving_room(res)) != here)
    {
	if (!room)
	{
	    tp->catch_tell("No way there.\n");
	    return 0;
	}
	if (room->query_leaving_room(res) != room)
	{
	    tp->catch_tell("That's too far.\n");
	    return 0;
	}
    }
    return res;
}

/*
 * Function name: spell_now(int level, int knownlev, int prep,
 *                          string target, object ob, string extra,
 *                          string words)
 * Description:   execute the spell without any more delay
 * Arguments:     level - casting level
 *                knownlev - learned spell-list level
 *                prep - preparation time
 *                target - target name(s)
 *                ob - the casting object
 *                extra - extra spell arguments
 * Returns:       1 if successful, -1 else
 */
private int
spell_now(int level, int knownlev, int prep, string target, object ob,
	  string extra)
{
    int    *data, *co, *co_target;
    string dir;
    object *tar;
    
    if (!ob) 
	return -1;
    ob->remove_extra_desc(" (casting)");
    if (!(co = ob->query_coordinates()))
	return -1;
    if (sizeof(data = query_spell_data()) != SP_DATA_SIZE)
	error("Bad spelldata");
    
    if (data[SP_TARGETS] == SP_DIRECTION)
    {
	if (!(co_target = parse_dir_string(target, ob, co, data[SP_RANGE])))
	    return -1;
	if (living(ob) &&
	    !test_success(ob, level, knownlev, data[SP_TYPE], prep))
	    return -1;
	spell_effect(level, co_target, ob, extra);
	return -1;
    }
    if (!sizeof(tar = parse_targets(target, data[SP_TARGETS], data[SP_RANGE],
				    data[SP_TARGET_TYPE], level, co, ob)))
	return -1;
    if (living(ob))
    {
	if (!test_success(ob, level, knownlev, data[SP_TYPE], prep))
	    return -1;
	message(ob, tar);
    }
    tar = resist(ob, tar, level, data[SP_TYPE]);
    tar = filter_similar(tar, ob);
    spell_effect(level, tar, ob, extra);
    return 1;
}

/*
 * Function name: do_spell
 * Description:   start casting a spell
 * Arguments:     level - the level the spell will be casted
 *                knownlev - the level the spell list has be learned to
 *                target - target name(s)
 *                ob - the object casting
 *                extra - extra spell arguments
 *                words - the spell formula
 * Returns:       1 if successful, -1 else
 */
nomask int
do_spell(int level, int knownlev, string target, object ob, string extra,
	 string words)
{
    int prep, *data;
    
    if (sizeof(data = query_spell_data()) != SP_DATA_SIZE)
	error("Bad spelldata");
    
    if (living(ob))
    {
	ob->catch_tell("You utter the magical words.\n");
	tell_objects(LISTEN(E(ob)) - ({ ob }),
		     ({ QCTNAME(ob), " utters the words: " + words +
			    "\n" }), MSG_HEAR, ob, 590);
	ob->add_extra_desc(" (casting)");
    }
    if (!(data[SP_TYPE] & SP_INSTANTLY))
    {
	prep = ob->query_prop(LIVE_PREPARE_RNDS);
	ob->add_state(CONCENTRATE);
	ob->change_int_prop(SPELL_CODE, 1); /* change spell code value */
	call_out("delay_spell", CAST_RND + prep * CAST_RND,
		 level, knownlev, prep, target, ob, extra,
		 ob->query_prop(SPELL_CODE));
	return 1;
    }
    return spell_now(level, knownlev, 0, target, ob, extra);
}

/*
 * Function name: delay_spell(int level, int knownlev, int prep,
 *                            string target, object ob, string extra, int code)
 * Description:   delay the spell (prepare time)
 * Arguments:     level - the level the spell will be casted
 *                knownlev - the level the spell list has be learned to
 *                prep - preparation time
 *                target - target name(s)
 *                ob - the object casting
 *                extra - extra spell arguments
 *                code - spell id
 */
static nomask void
delay_spell(int level, int knownlev, int prep, string target, object ob,
	    string extra, int code)
{
    int *data;
    
    if (!(ob->query_state() & CONCENTRATE) ||
	ob->query_prop(SPELL_CODE) != code)
	return;

    ob->delete_state(CONCENTRATE);
    if (sizeof(data = query_spell_data()) != SP_DATA_SIZE)
	error("Bad spelldata");
    
    spell_now(level, knownlev, prep, target, ob, extra);
}

/*
 * Function name: remove_spell
 * Description:   stop a spell effect
 * Arguments:     t - optional target object, if 0 spell is totally removed
 */
varargs void
remove_spell(object t)
{
}

/*
 * Function name: start_call_out
 * Description:   start a callout. start a call_out within clone from master
 * Arguments:     fun - callout function
 *                delay - delay in seconds
 *                arg - optinal argument
 */
varargs void
start_call_out(string fun, int delay, mixed arg)
{
    call_out(fun, delay, arg);
}

/*
 * Function name: start_concentrate
 * Description:   start concentrating on a spell effect
 * Arguments:     dist - distance, check every 15 seconds
 */
void
start_concentrate(int dist)
{
    caster->add_state(CONCENTRATE);
    caster->add_prop(CONCENTRATE_DATA,
		     ({ this_object(), "stop_concentrate",
			call_out("concentrate", 15, dist), caster}));
}

/*
 * Function name: stop_concentrate
 * Description:   called if any event disturbs the concentration
 * Arguments:     id - callout id
 *                tp - the living that concentrates
 */
void
stop_concentrate(int id, object tp)
{
    remove_call_out(id);
    remove_spell();
}

/*
 * Function name: concentrate
 * Description:   check if concentration is still unbroken and
 *                remove spell effect for targets out of range
 * Arguments:     dist - spell range
 */
static void
concentrate(int dist)
{
    int i, *co_tp;
    
    targets -= ({ 0 });
    if (!caster || !(caster->query_state() & CONCENTRATE) ||
	!(i = sizeof(targets)))
    {
	remove_spell();
	return;
    }
    if (dist == SP_TOUCH && caster != targets[0] &&
	!direct_approach(caster, targets[0]))
    {
	remove_spell();
	return;
    }
    if (dist > SP_SELF)
    {
	co_tp = caster->query_coordinates();
	while (i--)
	{
	    if (!distance(co_tp, targets[i]->query_coordinates(), dist))
		/* remove all non valid targets */
		remove_spell(targets[i]);
	}
	if (!sizeof(targets))
	    /* size could have been changed by remove_spell */
	    return;
    }
    call_out("concentrate", 15, dist);
}
