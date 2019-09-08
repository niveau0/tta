inherit "object";
inherit "misc/wear";

#include <event.h>
#include <macros.h>
#include <properties.h>

private int atype; /* armor type */
private int *protection; /* armor protection locations */

/*
 * Function name: create_armor
 * Description:   create the armor.
 */
static void
create_armor()
{
}

/*
 * Function name: create_object
 * Description:   Create the armor. (constructor)
 */
static nomask void
create_object()
{
    set_name("armor");
    add_prop(IS_WEARABLE, 1);
    add_prop(IS_ARMOR, 1);
    create_armor();
    add_event(E_GOT_HIT, "E_got_hit", TO());
    add_reload("reload_armor", "init_armor");
    if (!protection)
	protection = query_locations();
}

/*
 * Function name: set_protection
 * Description:   Set the locations the armor protects
 * Arguments:     locs - integer array, the protection-slots
 */
static nomask void
set_protection(int *p)
{
    protection = p;
}

/*
 * Function name: query_protection
 * Description:   Gives the locations which are protected by this armor
 * Returns:       integer array, the protection-slots
 */
nomask int *
query_protection()
{
    return protection;
}

/*
 * Function name: set_at
 * Description:   Set at value
 * Arguments:     at - the armor type (equals weapon data type)
 */
static nomask void
set_at(int at)
{
    atype = at;
}

/*
 * Function name: init_armor
 * Description:   initialize stored data of the armor
 * Arguments:     data - array with reloading data
 */
nomask void
init_armor(mixed data)
{
    add_prop(OBJ_DAMAGED, (int)data);
}

/*
 * Function name: reload_armor
 * Description:   give the data array to reload for an armor
 * Returns:       data values to restore
 */
nomask mixed 
reload_armor()
{
    return query_prop(OBJ_DAMAGED);
}

/*
 * Function name: query_at
 * Description:   gives the armor type 
 * Returns:       int, at code
 */
nomask int
query_at()
{
    return atype;
}

/*
 * Function name: check_armor
 * Description:   helps to determine if object is an armor
 * Returns:       1 if object is an armor
 */
nomask int
check_armor()
{
    return 1;
}


/*
 * Function name: E_got_hit
 * Description:   notify hit on this armor
 * Arguments:     o - object that made the hit
 *                ao - object that has been hit
 *                damage - the inflicted damage
 *                slot - the slot that has been hit
 *                w - the object that caused the hit
 */

nomask void
E_got_hit(object o, object ao, int damage, int slot, object w)
{
    int i;

    if(!TO() || damage < 1 || query_prop(OBJ_NEVER_BREAK))
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
    if (random(100) <= i)
    {
        add_prop(OBJ_DAMAGED, -1);
        debug("aule", ({ "Armor zerstoert:", i }));
        ao->catch_tell("Oops. The last hit was too much for your armor.\n");
        return;
    }
    debug("aule", ({ "armor damaged:", i }));
    change_int_prop(OBJ_DAMAGED, i);
}

