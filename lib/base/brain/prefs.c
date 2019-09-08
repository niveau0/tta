#include <combat.h>
#include <brain.h>
#include <event.h>
#include <macros.h>
#include <skills.h>
#include <properties.h>

private void appraise_incoming_melee_weapon(object ob);
private void search_new_melee_weapon(object ob);
private void appraise_incoming_range_weapon(object ob);
private void search_new_range_weapon(object ob);
object query_pref_range_weapon();
object query_pref_melee_weapon();

private mapping prefs;

static nomask void
brain_init_prefs()
{
    prefs = ([ ]);
    search_new_melee_weapon(prefs[PREF_MELEE_WEAPON]);
    search_new_range_weapon(prefs[PREF_RANGE_WEAPON]);
    add_event(E_MOVE_INV, "brain_prefs_notify", this_object());
}
 
nomask void
brain_prefs_notify(object ob, object from, object to)
{
    int i, sz;
    object *invent;
    if (!ob)
	return;

    if (!living(this_object()) || from == to)
	return;

    if ((E(from) == this_object()) && (to == this_object()))
	return;

    if ((E(to) == this_object()) && (from == this_object()))
	return;
    
    if(to == this_object() || E(to) == this_object())
    {
	if (ob->query_prop(IS_WIELDABLE))
	{
	    if (ob->query_prop(IS_MISSILE_WEAPON))
	    {
		appraise_incoming_range_weapon(ob);
		return;
	    }
	    else
	    {
		appraise_incoming_melee_weapon(ob);
		return;
	    }
	}

	if (to == this_object() && ob->query_prop(IS_CONT))
	    add_event(E_MOVE_INV, "brain_prefs_notify", ob);

	if (sz=sizeof(invent = ob->deep_inventory(1)))
	{
	    for (i=sz; i--;)
	    {
		if (invent[i]->query_prop(IS_WEAPON))
		{
		    if (invent[i]->query_prop(IS_MISSILE_WEAPON))
			appraise_incoming_range_weapon(invent[i]);
		    else
			appraise_incoming_melee_weapon(invent[i]);
		}
	    }
	    return;
	}
    }
    else if (from == this_object() || E(from) == this_object())
    {
	if (ob == prefs[PREF_MELEE_WEAPON])
	{
	    search_new_melee_weapon(ob);
	    return;
	}

	else if (ob == prefs[PREF_RANGE_WEAPON])
	{
	    search_new_range_weapon(ob);
	    return;
	}

	else if (ob->query_prop(IS_CONT))
	    remove_event(E_MOVE_INV, "brain_prefs_notify", ob);
	
	if (sz=sizeof(invent = ob->deep_inventory(1)))
	{
	    for (i=sz; i--;)
	    {
		if(invent[i] == prefs[PREF_MELEE_WEAPON])
		{
		    search_new_melee_weapon(invent[i]);
		    return;
		}
		else if (invent[i] == prefs[PREF_RANGE_WEAPON])
		{
		    search_new_range_weapon(invent[i]);
		    return;
		}
	    }
	}
    }
}

private void
appraise_incoming_melee_weapon(object ob)
{
    object to;

    to = this_object();
	
    if (!prefs[PREF_MELEE_WEAPON])
    {
	prefs[PREF_MELEE_WEAPON] = ob;
	debug("aule","new melee weapon(1a):");
	debug("aule",prefs[PREF_MELEE_WEAPON]);
	return;
    }
    
    if (to->query_skill_rank(ob->query_data()[A_SKILL]) >
	to->query_skill_rank(prefs[PREF_MELEE_WEAPON]->query_data()[A_SKILL]))
    {
	prefs[PREF_MELEE_WEAPON] = ob;
	debug("aule","new melee weapon(1b):");
	debug("aule",prefs[PREF_MELEE_WEAPON]);
	return;
    }
    debug("aule","old melee weapon(1c):");
    debug("aule",prefs[PREF_MELEE_WEAPON]);
}

private void
search_new_melee_weapon(object ob)
{
    int sz, i, skill, player_skill;
    object *invent, to;
    mapping weapons;

    to = this_object();
    
    if (prefs[PREF_MELEE_WEAPON] != ob)
	return;

    if (!mappingp(weapons))
	weapons=([]);
       
    invent = to->deep_inventory(1);

    if(!(sz=sizeof(invent)))
    {
	prefs[PREF_MELEE_WEAPON] = 0;
	debug("aule","no melee weapon:(2a)");
	debug("aule",prefs[PREF_MELEE_WEAPON]);
	return;
    }

    for (i=sz; i--;)
    {
	if (invent[i]->query_prop(IS_WIELDABLE))
	{
	    skill = invent[i]->query_data()[A_SKILL];

	    if (!(invent[i]->query_prop(IS_MISSILE_WEAPON)))
	    {
		player_skill = to->query_skill_rank(skill);
		/* HIER weitere Werte abfragen!
		   waffenloser Kampf ? */
		if(weapons[player_skill])
		    weapons[player_skill] += ({ invent[i] });
		else
		    weapons[player_skill] = ({ invent[i] });
	    }
	}
    }
    
    if(!(map_sizeof(weapons)))
    {
	prefs[PREF_MELEE_WEAPON] = 0;
	debug("aule","no melee weapon:(2b)");
	debug("aule",prefs[PREF_MELEE_WEAPON]);
	return;
    }
    
    invent = map_indices(weapons);

    sz=sizeof(invent);
    
    i = map_indices(weapons)[sz-1];

    prefs[PREF_MELEE_WEAPON] = weapons[i][0];
    debug("aule","new melee weapon:(2c)");
    debug("aule",prefs[PREF_MELEE_WEAPON]);
}

/* nach PFEILEN ueberpruefen !!!! */

private void
appraise_incoming_range_weapon(object ob)
{
    object to;

    to = this_object();
	
    if (!prefs[PREF_RANGE_WEAPON])
    {
	prefs[PREF_RANGE_WEAPON] = ob;
	debug("aule","new range weapon:(1a)");
	debug("aule",prefs[PREF_RANGE_WEAPON]);
	return;
    }
    
    if (to->query_skill_rank(ob->query_weapon_skill()) >
	to->query_skill_rank(prefs[PREF_RANGE_WEAPON]->query_weapon_skill()))
    {
	prefs[PREF_RANGE_WEAPON] = ob;
	debug("aule","new range weapon:(1b)");
	debug("aule",prefs[PREF_RANGE_WEAPON]);
	return;
    }
    debug("aule","old range weapon:(1c)");
    debug("aule",prefs[PREF_RANGE_WEAPON]);
}

private void
search_new_range_weapon(object ob)
{
    int sz, i, skill, player_skill;
    object *invent, to;
    mapping weapons;

    to = this_object();
    
    if (prefs[PREF_RANGE_WEAPON] != ob)
	return;

    if (!mappingp(weapons))
	weapons=([]);
       
    invent = to->deep_inventory(1);

    if(!(sz=sizeof(invent)))
    {
	prefs[PREF_RANGE_WEAPON] = 0;
	debug("aule","no range weapon:(2a)");
	debug("aule",prefs[PREF_RANGE_WEAPON]);
	return;
    }

    for (i=sz; i--;)
    {
	if (invent[i]->query_prop(IS_WIELDABLE))
	{
	    skill = invent[i]->query_weapon_skill();

	    if (invent[i]->query_prop(IS_MISSILE_WEAPON))
	    {
		player_skill = to->query_skill_rank(skill);
		/* HIER weitere Werte abfragen! 
		   anzahl missiles ? */
		if(weapons[player_skill])
		    weapons[player_skill] += ({ invent[i] });
		else
		    weapons[player_skill] = ({ invent[i] });
	    }
	}
    }
    
    if(!(map_sizeof(weapons)))
    {
	prefs[PREF_RANGE_WEAPON] = 0;
	debug("aule","no range weapon:(2b)");
	debug("aule",prefs[PREF_RANGE_WEAPON]);
	return;
    }
    
    invent = map_indices(weapons);

    sz=sizeof(invent);
    
    i = map_indices(weapons)[sz-1];

    prefs[PREF_RANGE_WEAPON] = weapons[i][0];
    debug("aule","new range weapon:(2c)");
    debug("aule",prefs[PREF_RANGE_WEAPON]);
}

object
query_pref_range_weapon()
{
    return prefs[PREF_RANGE_WEAPON];
}

object
query_pref_melee_weapon()
{
    return prefs[PREF_MELEE_WEAPON];
}
