/* contains the magic related functions for a living */

#include <combat.h>
#include <living.h>
#include <magic.h>
#include <material.h>
#include <mudtime.h>
#include <rr_types.h>
#include <skills.h>
#include <slots.h>
#include <properties.h>
#include "prototypes.h"

int         mana,            /* The spell points of this lifeform */
	    realm;           /* realm of magic */

private int mana_time;

/*
 * Function name: magic_reset
 * Description:   set some default values, init arrays
 */
static nomask void
magic_reset()
{
    mana_time = time();
}

/*
 * Function name: set_realm
 * Description:   set the realm the living is assigned to
 * Arguments:     r - the realm code
 * Returns:       none
 */
nomask void
set_realm(int r)
{
    realm = r;
}

/*
 * Function name: query_realm
 * Description:   gives the realm code
 * Returns:       the realm code
 */
nomask int
query_realm()
{
    return realm;
}

/*
 * Function name: query_realm_stat
 * Description:   get the responsible stat for our realm
 * Returns:       array of stats
 */
nomask int *
query_realm_stats()
{
    switch(query_realm())
    {
      case CHA:
	return ({ STAT_IN });
      case MEN:
	return ({ STAT_PR });
      case ESS:
	return ({ STAT_EM });
      case (CHA | MEN):
	return ({ STAT_IN, STAT_PR });
      case (CHA | ESS):
	return ({ STAT_IN, STAT_EM });
      case (MEN | ESS):
	return ({ STAT_PR, STAT_EM });
      case (ARC):
        return ({ STAT_IN, STAT_PR, STAT_EM });
    }
}

/*
 * Function name: mana_stat_value
 * Description:   how much mana to get for each skill rank
 */
nomask int
mana_stat_value()
{
    int i, sz, stat, *realm_stats;
  
    if (!(sz = sizeof(realm_stats = query_realm_stats())))
	return 0;

    for (i=0; i < sz; i++)
	stat += query_stat(realm_stats[i]);
    stat /= sz;

    if (stat > 100)
	return (stat + (stat-100) * 5);
    if (stat <  60)
	return 0;
    switch (stat)
    {
      case    100: return 25;
      case     99: return 23;
      case     98: return 21;
      case     97: return 19;
      case     96: return 17;
		   
      case     95: return 15;
      case     94: return 14;
      case     93: return 13;
      case     92: return 12;
      case     91: return 11;
		   
      case     90: return 10;
      case 87..89: return  9;
      case 84..86: return  8;
      case 81..83: return  7;
      case 78..80: return  6;
		   
      case 75..77: return  5;
      case 72..74: return  4;
      case 68..71: return  3;
      case 64..67: return  2;
      case 60..63: return  1;
    }
}

/*
 * Function name:   query_max_mana()
 * Description:     Calculates the maximum of mana points
 * Returns:         integer
 */
nomask int
query_max_mana()
{
    return query_skill_rank(SK_MANA_DEVELOP) * mana_stat_value() +
	query_prop(LIVE_MANA_MULTIPLY) + 1;
}

/*
 * Function name:   set_mana(int sp)
 * Description:     Set mana points that a living has (up to double of maximum)
 * Arguments:       sp - The new amount of mana points.
 */
static nomask void
set_mana(int sp)
{
    int max;

    max = query_max_mana() * 2;
    if (sp < 0)
	mana = 0;
    else if (sp < max)
	mana = sp;
    else
	mana = max;
}

/*
 * Function name:   add_mana(int sp)
 * Description:     Add a certain amount of mana points (up to double maximum)
 * Arguments:       sp - The number of mana points to change.
 */
nomask void
add_mana(int sp)
{
    set_mana(mana + sp);
}

/*
 * Function name:   query_mana()
 * Description:     Gives the number of mana points that the living has
 * Returns:         The number of mana points.
 */
nomask int
query_mana()
{
    int h, add, max;

    if (!mana_time)
    {
	mana_time = time();
	return mana;
    }
    if ((h = time() - mana_time) < 10 || !living(this_object()))
	return mana;
    add = query_max_mana() / 16;
    if (add < 1)
	add = 1;
    /* add mana each 'mud' hour */
    h = h * add * TIME_SCALE / 3600;
    if (!h) return mana;
    mana_time += h * 3600 / (add * TIME_SCALE);
    if (mana < (max = query_max_mana()))
    {
	if (mana + h > max)
	    mana = max;
	else
	    mana += h;
    }
    return mana;
}

/*
 * Function name: spell_resistance(int attack_type, int attack_level, int roll,
 *                                 int rr_mod)
 * Description:   evaluate resistance against spells
 * Arguments:     attack_type - spell type, see <rr_types.h>
 *                attack_level - spell level
 *                roll - dice roll of attacker
 *                rr_mod - resistance modifier (positive: easier to resist)
 * Returns:       resistance_roll result
 */
nomask varargs int
spell_resistance(int attack_type, int attack_level, int roll, int rr_mod)
{
    int    i, sz, mat, failure;
    object *obj;
    
    if (roll < 3) /* failure */
	return 1;

    switch (attack_type)
    {
      case RR_CHA:
	if (sz = sizeof(obj = query_worn(SLT_CHEST + 1)))
	{
	    for (i=0; i<sz; i++)
		if ((mat = obj[i]->query_prop(OBJ_MATERIAL)) >= M_METAL)
		{
		    failure = 25;
		    break;
		}
		else if (mat == M_LEATHER)
		    failure = 17;
	}
	break;
      case RR_ESS:
	if (sz = sizeof(obj = query_worn(SLT_CHEST + 1)))
	{
	    for (i=0; i<sz; i++)
		if ((mat = obj[i]->query_prop(OBJ_MATERIAL)) >= M_METAL)
		{
		    failure = 17;
		    break;
		}
		else if (mat == M_LEATHER)
		    failure = 13;
	}
	break;
      case RR_MEN:
	if (sz = sizeof(obj = query_worn(SLT_HEAD + 1)))
	{
	    for (i=0; i<sz; i++)
		if ((mat = obj[i]->query_prop(OBJ_MATERIAL)) >= M_METAL)
		{
		    failure = 25;
		    break;
		}
		else if (mat == M_LEATHER)
		    failure = 13;
	}
	break;
    }
    
    roll += attack_level;
    if (roll < failure)
	return 1;
    
    roll = -(roll - 50) / 4 * 5 + rr_mod;
    
    return resistance_roll(attack_type, attack_level, roll);
}
