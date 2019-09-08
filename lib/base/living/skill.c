/* contains all skill related functions for a living */

#include <access.h>
#include <combat.h>
#include <composite.h>
#include <config.h>
#include <macros.h>
#include <properties.h>
#include <skills.h>
#include "prototypes.h"

mapping         skill_rank;      /* how often skill was learned */
private mapping skill_boni;      /* calculated skill boni */

/*
 * Function name: reset_skill_boni
 * Description:   resets all skill boni for new calculation
 */
nomask void
reset_skill_boni()
{
    skill_boni = ([ ]);
}

/*
 * Function name: skill_reset
 * Description:   set some default values, init arrays
 */
static nomask void 
skill_reset()
{
    skill_boni = ([ ]);
    skill_rank = ([ ]);
}

/*
 * Function name: add_tmp_skill
 * Description:   add a skill bonus/malus to skill rank
 * Arguments:     skill - which skill to change
 *                v - the value 
 */
nomask void
add_tmp_skill(int skill, int v)
{
    mapping tmp;
    
    if (!mappingp(tmp = query_prop(LIVE_TMP_SKILLS)))
	tmp = ([ ]);
    tmp[skill] += v;
    add_prop(LIVE_TMP_SKILLS, tmp);
}

/*
 * Function name:   set_skill_rank(int skill, int val)
 * Description:     Set a specific skillrank to a specific value.
 * Arguments:       skill - The skill-number to set
 *                  val - The value to set the skillrank to.
 */
nomask void
set_skill_rank(int skill, int val)
{
    int old;

    old = skill_rank[skill] % 10000;

    skill_rank[skill] = (skill_rank[skill] - old) + val;
    skill_boni[skill] = 0; /* force new calculation */
}

/*
 * Function name:   query_skill_rank()
 * Description:     Gives the value of a specific skillrank.
 * Arguments:       skill - The number of the skill to check
 * Returns:         The value of the skill
 */
nomask int
query_skill_rank(int skill)
{
	if (!mappingp(skill_rank))
		return 0;
    if (mappingp(query_prop(LIVE_TMP_SKILLS)))
	return skill_rank[skill] % 10000 + query_prop(LIVE_TMP_SKILLS)[skill];
    return skill_rank[skill] % 10000;
}

/*
 * Function name:   query_skill_rank_orig()
 * Description:     Gives the value of a specific skillrank without temporal
 *                  changes.
 * Arguments:       skill - The number of the skill to check
 * Returns:         The value of the skill
 */
nomask int
query_skill_rank_orig(int skill)
{
    return skill_rank[skill] % 10000;
}

/*
 * Function name: query_skill_map_orig
 * Description:   Gives the whole unmodified skill map
 * Returns:       mapping
 */
nomask mapping
query_skill_map_orig(int skill)
{
    return skill_rank;
}

/*
 * Function name:   set_theoretical_rank(int skill, int val)
 * Description:     Set a specific theoretical skillrank to a specific value.
 * Arguments:       skill - The skill-number to set
 *                  val - The value to set the skillrank to.
 */
nomask void
set_theoretical_rank(int skill, int val)
{
    int old;

    old = skill_rank[skill] % 10000;

    skill_rank[skill] = old + val * 10000;
}

/*
 * Function name:   query_theoretical_rank()
 * Description:     Gives the value of a specific theoretical skillrank.
 * Arguments:       skill: The number of the skill to check
 * Returns:         The value of the skill
 */
nomask int
query_theoretical_rank(int skill)
{
    return skill_rank[skill] / 10000;
}

/*
 * Function name:   query_theoretical_rank_orig()
 * Description:     Gives the value of a specific theoretical skillrank
 *                  without temporal changes.
 * Arguments:       skill - The number of the skill to check
 * Returns:         The value of the skill
 */
nomask int
query_theoretical_rank_orig(int skill)
{
    return skill_rank[skill] / 10000;
}

/*
 * Function name: query_skill_bonus
 * Description:   how much bonus for a skill
 * Arguments:     skill - the skill 
 * Returns:       the bonus for the skill
 */
nomask int
query_skill_bonus(int skill)
{
    int mod;
    
    if (!skill_boni[skill])
	calc_skill_bonus(skill);
    if (mod = cb_query_bonus(CB_ACTION_MOD))
	return skill_boni[skill] * (100 + mod) / 100;
    return skill_boni[skill]; 
}

/*
 * Function name: calc_skill_bonus
 * Description:   calculates the new skill bonus if skill has changed
 * Arguments:     skill - the skill
 */
nomask void
calc_skill_bonus(int skill)
{
    int      i, sz, rank, rank_bonus, stat_bonus;
    mixed    attrib;

    if (!(rank = query_skill_rank(skill)))
    {
	skill_boni[skill] = 0;
	return;
    }
    else if (rank > 30)
	/* rank_bonus = 80 + (rank - 30) * 0.5 */
	rank_bonus = 65 + rank / 2;
    else if (rank > 20)
	/* rank_bonus = 70 + (rank - 20) * 1 */
	rank_bonus = 50 + rank;
    else if (rank > 10)
	/* rank_bonus = 50 + (rank - 10) * 2 */
	rank_bonus = 30 + rank + rank;
    else if (rank > 0)
	rank_bonus = rank * 5;
    
    attrib = LEARN_OB->query_skill_attrib(skill);

    if (stringp(attrib)) /* attrib contains a function */
	attrib = call_other(this_object(), attrib);  

    if (sz = sizeof(attrib))
    {
	for (i=0; i<sz; i++)
	    stat_bonus += query_stat_bonus(attrib[i]);
	stat_bonus /= sz;
    }

    skill_boni[skill] = (stat_bonus + rank_bonus);
}

/*
 * Function name: improve_resistance
 * Description:   raise the resistance skills after raising a 'level'
 *                (see /base/player.c autosave)
 */
static nomask void
improve_resistance()
{
    int i, sk;

    for (i=SK_RESIST_FIRST; i<=SK_RESIST_LAST; i++)
    {
	sk = query_theoretical_rank_orig(i) + 1;
	set_skill_rank(i, sk);
	set_theoretical_rank(i, sk);
    }
    sk = query_theoretical_rank_orig(SK_MANA_DEVELOP) + 1;
    set_skill_rank(i, sk);
    set_theoretical_rank(i, sk);
}

/*
 * Function name: skill_living
 * Description:   show all learned skills with their boni
 * Arguments:     arg - the name of one group or 'all' for all groups
 * Returns:       string of the requested skillgroups and their boni
 */
nomask string
skill_living(string arg)
{
    int       i, j, sz, theo, prac, first, last, group_id, coder;
    string    out, desc;
    mapping   sk_desc, sk_groups, group_range, group_names, map;

    group_names = LEARN_OB->query_group_names();
    if (!(group_id = LEARN_OB->query_group_id(arg)))
	return "This is no valid group. Possible:\n" +
	    BS(COMPOSITE_WORDS(map_values(group_names) + ({ "all" })),
	       60, 3) + ".\n";

    sk_desc     = SK_DESC;
    sk_groups   = LEARN_OB->query_skill_groups();
    group_range = LEARN_OB->query_group_range();
    coder       = _ACCESS_LEVEL(geteuid(this_player()));
    map   = ([ ]);
    for (j=0, sz=sizeof(group_range[group_id]); j<sz; j+=2)
    {
	first = group_range[group_id][j];
	last  = group_range[group_id][j + 1];
	for (i=first; i<=last; i++)
	{
	    theo = query_theoretical_rank(i);
	    prac = query_skill_rank(i);
	    if (!theo && !prac)
		continue;
	    if (!(desc = sk_desc[i]))
	    {
		set_skill_rank(i, 0);
		set_theoretical_rank(i, 0);
		continue;
	    }
	    if (theo < prac)
	    {
		set_theoretical_rank(i, prac);
		theo = prac;
	    }
	    if (coder > 0)
		out = sprintf("  %_/-18s_%_/4d_%_/4d_%_/6d\n",
			      desc, prac, theo, query_skill_bonus(i));
	    else
	    {
		theo = ((theo - prac) / 5 + prac);
		out = sprintf("  %_/-28s_%_/6d\n", desc, theo);
	    }
	    if (!map[sk_groups[i][0]])
		map[sk_groups[i][0]] = out;
	    else
		map[sk_groups[i][0]] += out;
	}
    }
    if (coder > 0)
	out = ("  Name               Rank Theo  Bonus  |"+
	       "  Name               Rank Theo  Bonus\n"+
	       "=======================================|"+
	       "=====================================\n");
    else
	out = ("  Name                           Rank  |"+
	       "  Name                           Rank\n"+
	       "=======================================|"+
	       "=====================================\n");
    for (i=SK_GRP_FIRST; i<=SK_GRP_LAST; i++)
	if (map[i])
	    out += capitalize(group_names[i]) + "\n" +
		sprintf("%  |*78s", map[i]);
    return out;
}

/*
 * Function name: skill_check
 * Description:   General skillcheck method.
 * Arguments:     skill - skillcode
 *                modifier - difficulty (negative -> harder)
 *                countUsage - if 1  skill usage is stored if success
 *                             if 0  skill usage is always stored
 *                             if -1 skill usage is stored if failed
 * Returns:       integer, success difference,
 *                         positive value means success
 */
nomask int
skill_check(int skill, int modifier, int countUsage)
{
    modifier = query_skill_bonus(skill) + roll_dice(-1) + modifier;
    if (!countUsage)
	this_object()->skill_usage(skill);
    else if (modifier >= 0 && countUsage == 1)
	this_object()->skill_usage(skill);
    else if (modifier < 0 && countUsage == -1)
	this_object()->skill_usage(skill);
    return modifier;
}
