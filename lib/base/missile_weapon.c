inherit "weapon";

#include <combat.h>
#include <language.h>
#include <macros.h>
#include <properties.h>

private int     mtype,        /* kind of used missile */
                weapon_range; /* maximum distance */
private mapping missile_data; /* attack data for missile attacks */

static void
create_missile_weapon()
{
}

static nomask void
create_weapon()
{
    add_prop(IS_MISSILE_WEAPON, 1);
    create_missile_weapon();
}

/*
 * Function name: set_missile_data
 * Description:   Set the weapon data for missile attacks:
 *                skill, at modifier, ob_modifier, fumble range
 * Arguments:     sk - weapon skill
 *                am - at modifier
 *                     ({ damage mod, 5 critical thresholds,(skin...plate)
 *                       critical types of this weapon })
 *                om - base ob modifier
 *                f - fumble range
 */
nomask void
set_missile_data(int sk, mixed am, int om, int f)
{
    missile_data = ([
	A_SKILL     : sk,
	A_OBMOD     : om,
	A_DMOD      : am[0], 
	A_SKIN      : am[1], 
	A_S_LEATHER : am[2], 
	A_R_LEATHER : am[3], 
	A_CHAIN     : am[4], 
	A_PLATE     : am[5], 
	A_CRIT      : am[6], 
	A_FRANGE    : f,
	]);
}

/*
 * Function name: query_missile_data
 * Description:   give the weapon data for missile attacks
 * Returns:       mapping
 */
nomask mapping
query_missile_data()
{
    return missile_data;
}

/*
 * Function name: set_weapon_range
 * Description:   how far does this weapon shoot
 * Arguments:     r - integer range (inch)
 */
static nomask void
set_weapon_range(int r)
{
    weapon_range = r;
}

/*
 * Function name: query_weapon_range
 * Description:   gives maximum shoot range of missile weapon
 * Returns:       integer
 */
nomask int
query_weapon_range()
{
    return weapon_range;
}

/*
 * Function name: set_missile_type
 * Description:   Set the kind of missile this is
 * Arguments:     t - missile type, see <weapon.h>
 */
void
set_missile_type(int t)
{
    mtype = t;
}

/*
 * Function name: query_missile_type
 * Description:   what kind of missile is this?
 * Returns:       integer
 */
nomask int
query_missile_type()
{
    return mtype;
}

/*
 * Function name: check_missile_weapon
 * Description:   indicates that this is a missile weapon
 * Returns:       1
 */
nomask int
check_missile_weapon()
{
    return 1;
}

/*
 * Function name: check_weapon_damage
 * Description:   looks if the weapon gets damaged
 * Arguments:     tp - the player who wields the weapon       
 */
nomask void
check_weapon_damage(object tp)
{
    int damage, i;
    
    if (!(random(25)))
    {
	damage = random(2)+1;
	switch (query_prop(OBJ_DAMAGED)+damage)
	{
	  case 0..60:
	      i=1;
	      break;
	  case 61..70:
	      i=5;
	      break;
	  case 71..76:
	      i=8;
	      break;
	  case 77..83:
	      i=10;
	      break;
	  case 84..89:
	      i=13;
	      break;
	  case 90..95:
	      i=18;
	      break;
	  case 96..99:
	      i=22;
	      break;
	  default:
	      i=30;
	      break;
	}
	if (random(i) <= i)
	{
	    tp->catch_tell("After your shot, your bow breaks.\n");
	    return;
	}
	change_int_prop(OBJ_DAMAGED, damage);
    }
}
