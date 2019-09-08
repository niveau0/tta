inherit "/system/parse";

#include <access.h>
#include <composite.h>
#include <event.h>
#include <living.h>
#include <macros.h>
#include <magic.h>
#include <mudtime.h>
#include <properties.h>
#include <race.h>
#include <skills.h>
#include "learn/learn.h"
#include "learn/similar.h"
#include "learn/skill_costs.h"

static string   *diff_desc,    /* difficulty descriptions */
                *diff_short;   /* difficulty short descriptions */
static mapping	need_teach,    /* skills that must be teached */
    		need_loc,      /* skills that need a location */
                need_obj,      /* skills that need an object */
                group_range,   /* skillgroup boundaries */
    		group_names,   /* skillgroup names -> id */
    		group_ids,     /* skillgroup id -> names */
    		learnfactor,   /* difficulty for practice learning */
    		sk_desc,       /* skill -> description */
    		sk_groups,     /* skill -> related groups */
    		sk_attrib,     /* skill -> related attributes */
                base_cost,     /* unmodified costs for every skill */
                race_mod;      /* race modifier mapping for all races */

static nomask void
create()
{
    if (IS_CLONE)
    {
	destruct_object();
	return;
    }
    group_range	= LEARN_GROUP_RANGE;
    group_names	= LEARN_GROUP_TO_NAMES;
    group_ids	= LEARN_NAMES_TO_GROUP;
    learnfactor	= LEARN_FACTORS;
    need_loc	= LEARN_NEEDS_LOCATION; 
    need_obj	= LEARN_NEEDS_OBJECT;
    need_teach	= LEARN_NEEDS_TEACHER;
    sk_desc	= SK_DESC;
    sk_groups	= LEARN_SKILL_GROUPS;
    sk_attrib   = SKILL_ATTRIBUTES;
    base_cost   = SK_BASE_COSTS;
    race_mod    = RACE_MODIFIER;
    diff_desc   = ({ "extremely easy",
		     "very easy",
		     "easy",
		     "moderatly easy",
		     "slightly easy",
		     "slightly difficult",
		     "moderatly difficult",
		     "difficult",
		     "very difficult",
		     "extremely difficult", });
    diff_short  = ({ "ext. easy",
		     "very easy",
		     "easy",
		     "mod. easy",
		     "sli. easy",
		     "sli. diff",
		     "mod. diff",
		     "difficult",
		     "very diff",
		     "ext. diff", });
}

/*
 * Function name: query_race_mod
 * Description:   gives all modifers for a given race
 * Arguments:     race - race name
 * Returns:       array
 */
nomask mixed
query_race_mod(string race)
{
    return race_mod[race];
}

/*
 * Function name: query_group_names
 * Description:   gives the names of all skillgroups
 * Returns:       array of groupnames
 */
nomask mapping
query_group_names()
{
    return group_names;
}

/*
 * Function name: query_group_id
 * Description:   gives the groupid of given skillgroup
 * Arguments:     name - the group name
 * Returns:       integer, representing the skillgroup,
 *                0 if groupname is no legal name
 */
nomask int
query_group_id(string name)
{
    return group_ids[name];
}

/*
 * Function name: query_group_range
 * Description:   gives the boundaries of all skillgroups
 * Returns:       mapping, GROUPNUMBER: ({ START, STOP })
 */
nomask mapping
query_group_range()
{
    return group_range;
}

/*
 * Function name: query_skill_groups
 * Description:   give the arrangement of skills and their groups
 * Returns:       mapping, SKILLNUMBER:GROUPNUMBER
 */
nomask mapping
query_skill_groups()
{
    return sk_groups;
}

/*
 * Function name: query_skill_attrib
 * Description:   gives all attributes responsible for a skills bonus
 *                or a function to query the attributes
 * Arguments:     skill - the skill number
 * Returns:       array or function name
 */
nomask mixed
query_skill_attrib(int skill)
{
    if (!sk_attrib[skill])
	debug("manwe", skill);
    return sk_attrib[skill];
}

/*
 * Function name: calc_cost_modifier
 * Description:   sum up the theoretical values and store them as prop
 * Arguments:     tp - the player to calculate the cost modifier for
 * Returns:       mapping, cost modifiers
 */
nomask mapping
calc_cost_modifier(object tp)
{
    int     i, j, sz, sum;
    mapping sk_map, modifier, sums;

    modifier = ([ ]);
    sums = ([ ]);
    sk_map = tp->query_skill_map_orig();
    for (i=SK_GRP_FIRST; i<=SK_GRP_LAST; i++)
    {
	sum = 0;
	for (j=group_range[i][0], sz=group_range[i][1]; j<=sz; j++)
	    sum += sk_map[j] / 10000;
	sums[i] = sum; 
    }
    for (i=SK_GRP_FIRST; i<=SK_GRP_LAST; i++)
    {
	for (j=SK_GRP_FIRST; j<=SK_GRP_LAST; j++)
	{
	    if (i == j)
		continue;
	    modifier[i] += sums[j];
	}
    }
    tp->add_prop(LEARN_COST_MODIFIER, modifier);
    return modifier;
}

/*
 * Function name: query_cost_modifier
 * Description:   get the skill cost modifier for one group
 * Arguments:     group_id - group code, see <skills.h>
 *                tp - the player to query cost for
 * Returns:       integer, the modifier
 */
nomask int
query_cost_modifier(int group_id, object tp)
{
    mapping gmod;

    gmod = tp->query_prop(LEARN_COST_MODIFIER);
    if (!gmod || !gmod[group_id])
	gmod = calc_cost_modifier(tp);
    return gmod[group_id];
}

/*
 * Function name: query_cost
 * Description:   gives basic costs for a skill
 * Arguments:     prac - practical skill value
 *                theo - theoretical skill value
 *                skill - skill number
 *                tp - the player object
 * Returns:       integer, basic cost for skill
 */
nomask int
query_cost(int prac, int theo, int skill, object tp)
{
    int i, sz, mod, cost, *groups;

    for (i=0, sz=sizeof(groups = sk_groups[skill]); i<sz; i++)
	mod += query_cost_modifier(groups[i], tp);
    mod /= sz;

    cost = (base_cost[skill] & 0x3FF) - prac + mod / 10;
    if (cost < 5)
	cost = 5;
    else if (cost > 200)
	cost = 200;
    
    if (prac >= theo)
	return cost;
    return cost + (theo - prac) * (base_cost[skill] >> 10);
} 

/*
 * Function name: body_development
 * Description:   raise hitpoints
 * Arguments:     cost - development cost
 *                theo - theoretical knowledge
 *                tp - player object
 * Returns:       1 if success, -1 else
 */
static nomask int
body_development(int cost, int theo, object tp)
{
    int   max;
    mixed race_data;
    
    if (!(race_data = race_mod[tp->query_race()]))
    {
	tp->catch_tell("Oops! No race specific data! Please contact " +
		       "administration.\n");
	return -1;
    }
    max = tp->query_base_hp() + 1 + random(race_data[HIT_DICE]);
    if (max < race_data[MAX_HITS])
    {
	tp->catch_tell("You now have trained your body.\n");
	tp->set_skill_rank(SK_BODY_DEVELOP, theo + 1);
	tp->set_theoretical_rank(SK_BODY_DEVELOP, theo + 1);
	tp->reduce_development_points(cost);
	tp->set_base_hp(max);
	return 1;
    }
    tp->catch_tell("You have maxed out the potential of developing your " +
		   "body.\nThe limits of your race are reached.\n");
    return -1;
}

/*
 * Function name: improve_skills
 * Description:   called after resting. if the skillrank is lower than the
 *                theoretical rank, there is a chance (depending on
 *                learnfactor for the skill) that the skillrank (practical
 *                value) will raise.
 * Arguments:     tp - player
 */
nomask void
improve_skills(object tp)
{
    int     i, theo, prac, factor, chance, skill, *ind;
    mapping sk_usage;

    if (!tp)
	return;
    if (time() - tp->query_prop(SLEEP_START) < 3600/TIME_SCALE)
    {
	/* 1 hour sleep required for skill improve */
	tp->add_prop(SLEEP_START, 0);
	if (time() - tp->query_prop(RESTING_START) < 7200/TIME_SCALE)
	{
	    /* 2 hours rest required for skill improve */
	    tp->add_prop(RESTING_START, 0);
	    return;
	}
    }
    if (!(sk_usage = tp->query_skill_usage()))
	return;
    debug("manwe", "skill improve: " + tp->query_name());
    
    ind = map_indices(sk_usage);
    for (i=sizeof(ind); i--;)
    {
	skill = ind[i];
	if (skill < 1 || sk_usage[skill] < 1)
	    continue;
	prac = tp->query_skill_rank_orig(skill);
	theo = tp->query_theoretical_rank_orig(skill);
	factor = learnfactor[skill];
	if (prac < theo) 
	{
	    if (!factor)
		continue;
	    if (factor < 0) /* raise practice without test */
		tp->set_skill_rank(skill, theo);
	    else if (!random(factor * prac))
		tp->set_skill_rank(skill, prac + 1);
	    continue;
	}
	factor += 10;
	if (!random(factor * theo))
	{
	    /* maybe improve theory (and practice) by using the skill */
	    tp->set_skill_rank(skill, theo + 1);
	    tp->set_theoretical_rank(skill, theo + 1);
	}
    }
    tp->reset_skill_usage();
}

/*
 * Function name: learn_skill
 * Description:   raises a skill if enough devpoints left
 * Arguments:     skill - skill number
 *                theo - theoretical rank of skill
 *                tp - the learning player
 *                item - the object that enables us to learn, 0 if no object
 * Returns:       1 if success, -1 else
 */
static nomask int
learn_skill(int skill, int theo, object tp, object item)
{
    int cost, prac;
    
    prac = tp->query_skill_rank_orig(skill);
    cost = query_cost(prac, theo, skill, tp);
    if (cost > tp->query_development_points())
    {
	tp->catch_tell("This skill is too difficult for now.\n");
	return -1;
    }
    if (skill == SK_BODY_DEVELOP)
	return body_development(cost, theo, tp);
    tp->catch_tell("You have improved your knowledge.\n");
    tp->set_theoretical_rank(skill, theo + 1);
    tp->reduce_development_points(cost);
    if (item)
	item->teached_skill(tp, skill);
    return 1;
}

/*
 * Function name: offer_teach(int skill, object pupil, object teacher,
 *                            object item)
 * Description:   offers a lesson to the given pupil
 * Arguments:     skill   - the skill
 *                pupil   - the player to be taught
 *                teacher - the person who teaches
 *                item    - if a special object is needed,
 *                          this must be the objectpointer
 * Returns:       1 if success, -1 else
 */
static nomask int
offer_teach(int skill, object pupil, object teacher, object item)
{
    int tprac, ptheo;
    
    if ((tprac = teacher->query_skill_rank_orig(skill)) <= TEACH_MIN_RANK &&
	(ptheo = pupil->query_theoretical_rank_orig(skill)) > 0)
    {
	/* teacher must have a proper rank to teach if pupil
	 * has basic knowledge in skill
	 */
	teacher->catch_tell("You need more practice in " + sk_desc[skill] +
			    " to teach this skill.\n");
	return -1;
    }
    
    pupil->add_prop(LEARN_TEACH_DATA, ({ skill, teacher, item }));
    teacher->catch_tell("You offer your lesson.\n", 1);
    tell_objects(LISTEN(E(teacher)) - ({ teacher }),
		 ({ QCTNAME(teacher), " asks: Do you want a lesson in " +
			sk_desc[skill] + "?\n" }), MSG_HEAR, teacher, 590);
    if (!pupil->query_npc())
	teacher->add_prop(LIVE_LAST_QUESTION, this_object());
    return 1;
}

/*
 * Function name: query_spellearn_bonus
 * Description:   gives bonus that can be used once to learn a spell
 * Arguments:     realm - realm id (<magic.h> of the spell to learn
 *                tp - player object that is learning
 * Returns:       integer
 */
nomask int
query_spellearn_bonus(int realm, object tp)
{
    switch (realm)
    {
      case ARC:
	  return (tp->query_stat_bonus(STAT_IN) +
		  tp->query_stat_bonus(STAT_PR) +
		  tp->query_stat_bonus(STAT_EM)) / 3;
      case CHA:
	  return tp->query_stat_bonus(STAT_IN);
      case MEN:
	  return tp->query_stat_bonus(STAT_PR);
      case ESS:
	  return tp->query_stat_bonus(STAT_EM);
    }
    return 0;
}

/*
 * Function name: learn_spellist
 * Description:   handle the try to learn one spellist
 * Arguments:     arg - name of spell list
 * Returns:       1  - success
 *                -1 - else
 */
static nomask int
learn_spellist(string arg)
{
    int     i, sz, realm_id, cost, devpts, bonus, progress, listlevel;
    string  research_list, realm, list, path;
    object  tp, *inv;
    mapping known_spells, sk_support;
    
    tp = this_player();

    if (!(realm_id = tp->query_realm()))
    {
	/* Error, no realm */
	tp->catch_tell("You have no relation to any magic realm!\n" +
		       "Contact the administration, please.\n");
	return -1;
    }
    research_list = tp->query_prop(LEARN_RESEARCH_LIST);
    debug("ulmo", research_list);
    if (!arg || !strlen(arg))
    {
	if (!research_list || !strlen(research_list))
	    tp->catch_tell("You are not researching any list.\n");
	else
	    tp->catch_tell("You are currently researching the '" +
			   research_list + "' list.\n");
	return 1;
    }
    if (arg == "clear")
    {
	tp->catch_tell("You clear up your mind and quit your current " +
		       "research.\n");
	tp->add_prop(LEARN_RESEARCH_LIST, 0);
	return 1;
    }
    if (sscanf(arg, "%s %s", realm, list) != 2)
    {
	tp->catch_tell("Usage: learn list <realm> <list>\n");
	return -1;
    }
    if (!IN_ARRAY(realm, REALMS))
    {
	tp->catch_tell("This is no valid realm.\n");
	return -1;
    }
    /* Player can't research more than one list at the same time. */
    if (research_list && (research_list != list))
    {
	tp->catch_tell("You cannot research more than one list at once. " +
		       "You may only try to finish the '" + research_list +
		       "' list or clear your mind and focus on a new " +
		       "list. Remember that all your current efforts " +
		       "will be lost. Use 'learn list clear' to " +
		       "begin a new research.\n");
	return -1;
    }

    debug("ulmo", "SPELLPATH IST NOCH NICHT GEKLÄRT!");

    path = (SPELL_DIR + "/" + realm + "/spellpath")->query_path(list);
    if (file_size(path) != -2)
    {
	tp->catch_tell("This is no valid spellist.\n");
	return -1;
    }
    research_list = arg;
    known_spells = tp->query_prop(LIVE_KNOWN_SPELLS);
    if (mappingp(known_spells) && mappingp(known_spells[realm]))
	listlevel = known_spells[realm][list];
    /* Spells need an item to be learned (books, scrolls, etc.) */
    inv = I(tp, 4);
    for (i=0, sz=sizeof(inv); i<sz; i++)
    {
	if (mappingp(sk_support = inv[i]->query_prop(SPELLIST_SUPPORT)) &&
	    sk_support[research_list] > listlevel)
	    break;
    }
    if (i == sz)
    {
	tp->catch_tell("You need an item of wisdom to continue " +
		       "your studies.\n");
	return -1;
    }
    if (!(realm_id & REALM_ID[realm])) 
    {
	/* learn from another realm */
	cost = 1;
    }
    else 
    {
	/* learn from own realm */
	cost = 1;
    }
    devpts = tp->query_development_points();
    if (cost > devpts)
    {
	tp->catch_tell("This list is to difficult to learn it further now.\n");
	return -1;
    }
    tp->reduce_development_points(cost);
    progress = tp->query_skill_rank(SK_SPELLIST_PROGRESS);
    bonus = query_spellearn_bonus(realm_id, tp);
    if (progress >= 20 || random(100) <= progress * 5 + bonus)
    {
	tp->catch_tell("You seem to have learned everything needed to " +
		       "understand the powers of this part. Your endurance " +
		       "has been rewarded. You have now mastered the next " +
		       "part of the '" + research_list + "' spellist.\n");
	known_spells[research_list] = listlevel;
	tp->add_prop(LIVE_KNOWN_SPELLS, known_spells);
	tp->set_skill_rank(SK_SPELLIST_PROGRESS, 0);
	tp->save();
	return 1;
    }
    tp->catch_tell("You will now continue your studies.\n");
    return 1;
}

/*
 * Function name: learn_spell
 * Description:   handle the try to learn one spell
 * Arguments:     arg - name of spell list
 * Returns:       1  - success
 *                -1 - else
 */
static nomask int
learn_spell(string arg)
{
    int     i, sz, realm_id, cost, devpts, bonus, progress, spelllevel;
    string  research_spell, realm, spell, path;
    object  tp, *inv;
    mapping known_spells, sk_support;
    
    tp = this_player();

    if (!(realm_id = tp->query_realm()))
    {
	/* Error, no realm */
	tp->catch_tell("You have no relation to any magic realm!\n" +
		       "Contact the administration, please.\n");
	return -1;
    }
    research_spell = tp->query_prop(LEARN_RESEARCH_SPELL);
    if (!arg || !strlen(arg))
    {
	if (strlen(research_spell))
	    tp->catch_tell("You are currently researching the '" +
			   research_spell + "' spell.\n");
	else
	    tp->catch_tell("You are not researching any spell.\n");
	return 1;
    }
    if (arg == "clear")
    {
	tp->catch_tell("You clear up your mind and quit your current " +
		       "research.\n");
	tp->add_prop(LEARN_RESEARCH_SPELL, 0);
	return 1;
    }
    if (sscanf(arg, "%s %s", realm, spell) != 2)
    {
	tp->catch_tell("Usage: learn spell <realm> <spell>\n");
	return -1;
    }
    if (!IN_ARRAY(realm, REALMS))
    {
	tp->catch_tell("This is no valid realm.\n");
	return -1;
    }
    /* Player can't research more than one spell at the same time. */
    if (research_spell && research_spell != spell)
    {
	tp->catch_tell("You cannot research more than one spell at once. " +
		       "You may only try to finish the '" + research_spell +
		       "' spell or clear your mind and focus on a new " +
		       "spell. Remember that all your current efforts " +
		       "will be lost. Use 'learn spell clear' to " +
		       "begin a new research.\n");
	return -1;
    }
    path = (SPELL_DIR + "/" + realm + "/spellpath")->query_path(spell);
    if (file_size(path) != -2)
    {
	tp->catch_tell("This is no valid spell.\n");
	return -1;
    }
    research_spell = arg;
    known_spells = tp->query_prop(LIVE_KNOWN_SPELLS);
    if (mappingp(known_spells) && mappingp(known_spells[realm]))
	spelllevel = known_spells[realm][spell];
    /* Spells need an item to be learned (books, scrolls, etc.) */
    inv = I(tp, 4);
    for (i=0, sz=sizeof(inv); i<sz; i++)
    {
	if (mappingp(sk_support = inv[i]->query_prop(SPELLIST_SUPPORT)) &&
	    sk_support[research_spell] > spelllevel)
	    break;
    }
    if (i == sz)
    {
	tp->catch_tell("You need an item of wisdom to continue " +
		       "your studies.\n");
	return -1;
    }
    if (!(realm_id & REALM_ID[realm])) 
    {
	/* learn from another realm */
	cost = 1;
    }
    else 
    {
	/* learn from own realm */
	cost = 1;
    }
    devpts = tp->query_development_points();
    if (cost > devpts)
    {
	tp->catch_tell("This spell is to difficult to learn it further now.\n");
	return -1;
    }
    tp->reduce_development_points(cost);
    progress = tp->query_skill_rank(SK_SPELLIST_PROGRESS);
    bonus = query_spellearn_bonus(realm_id, tp);
    if (progress >= 20 || random(100) <= progress * 5 + bonus)
    {
	tp->catch_tell("You seem to have learned everything needed to " +
		       "understand the powers of this part. Your endurance " +
		       "has been rewarded. You have now mastered the next " +
		       "part of the '" + research_spell + "' spell.\n");
	known_spells[research_spell] = spelllevel;
	tp->add_prop(LIVE_KNOWN_SPELLS, known_spells);
	tp->set_skill_rank(SK_SPELLIST_PROGRESS, 0);
	tp->save();
	return 1;
    }
    tp->catch_tell("You will now continue your studies.\n");
    return 1;
}

/*
 * Function name: difficulty_desc
 * Description:   gives a difficulty code string for the given cost
 * Arguments:     cost - the cost to show as difficulty code
 *                flag - if true give short description
 * Returns:       string, difficulty code
 */
nomask varargs string
difficulty_desc(int cost, int flag)
{
    int sz;

    if (flag)
    {
	sz = sizeof(diff_short) - 1;
	cost /= 10;
	if (cost > sz)
	    cost = sz;
	return diff_short[cost];
    }
    sz = sizeof(diff_desc) - 1;
    cost /= 10;
    if (cost > sz)
	cost = sz;
    return diff_desc[cost];
}

/*
 * Function name: learncost_cmd
 * Description:   called by cmdsoul, lists currently learnable skills,
 *                their current ranks and the devpts needed to improve further
 * Arguments:     arg - skillgroup to show
 *                tp - player
 */
nomask string
learncost_cmd(string arg, object tp)
{
    int      i, j, sz, prac, theo, first, last, cost, group, gid, coder,
	     *skills;
    string   out;
    mapping  map, sk_map;

    coder = _ACCESS_LEVEL(geteuid(this_player()));
    if (!(gid = group_ids[arg]))
    {
	skills = map_indices(sk_desc);
	for (i=sizeof(skills); i--;)
	{
	    if (arg == sk_desc[j = skills[i]] &&
		(theo = tp->query_theoretical_rank_orig(j)) > 0)
		break;
	}
	if (i < 0)
	    return "This is no valid group or skill. Possible groups:\n" +
		BS(COMPOSITE_WORDS(map_values(group_names) + ({ "all" })),
		   60, 3) + ".\n";
	prac = tp->query_skill_rank_orig(j);
	cost = query_cost(prac, theo, j, tp);
	if (coder < 1)
	    out = sprintf("     %_/-28s_%_/24s\n",
			  sk_desc[j], difficulty_desc(cost, 0));
	else
	    out = sprintf("     %_/-23s_%_/4d_%_/24s\n",
			  sk_desc[j], cost, difficulty_desc(cost, 0));
	if (need_loc[j])
	    out[1] = 'l';
	if (need_obj[j])
	    out[2] = 'o';
	if (need_teach[j])
	    out[3] = 't';
	if (coder < 1)
	    out  = ("     Name                              Difficulty\n" +
		    "===================================================" +
		    "========\n" + out);
	else
	    out  = ("     Name                    Cost      Difficulty\n" +
		    "===================================================" +
		    "========\n" + out);
	return out;
    }
    /* this_player() is calling player, could be different from 'tp' */
    sk_map  = tp->query_skill_map_orig();
    map     = ([ ]);
    for (j=0, sz=sizeof(group_range[gid]); j<sz; j+=2)
    {
	first = group_range[gid][j];
	last  = group_range[gid][j + 1];
	for (i=first; i<=last; i++)
	{
	    if (!(theo = sk_map[i] / 10000))
		continue;
	    prac = sk_map[i] % 10000;
	    cost = query_cost(prac, theo, i, tp);
	    if (coder < 1)
		out = sprintf("    %_/-18s_%_/14s\n",
			      sk_desc[i], difficulty_desc(cost, 1));
	    else
		out = sprintf("    %_/-13s_%_/4d_%_/14s\n",
			      sk_desc[i], cost, difficulty_desc(cost, 1));
	    if (need_loc[i])
		out[0] = 'l';
	    if (need_obj[i])
		out[1] = 'o';
	    if (need_teach[i])
		out[2] = 't';
	    group = sk_groups[i][0];
	    if (!map[group])
		map[group] = out;
	    else
		map[group] += out;
	}
    }
    if (coder < 1)
	out  = ("    Name                   Difficulty  |" +
		"     Name                   Difficulty\n" +
		"=======================================|" +
		"======================================\n");
    else
	out  = ("    Name          Cost     Difficulty  |" +
		"     Name          Cost     Difficulty\n" +
		"=======================================|" +
		"======================================\n");
    for (i=SK_GRP_FIRST; i<=SK_GRP_LAST; i++)
	if (map[i])
	    out += (capitalize(group_names[i]) + "\n" +
		    sprintf("%  | *78s", map[i]));
    return (out + "==================================================" +
	    "============================\n          l - location needed, " +
	    "o - object needed, t - teacher needed\n");
}

/*
 * Function name: learn_cmd
 * Description:   called by cmdsoul if player wants to learn a skill
 * Arguments:     str - the string argument, the player gave to cmd learn
 * Returns:       1 if success, -1 else
 */
nomask int
learn_cmd(string str)
{
    int     i, sz, skill, theo, *skills;
    string  list, spell;
    object  tp, item, *inv;
    mapping sk_support;

    tp = this_player();
    if (!str)
    {
	tp->catch_tell("Usage: learn <skill>\n"+
		       "       learn list <spellist>\n");
	return -1;
    }
    if (str == "list" || sscanf(str, "list %s", list))
    {
	learn_spellist(list);
	return 1;
    }
    if (str == "spell" || sscanf(str, "spell %s", spell))
    {
	learn_spell(spell);
	return 1;
    }
    skills = map_indices(sk_desc);
    for (i=0, sz=sizeof(skills); i<sz; i++)
    {
	if (str == sk_desc[skill = skills[i]] &&
	    (theo = tp->query_theoretical_rank_orig(skill)) > 0)
	    break;
    }
    if (i == sz)
    {
	tp->catch_tell("You don't know anything about such a skill.\n");
	return -1;
    }
    if (IN_ARRAY(skill, need_loc))
    {
	if (!(sk_support = E(tp)->query_prop(SKILL_SUPPORT)) ||
	    !sk_support[skill])
	{
	    tp->catch_tell("You cannot train that skill here.\n");
	    return -1;
	}
	if (IN_ARRAY(skill, need_teach))
	{
	    tp->catch_tell("You need a teacher to learn that skill here.\n");
	    return -1;
	}
	if (theo < sk_support[skill])
	{
	    tp->catch_tell("You cannot train that skill further here.\n");
	    return -1;
	}
    }
    else if (IN_ARRAY(skill, need_teach) || IN_ARRAY(skill, need_obj))
    {
	inv = I(tp, 4);
	for (i=0, sz=sizeof(inv); i<sz; i++)
	    if ((sk_support = inv[i]->query_prop(SKILL_SUPPORT)) &&
		sk_support[skill] > theo)
		break;
	if (i == sz)
	{
	    tp->catch_tell("You need something or someone else to learn " +
			   "this skill further.\n");
	    return -1;
	}
	item = inv[i];
    }
    /* now we know, the skill can be learned
     * item is the object that supports learning or
     * 0 if it is the environment 
     */
    return learn_skill(skill, theo, tp, item);
}

/*
 * Function name: answer_cmd
 * Description:   evaluates the answer given to offer_teach
 * Arguments:     the answer string
 */
nomask void
answer_cmd(string answer, object tp)
{
    int     skill, modifier, tprac, ptheo, cost;
    object  teacher, item;
    mixed   prop;

    if (!(prop = tp->query_prop(LEARN_TEACH_DATA)))
	return;
    tp->add_prop(LEARN_TEACH_DATA, 0);
    
    skill   = prop[0];
    teacher = prop[1];
    if (!teacher || !CAN_HEAR(tp, teacher, 300))
    {
	tp->catch_tell("Your teacher too far away.\n");
	return;
    }
    if (answer != "yes" && answer != "y")
    {
	tp->catch_tell("Well, maybe next time.\n");
	teacher->catch_msg(({ QCTNAME(tp), " rejected your offer.\n" }), 0, 0);
	tell_objects(LISTEN(E(tp)) - ({tp, teacher}),
		     ({ QCTNAME(tp), " rejected the offer.\n" }), MSG_SEE, tp);
	return;
    }
    item = prop[2];
    tprac = teacher->query_skill_rank_orig(skill);
    ptheo = tp->query_theoretical_rank_orig(skill);
    if (tprac - 5 <= ptheo)
    {
	tp->catch_tell("Your teacher is not skilled enough.\n");
	teacher->catch_msg(({ "You are not skilled enough to teach ",
				  QTNAME(tp), " any further.\n"}), 0, 0);
	return;
    }
    if ((modifier = (tprac - ptheo) * 2) > 30)
	modifier = 30;
    cost = query_cost(tp->query_skill_rank_orig(skill), ptheo, skill, tp);
    cost = cost * (100 - modifier) / 100;
    if (tp->query_development_points() < cost)
    {
	tp->catch_tell("This skill is too difficult for now.\n");
	teacher->catch_msg(({ QCTNAME(tp), " cannot follow your " +
				  "instructions.\n"}), 0, 0);
	return;
    }
    teacher->catch_tell("Your lesson was accepted.\n");
    if (skill == SK_BODY_DEVELOP)
    {
	body_development(cost, ptheo, tp);
	return;
    }
    tp->set_theoretical_rank(skill, ptheo + 1);
    if (tp->query_skill_rank_orig(skill) == 0)
	tp->set_skill_rank(skill, 1);
    tp->reduce_development_points(cost);
    tp->catch_tell("You have improved your knowledge.\n");
}


/*
 * Function name: teach_cmd
 * Description:   called from cmdsoul, teach a skill
 * Arguments:     arg - the arguments, the player gave to this command;
 *                      must include what skill should be taught to whom
 * Return:        1 if success, -1 else
 */
nomask int
teach_cmd(string str)
{
    int      i, sz, max, skill, *skills;
    string   sk_name, whom;
    object   tp, pupil, item, *list;
    mapping  sk_support;
    
    tp = this_player();
    if (!str || sscanf(str,"%s in %s", whom, sk_name) != 2)
    {
	tp->catch_tell("Usage: teach <someone> in <skill name>\n");
	return -1;
    }
    skills = map_indices(sk_desc);
    for (i=0, sz=sizeof(skills); i<sz; i++)
    {
	if (sk_name == sk_desc[skill = skills[i]] &&
	    tp->query_theoretical_rank_orig(skill) > 0)
	    break;
    }
    if (i == sz)
    {
	tp->catch_tell("You don't know anything about such a skill.\n");
	return -1;
    }
    if (need_loc[skill])
    {
	if (!(sk_support = E(tp)->query_prop(SKILL_SUPPORT)) ||
	    !sk_support[skill])
	{
	    tp->catch_tell("You cannot teach that skill here.\n");
	    return -1;
	}
    }
    list = tp->visible(I(E(tp), 2));
    list = parse(whom, list, tp);
    if (!(sz = sizeof(list)))
    {
	tp->catch_tell("Teach whom?\n");
	return -1;
    }
    pupil = list[0];
    if (pupil == tp)
    {
	tp->catch_tell("You can't teach something to yourself.\n");
	return -1;
    }
    if (sz > 1)
    {
	tp->catch_tell("Be specific, you can't teach more than one " +
		       "person at the same time.\n");
	return -1;
    }
    if (!CAN_HEAR(pupil, tp, 300))
    {
	tp->catch_msg("You are too far away.\n");
	return -1;
    }
    if (need_obj[skill])
    {
	list = I(tp, 4);
	for (i=0, sz=sizeof(list); i<sz; i++)
	    if ((sk_support = list[i]->query_prop(SKILL_SUPPORT)) &&
		sk_support[skill] > max)
	    {
		max = sk_support[skill];
		item = list[i];
	    }
	if (!item)
	{
	    tp->catch_tell("You need something to teach this skill.\n");
	    return -1;
	}
    }
    return offer_teach(skill, pupil, tp, item);
}
