inherit "object";

#include <combat.h>
#include <event.h>
#include <macros.h>
#include <properties.h>

private int     hands,    /* maximum number of hands that wield */
                slot_vol; /* wield volume */
static  object  wielder;  /* who is using this weapon */
private mapping data;     /* weapon data (see set_data) */

/*
 * Function name: create_weapon
 * Description:   create the weapon
 */
static void
create_weapon()
{
}

/*
 * Function name: create_object
 * Description:   standard constructor
 */
static nomask void
create_object()
{
    set_name("weapon");
    add_prop(IS_WIELDABLE, 1);
    add_prop(IS_WEAPON, 1);
    create_weapon();
    add_event(E_MOVE, "E_move", this_object());
    add_prop(OBJ_ATTACH_SIZE, 2);
    add_reload("reload_weapon", "init_weapon");
}

/*
 * Function name: set_data
 * Description:   Set the weapon data:
 *                skill, at modifier, ob_modifier, fumble range
 * Arguments:     sk - weapon skill
 *                am - at modifier
 *                     ({ damage mod, 5 critical thresholds,(skin...plate)
 *                       critical types of this weapon })
 *                om - base ob modifier
 *                f - fumble range
 */
nomask void
set_data(int sk, mixed am, int om, int f)
{
    data = ([
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
 * Function name: query_data
 * Description:   give the weapon data
 * Returns:       mapping
 */
nomask mapping
query_data()
{
    return data;
}

/*
 * Function name: set_wielder
 * Description:   Set the wielder of this object
 * Arguments:     w - The wielder object
 */
void
set_wielder(object w)
{
    wielder = w;
    if (w)
	add_event(E_DID_HIT, "E_did_hit", w);
    else if (wielder)
	remove_event(E_DID_HIT, "E_did_hit", wielder);
}

/*
 * Function name: query_wielder
 * Description:   If this object is wielded or not
 * Returns:       object who wields this object, else 0
 */
nomask object
query_wielder()
{
    return wielder;
}

/*
 * Function name: set_wield_size
 * Description:   set the volumes this weapon occupy if wielded
 * Arguments:     sz - the wield volume weapon needs
 *                h - maximum number of hand that could be used
 *                    negative: force more handed (e.g. longbow: -2) 
 */
nomask int
set_wield_size(int sz, int h)
{
    slot_vol = sz;
    hands    = h;
}

/*
 * Function name: init_weapon
 * Description:   initialize stored data of the weapon
 * Arguments:     data - array with reloading data
 */
nomask void
init_weapon(mixed data)
{
    add_prop(OBJ_DAMAGED, data);
}

/*
 * Function name: reload_weapon
 * Description:   give the data array to reload for an weapon
 * Returns:       data values to restore
 */
nomask mixed
reload_weapon()
{
    return query_prop(OBJ_DAMAGED);
}

/*
 * Function name: query_wield_size
 * Description:   how much volume does this weapon occupy and how many hands
 *                may be used
 * Returns:       integer array
 */
nomask int *
query_wield_size()
{
    return ({ slot_vol, hands });
}

/*
 * Function name: E_move
 * Description:   notify movement
 * Arguments:     ob - object that moved
 *                from - old environment
 *                to - new environment
 */
nomask void
E_move(object ob, object from, object to)
{
    if (!wielder || ob != this_object() || wielder == to)
	return;
    set_wielder(0);
}

/*
 * Function name: E_did_hit
 * Description:   notify every hit
 * Arguments:     o - object that made the hit
 *                ao - object that has been hit
 *                damage - the inflicted damage
 *                slot - the slot that has been hit
 *                w - the object that caused the hit
 */
nomask void
E_did_hit(object o, object ao, int damage, int slot, object w)
{
    int i, j;
    
    if (!o || w != this_object() || damage < 1 || query_prop(OBJ_NEVER_BREAK))
	return;

    switch (damage)
    {
      case 1..6:
	  i = 1; break;
      case 7..11:
	  i = 2; break;
      case 12..16:
	  i = 3; break;
      case 17..20:
	  i = 4; break;
      case 21..23:
	  i = 5; break;
      case 24..25:
	  i = 6; break;
      default:
	  i = 8; break;
    }
    i = random(100) - i;
    switch (i)
    {
      case 0:
	  i = 1; break;
      case -1:
	  i = 3; break;
      case -2:
	  i = 5; break;
      case -3:
	  i = 7; break;
      case -4:
	  i = 8; break;
      case -5:
	  i = 10; break;
      case -6:
	  i = 12; break;
      case -7:
	  i = 15; break;
      default:
	  return;
    }
    i = query_prop(OBJ_DAMAGED) + random(i) + 1;
    switch (i)
    {
      case 0..40:
          i = 1; break;
      case 41..60:
	  i = 5; break;
      case 61..70:
          i = 10; break;
      case 71..78:
          i = 13; break;
      case 79..85:
          i = 16; break;
      case 86..90:
          i = 20; break;
      case 91..93:
          i = 22; break;
      case 94..96:
          i = 25; break;
      case 97..98:
          i = 27; break;
      case 99:
          i = 30; break;
      default:
          i = 35 + random(100); break;
    }
    if (random(100) <= j)
    {
	add_prop(OBJ_DAMAGED, -1);
	debug("aule", ({ "Waffe zerstoert:", i }));
	o->catch_tell("Oops. The last hit was too much for your weapon.\n");
	return;
    }
    debug("aule", ({ "weapon damaged:", i }));
    change_int_prop(OBJ_DAMAGED, i);
}
