/* contains all stat related functions */

#include <access.h>
#include <config.h>
#include <magic.h>
#include <living.h>
#include <race.h>
#include "prototypes.h"

int          dev_points,      /* remaining development points */
             *tmp_stats,      /* current stats */
	     *pot_stats,      /* potential stats */
	     **tmp_stat_val,  /* The list of temporary stat values */
	     **tmp_stat_t;    /* The list of temporary stat times */

private int  *decr_time;

/*
 * Function name: stat_reset
 * Description:   set some default values, init arrays
 */
static nomask void
stat_reset()
{
    decr_time         = allocate(STAT_LAST + 1); 
    tmp_stats         = allocate(STAT_LAST + 1); 
    pot_stats         = allocate(STAT_LAST + 1); 
    tmp_stat_val      = allocate(STAT_LAST + 1);
    tmp_stat_t        = allocate(STAT_LAST + 1);
}

/*
 * Function name:   set_tmp_stat(int stat, int val)
 * Description:     Sets the value of a specific stat to a new value.
 * Arguments:       stat: The index in the stat array
 *                  val:  The value that the stat is set to
 * Returns:         The value of the stat, 0 if failed.
 */
nomask int
set_tmp_stat(int stat, int val)
{
    if (stat < STAT_FIRST || stat > STAT_LAST || val < 1 )
	return 0;
    
    tmp_stats[stat] = val;
    return val;
}  

/*
 * Function name:   query_tmp_stat()
 * Description:     Gives the value of a specific stat
 * Arguments:       stat: The index in the array of stats
 * Returns:         The value of a stat, -1 on failure.
 */
nomask int
query_tmp_stat(int stat)
{
    if (stat < 0 || stat > STAT_LAST)
	return -1;
    return tmp_stats[stat];
}
/*
 * Function name:   set_pot_stat(int stat, int val)
 * Description:     Sets the value of a specific potential stat to a new value.
 * Arguments:       stat: The index in the stat array
 *                  val:  The value that the stat is set to
 * Returns:         The value of the stat, 0 if failed.
 */
nomask int
set_pot_stat(int stat, int val)
{
    if (stat < STAT_FIRST || stat > STAT_LAST || val < 1 )
	return 0;
    
    pot_stats[stat] = val;
    return val;
}  

/*
 * Function name:   query_pot_stat()
 * Description:     Gives the value of a specific potential stat
 * Arguments:       stat: The index in the array of stats
 * Returns:         The value of a stat, -1 on failure.
 */
nomask int
query_pot_stat(int stat)
{
    if (stat < 0 || stat > STAT_LAST)
	return -1;
    return pot_stats[stat];
}

/*
 * print a message to the player that he/she feels the stat change
 * but only if the change is high enough.
 */
static nomask void
printout_tmp_change(int ds, int stat)
{
    if (stat != STAT_CO && stat != STAT_QU && stat != STAT_ST)
	return;
    if (ds > 5)
	catch_tell("You feel your " + STAT_DESC[stat] + " increasing.\n");
    else if (ds < (-5))
	catch_tell("You feel your " + STAT_DESC[stat] + " fading.\n");
}

/*
 * Function name: add_tmp_stat
 * Description:   add a temporary stat.
 * Arguments:     stat - which stat
 *                ds - change in stat
 *                dt - How many seconds to keep the change.
 * Returns:       1 - Ok, 0 - Change rejected.
 */
nomask int
add_tmp_stat(int stat, int ds, int dt)
{
    int   tmp, i, sz, *start, *end;
    mixed tmp_t, tmp_v;
        
    if (dt < 1)
	return 0;

    if (pointerp(tmp_t = tmp_stat_t[stat]))
    {
	for (i=0, sz=sizeof(tmp_t), tmp = sz-1; i<sz; i++)
	{
	    if (dt < tmp_t[i])
	    {
		tmp = i - 1;
		break;
	    }
	    dt -= tmp_t[i];
	}
	
	start = tmp_t[0..tmp];
	end = tmp_t[tmp+1..sz-1];
	if (sizeof(end))
	    end[0] -= dt;
	tmp_stat_t[stat] = start + ({ dt }) + end;
	
	tmp_v = tmp_stat_val[stat];
	start = tmp_v[0..tmp];
	end = tmp_v[tmp+1..sz-1];
	tmp_stat_val[stat] = start + ({ ds }) + end;
    }
    else
    {
	tmp_stat_val[stat] = ({ ds });
	tmp_stat_t[stat] = ({ dt });
    }
    
    printout_tmp_change(ds, stat);
    return 1;
}

/*
 * Function name: decr_tmp_stats
 * Description:   Decrement the temp stat counters and remove 
 *		  the additions as they time out.
 * Arguments:     stat - the stat that needs check
 */
static nomask void
decr_tmp_stats(int stat)
{
    int seconds;
    
    if (!decr_time[stat])
    {
	decr_time[stat] = time();
	return;
    }
    if (!(seconds = time() - decr_time[stat]))
	return;
    
    decr_time[stat] = time();
    
    if (!sizeof(tmp_stat_t[stat]))
	return;
    
    tmp_stat_t[stat][0] -= seconds;
    
    if (tmp_stat_t[stat][0] <= 0)
    {
	printout_tmp_change(-tmp_stat_val[stat][0], stat);
	tmp_stat_t[stat] = tmp_stat_t[stat][1..];
	tmp_stat_val[stat] = tmp_stat_val[stat][1..];
    }
}

/*
 * Function name: query_stat
 * Description:   Get the compound value of a stat. Never less than 1.
 * Arguments:     stat - Which stat to find.
 * Returns:       the current stat value
 */
nomask int
query_stat(int stat)
{
    int i, sz, tmp;

    if (stat < 0 || stat > STAT_LAST)
	return -1;

    decr_tmp_stats(stat);
    tmp = query_tmp_stat(stat);
    
    if (sz = sizeof(tmp_stat_val[stat]))
	for (i=0; i<sz; i++)
	    tmp += tmp_stat_val[stat][i];
    
    return (tmp > 0 ? tmp : 1);
}

/*
 * Function name: dev_points_for_stat
 * Description:   how much development for each stat
 * Arguments:     stat - the stat to query
 * Returns:       the development points
 */
nomask int
dev_points_for_stat(int stat)
{
    stat = query_stat(stat);
    
    if (stat > 100)
	return (100 + (stat-100)*5);
    switch (stat)
    {
      case    100: return 100;
      case     99: return  98;
      case     98: return  96;
      case     97: return  94;
      case     96: return  92;
		   
      case     95: return  90;
      case     94: return  89;
      case     93: return  88;
      case     92: return  87;
      case     91: return  86;
		   
      case     90: return  84;
      case 87..89: return  82;
      case 84..86: return  80;
      case 81..83: return  77;
      case 78..80: return  74;
		   
      case 75..77: return  70;
      case 72..74: return  68;
      case 68..71: return  66;
      case 64..67: return  63;
      case 60..63: return  60;
		   
      case 41..59: return  55;
		   
      case 37..40: return  50;
      case 33..36: return  48;
      case 31..32: return  46;
      case 28..30: return  43;
      case 25..27: return  40;
		   
      case 22..24: return  38;
      case 19..21: return  36;
      case 16..18: return  33;
      case 13..15: return  30;
      case     11: return  29;
		   
      case     10: return  28;
      case      9: return  27;
      case      8: return  26;
      case      7: return  24;
      case      6: return  22;
		   
      case      5: return  20;
      case      4: return  18;
      case      3: return  16;
      case      2: return  13;
      case      1: return  10;
    }
}

/*
 * Function name: calc_dev_points
 * Description:   calculates new development points after stat improvement
 */
static nomask void
calc_dev_points()
{
    int i;
    
    for (i=STAT_FIRST; i<STAT_FIRST+5; i++)
	dev_points += dev_points_for_stat(i);
}

/*
 * Function name: query_development_points
 * Description:   gives the remaining development points
 * Returns:       the dev points
 */
nomask int
query_development_points()
{
    return dev_points;
}

/*
 * Function name: reduce_development_points
 * Description:   remove some dev pts
 * Arguments:     r - how much to reduce
 */
nomask void
reduce_development_points(int r)
{
    if (r < 0)
	return;
    dev_points -= r;
}

/*
 * Function name: query_stat_race_bonus
 * Description:   gives the race bonus for a stat
 * Arguments:     stat - the stat
 * Returns:       the race bonus for the stat
 */
nomask int
query_stat_race_bonus(int stat)
{
    mixed race_data;
    
    if (!sizeof(race_data = LEARN_OB->query_race_mod(query_race())))
	return 0;
    return race_data[stat];
}

/*
 * Function name: query_stat_bonus
 * Description:   how much bonus a stat gives
 * Arguments:     stat - the stat to query
 * Returns:       the bonus
 */
nomask int
query_stat_bonus(int stat)
{
    int b;

    if (!tmp_stats[stat])
	return 0; /* npc without stats get no penalty */
    
    b = query_stat_race_bonus(stat);

    stat = query_stat(stat);
    
    if (stat > 100)
	return (b + 25 + (stat-100) * 5);
    switch (stat)
    {
      case    100: b += 25; break;
      case     99: b += 23; break;
      case     98: b += 21; break;
      case     97: b += 19; break;
      case     96: b += 17; break;
		   
      case     95: b += 15; break;
      case     94: b += 14; break;
      case     93: b += 13; break;
      case     92: b += 12; break;
      case     91: b += 11; break;
		   
      case     90: b += 10; break;
      case 87..89: b +=  9; break;
      case 84..86: b +=  8; break;
      case 81..83: b +=  7; break;
      case 78..80: b +=  6; break;
		   
      case 75..77: b +=  5; break;
      case 72..74: b +=  4; break;
      case 68..71: b +=  3; break;
      case 64..67: b +=  2; break;
      case 60..63: b +=  1; break;
		   
      case 41..59: break;
		   
      case 37..40: b -=  1; break;
      case 33..36: b -=  2; break;
      case 31..32: b -=  3; break;
      case 28..30: b -=  4; break;
      case 25..27: b -=  5; break;
		   
      case 22..24: b -=  6; break;
      case 19..21: b -=  7; break;
      case 16..18: b -=  8; break;
      case 13..15: b -=  9; break;
      case     11: b -= 10; break;
		   
      case     10: b -= 11; break;
      case      9: b -= 12; break;
      case      8: b -= 13; break;
      case      7: b -= 14; break;
      case      6: b -= 15; break;
		   
      case      5: b -= 17; break;
      case      4: b -= 19; break;
      case      3: b -= 21; break;
      case      2: b -= 23; break;
      case      1: b -= 25; break;
    }
    return b;
}

/*
 * Function name: improve_stats
 * Description:   called for every improve phase to increase the stats
 */
static nomask void
improve_stats()
{
    int   i, diff, tmp, to_add, dice, stat_detortion;
    mixed mod;

    mod = LEARN_OB->query_race_mod(query_race());
    if (sizeof(mod))
	stat_detortion = mod[STAT_DETORT];

    for (i=STAT_FIRST; i<=STAT_LAST; i++)
    {
	if ((diff = query_pot_stat(i) - (tmp = query_tmp_stat(i))) < 1)
	    continue;
	if ((dice = random(100) + 1) <= 4)
	    tmp += -dice * 2 + stat_detortion;
	else
	{
	    if ((to_add = stat_detortion + (diff + 1) * dice / 100) > 10)
		to_add = 10;
	    tmp += (to_add > diff ? diff : to_add);
	}
	if (tmp > 0)
	    set_tmp_stat(i, tmp);
    }
    reset_skill_boni();
}

/*
 * Function name: stat_living
 * Description:   gives the stats for this living
 */
nomask string
stat_living()
{
    int       stat_b, race_b, i;
    string    str, realm, *statics, *stat_desc;

    stat_desc = STAT_DESC;

    realm = ID_REALM[query_realm()];
    str = sprintf("\n Name%_/18s    File   : %-40s\n"+
		  " Race%_/18s    Euid   : %-15s\n"+
		  " Gender%_/16s    Uid    : %-40s\n"+
		  " Coderlevel_%_/11d    " +   
		  "                           Dev   --==  BONI  ==--\n" +
		  "                           StatName        Tmp  Pot  "+
		  " pts   Stat  Race Total\n                           "+
		  "=================================================\n",
		  capitalize(query_name()),
		  object_name(this_object()), 
		  capitalize(query_race()), geteuid(), 
		  capitalize(query_gender_string()), getuid(),
		  _ACCESS_LEVEL(geteuid()));

    statics =
	     ({
	     sprintf(" Realm_%_/18s",
		     (realm ? realm : "none")),
	     "                         ",
	     sprintf(" HitPts_%_/7d%_/10s", query_hp(),
		     "("+query_max_hp()+")"),
	     sprintf(" Fatigue_%_/6d%_/10s", query_fatigue(),
		     "("+query_max_fatigue()+")"),
	     sprintf(" Mana_%_/9d%_/10s", query_mana(),
		     "(" + query_max_mana() + ")"),
	     sprintf(" DevPts_%_/7d%_/10s", query_development_points(),
		     "(" + (dev_points_for_stat(STAT_CO)+
			    dev_points_for_stat(STAT_AG)+
			    dev_points_for_stat(STAT_SD)+
			    dev_points_for_stat(STAT_ME)+
			    dev_points_for_stat(STAT_RE)) + ")"),
	     "                         ",
	     "                         ",
	     sprintf(" Size_%_/15d in ", query_size()),
	     sprintf(" Volume_%_/7d%_/6s)ltr", query_volume() / 1000,
		     "("+(query_max_volume() / 1000)),
	     sprintf(" Weight_%_/7d%_/6s)kg ", query_weight() / 1000,
		     "("+(query_max_weight() / 1000)),
	     });

    for (i=STAT_CO; i<=STAT_AP; i++)
    {
	stat_b = query_stat_bonus(i);
	race_b = query_stat_race_bonus(i);

	if (i<=STAT_RE)
	{
	    str += statics[i]+"  " +
		   sprintf("%_/-14s__%_/3d__%_/3d___%_/3d___%_/3d___" +
			   "%_/3d____%_/3d\n",
			   capitalize(stat_desc[i]), query_stat(i),
			   query_pot_stat(i), dev_points_for_stat(i),
			   stat_b - race_b, race_b, stat_b);
	}
	else
	{
	    str += statics[i]+"  " +
		   sprintf("%_/-14s__%_/3d__%_/3d___xxx___%_/3d___" +
			   "%_/3d____%_/3d\n",
			   capitalize(stat_desc[i]), query_stat(i),
			   query_pot_stat(i), stat_b - race_b, race_b, stat_b);
	}
    }
    return str + "\n";
}

/*
 * Function name: stat_check
 * Description:   General statcheck method.
 * Arguments:     stat - statcode <see living.h>
 *                modifier - difficulty (negative -> harder)
 * Returns:       integer, success difference,
 *                         positive value means success
 */
nomask int
stat_check(int stat, int modifier)
{
    modifier = query_stat_bonus(stat) + roll_dice(-1) / 10 + modifier;
    return modifier;
}
