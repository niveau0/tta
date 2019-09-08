/*
 * There is a limited number of slots for a living and one slot can only be
 * occupied by a limited number of objects at a time. 
 */
inherit "/base/misc/subloc";

#include <composite.h>
#include <event.h>
#include <language.h>
#include <macros.h>
#include <material.h>
#include <slots.h>
#include <properties.h>
#include "prototypes.h"

private mapping wield,       /* all wielded items */
                worn,        /* all worn items */
                hold,        /* all held items */
                slot_sizes,  /* The sizes of the slots */
                slot_descs,  /* The descriptions of the slots */
                slot_types,  /* How many slots of a type, e.g. hands */
                reconnect,   /* connections reverse for healing checks */
                connections; /* all limbs cut together in fight */
			     
mapping         wounds,      /* the smashed or lost slots */ 
                bandaged;    /* all bandaged wounds */ 

/*
 * Function name: slot_reset
 * Description:   Configure slot variables
 */
static nomask void
slot_reset()
{
    init_subloc();
    
    wield       = ([]); /* used to speed up collection */
    worn        = ([]); /* used to speed up collection */
    hold        = ([]); /* used to speed up collection */
    slot_sizes  = ([]);
    slot_descs  = ([]);
    slot_types  = ([]);
    wounds      = ([]);
    bandaged    = ([]);
    connections = ([]);
    reconnect   = ([]);
}

/*
 * Function name: set_connections
 * Description:   sets the connections between limbs
 * Arguments:     c - mapping ([ HITLOC: ({ CONNECTIONS }) ])
 */
static nomask void
set_connections(mapping c)
{
    int i, j, *ind, *slots;
    
    connections = c;
    reconnect = ([]);
    ind = map_indices(c);
    for (i=sizeof(ind); i--;)
    {
	slots = c[ind[i]];
	for (j=sizeof(slots); j--;)
	    reconnect[slots[j]] = ind[i];
    }
}

/*
 * Function name: query_reconnect
 * Description:   get the reconnection to a slot
 * Arguments:     slot - the slot
 */
nomask int
query_reconnect(int slot)
{
    return reconnect[slot];
}

/*
 * Function name: query_connections
 * Description:   get the connections to a slot
 * Arguments:     slot - the slot
 */
nomask int *
query_connections(int slot)
{
    return connections[slot];
}

/*
 * Function name: slot_space
 * Description:   return the free size of the slot
 * Arguments:	  slot - which slot to ask
 *                tool - an object that shouldn't count
 * Returns:       the left size
 */
nomask varargs int
slot_space(int slot, object tool)
{
    int    i, used;
    object *obj;
    
    if (!slot_sizes[slot] || (wield[slot] && wield[slot] != tool))
	return 0;

    obj = query_hold(slot);
    obj -= ({ tool });
    for (i=sizeof(obj); i--;)
	used += obj[i]->query_volume();

    obj = query_worn(slot);
    obj -= ({ tool });
    for (i=sizeof(obj); i--;)
	used += obj[i]->query_slot_volume();
    return slot_sizes[slot] - used;
}

/*
 * Function name: add_tool
 * Description:   puts a tool into a slot, no matter if occupied
 * Arguments:	  tool - the tool object
 *                slot - which slot to use
 * Returns:       move error code, 0 if success
 */
nomask int
add_tool(object tool, int slot)
{
    int    err;
    object split;
    
    if (!tool || !slot_sizes[slot])
	return 1;

    if (err = tool->move(this_object()))
	return err;
    if (split = tool->last_split_obj())
	tool = split;
    add_to_subloc(SUBLOC_HOLD, tool);
    hold[tool] = slot;
    if (!hold[slot])
	hold[slot] = ({ tool });
    else
	hold[slot] += ({ tool });
    
    return 0;
}

/*
 * Function name: add_equipment
 * Description:   help function to easy equip npcs
 * Arguments:     path - the file to add
 * Returns:       object (the cloned equipment)
 */
nomask object
add_equipment(string path)
{
    int    *hands;
    object ob;

    if (!sizeof(hands = slot_types[SLT_HAND]))
	return 0;
    ob = clone_object(path);
    if (add_tool(ob, hands[0]))
    {
	remove_object(ob);
	error("Living failed to carry that much");
    }
    return ob;
}

/*
 * Function name: wield_tool
 * Description:   wield a tool
 * Arguments:	  tool - the tool object
 * Returns:       location string if success
 *                -1 if tool does not fit
 *                -2 if tool is too big
 *                -3 if already wielded
 *                -4 if wield failed by unknown reason
 */
nomask mixed
wield_tool(object tool)
{
    int    i, sz, hnum, *left_size, *slotv, *locs, *used;
    string *desc;

    if (!tool->query_prop(IS_WIELDABLE))
	return 0;
    if (tool->query_wielder())
	return -3;
    if (!(sz = sizeof(locs = slot_types[SLT_HAND])))
	return -1;
    
    slotv = tool->query_wield_size();
    hnum  = slotv[1];
    left_size = allocate(sz);
    for (i=0; i<sz; i++)
    {
	if (wield[locs[i]])
	    continue;
	if ((left_size[i] = slot_space(locs[i], tool)) <
	    slot_sizes[locs[i]] * 95 / 100)
	{
	    /* only 5% of the slot allowed to be occupied */
	    left_size[i] = 0;
	    continue;
	}
	if (left_size[i] < slotv[0])
	    continue;
	if (hnum < -1)
	{
	    /* forced number of hands (longbows), one hand is only allowed 
	       to serve 'space / hands' of the needed volume */
	    left_size[i] = -(slotv[0] / hnum); 
	    continue;
	}
	used = ({ locs[i] }); /* enough space in one hand */
	break;
    }
    if (i == sz)
    {
	/* check if weapon could be used with more than one hand */
	used = allocate(sz);
	for (i=0; i<sz; i++)
	{
	    if (!left_size[i])
		continue;
	    slotv[0] -= left_size[i];
	    used[i] = locs[i];
	    if (slotv[0] < 1)
		break;
	}
	used -= ({ 0 });
	if (i == sz || !(sz = sizeof(used)))
	    /* all free wield-space is not enough */
	    return -2;
	if (hnum < 0)
	{
	    if (sz != -hnum)
		/* does not fit, here: weapon demands fixed number of hands */
		return -1;
	}
	else if (sz > hnum) 
	    /* does not fit, here: weapon demands smaller number of hands */
	    return -1;
    }

    if (tool->call_event(E_WIELD, this_object(), tool))
	return -4;
    
    for (i=sizeof(used), desc=allocate(i); i--;)
    {
	wield[used[i]] = tool;
	desc[i]        = slot_descs[used[i]];
    }
    add_to_subloc(SUBLOC_WIELD, tool);
    remove_from_subloc(SUBLOC_HOLD, tool);
    hold[hold[tool]] -= ({ tool });
    hold[tool]        = 0;
    
    tool->set_wielder(this_object());
    tool->move(this_object());
    
    return COMPOSITE_WORDS(desc);
}

/*
 * Function name: fit_locs
 * Description:   fits a list of locations to the livings slots
 * Arguments:     locs - list of locations that shall be mapped
 *                       onto the slots
 *                start_slot - slot to start the mapping with
 *                forbid_slots - slots that shall not be fitted onto 
 * Returns:       list of fitted locations or ({ })
 */
nomask int *
fit_locs(int * locs, int start_slot, int * forbid_slots)
{
    int i, new_start, * conn_locs, * fitted_locs, * dummy;

    for (new_start = 0, i = sizeof(locs)-1; i>=0; i--)
    {
	if (start_slot == locs[i])
	{
	    new_start = locs[i]; /* exact fitting */
	    locs -= ({ locs[i] });
	    break;
	}
	if (SLOT_TYPE(start_slot) == locs[i]) /* fitting to non-special slot */
	    new_start = start_slot;
    }
    if (!new_start)
	return 0;
    if (i == -1) /* only non-special location for start_slot was found */
	locs -= ({ SLOT_TYPE(new_start) });
    if (!sizeof(locs)) /* no locations to fit left */
	return ({ new_start });
    forbid_slots += ({ new_start });
    if (dummy = query_connections(start_slot))
	conn_locs = dummy;
    else
	conn_locs = ({ });
    
    if (dummy = ({ query_reconnect(start_slot) }) )
	conn_locs += dummy;
    for ( i = sizeof(conn_locs)-1; i>=0; i--)
    {
	if (fitted_locs = fit_locs(locs, conn_locs[i], forbid_slots))
	    return ({ new_start }) + fitted_locs;
    }
    return 0; /* no fitting and connected slot for rest of locs found */
}

/*
 * Function name: wear_tool
 * Description:   wear a tool (on a specified slot)
 * Arguments:	  tool - the tool object
 *                pref_slot - if specified, the slot to wear the tool on
 * Returns:       location string
 *                -1 if tool does not fit
 *                -2 if tool is too big
 *                -3 if something is already occupying slot
 *                -4 if already worn
 *                -5 if tool is too small
 *                -6 if wear failed by unknown reason
 */
nomask varargs mixed
wear_tool(object tool, int pref_slot)
{
    int     i, j, sz, sz2, d, loc, *left, *locs, *used;
    string  *desc;
    object  *w;
    mapping left_slots;
    
    if (!tool->query_prop(IS_WEARABLE))
	return 0;
    if (tool->query_wearer())
	return -4;
    if ((used = tool->query_race_sizes())[0] > query_size())
	return -2;
    if (used[1] < query_size())
	return -5;
    if (!(sz = sizeof(locs = tool->query_locations())))
	return -1;
    if (pref_slot)
    {
	/* adapting the tool_locations to the preferred slots */
	/* all toolparts have to lie on directly connected slots */
	left = fit_locs(locs, pref_slot, ({ }));
	locs = left;
	if (!locs)
	    return -1;
    }
    
    for (i=0, used=allocate(sz), left_slots=([ ]); i<sz; i++)
    {
	loc = locs[i];
	if (loc % 100 > 0) /* left, right, whatever - specification */
	{
	    if (!slot_sizes[loc])
		return -1;
	    if (sizeof(w = query_worn(loc)) > 0)
	    {
		if ((d = w[0]->query_layer() - tool->query_layer()) > 0)
		    return -3;
		if (!d && !w[0]->query_prop(OBJ_THIN_MATERIAL))
		    return -3;
	    }
	    used[i] = loc;
	    continue;
	}
	if (!left_slots[loc])
	{
	    if (!slot_types[loc])
		return -1;
	    left_slots[loc] = slot_types[loc][..];
	}
	if (!(sz2 = sizeof(left = left_slots[loc])))
	    return -1;
	for (j=0; j<sz2; j++)
	{
	    if (sizeof(w = query_worn(left[j])) > 0)
	    {
		if ((d = w[0]->query_layer() - tool->query_layer()) > 0)
		    continue;
		if (!d && !w[0]->query_prop(OBJ_THIN_MATERIAL))
		    continue;
	    }
	    used[i] = left[j];
	    left_slots[loc] -= ({ left[j] });
	    break;
	}
	if (j == sz2)
	    return -3;
    }
    if (tool->call_event(E_WEAR, this_object(), tool))
	return -6;
    
    used -= ({ 0 });
    for (i=sizeof(used), desc=allocate(i); i--;)
    {
	d = used[i];
	if (!worn[d])
	    worn[d] = ({ tool });
	else
	    worn[d] = ({ tool }) + worn[d]; /* don't change order! */
	desc[i] = slot_descs[d];
    }
    add_to_subloc(SUBLOC_WORN, tool);
    remove_from_subloc(SUBLOC_HOLD, tool);
    hold[hold[tool]] -= ({ tool }); /* remove from held slot list */
    hold[tool]        = 0;             /* remove slot marker */
    
    tool->set_wearer(this_object());
    tool->move(this_object());
    
    return COMPOSITE_WORDS(desc);
}

/*
 * Function name: remove_tool
 * Description:   Remove a tool from a toolslot
 * Arguments:	  tool - The tool
 *                flag - 1 if only removed but not dropped
 * Return:        1 if success, 0 else
 */
nomask varargs int
remove_tool(object tool, int flag) 
{
    int     i, sz, *slots;
    object  *curr;
    
    if (!tool)
	return 0;
    if (hold[tool])
    {
	if (tool->call_event(E_REMOVE_TOOL, this_object(), tool))
	    return 0;
	remove_from_subloc(SUBLOC_HOLD, tool);
	hold[hold[tool]] -= ({ tool });
	hold[tool]        = 0;
    }
    else if (IN_ARRAY(tool, query_subloc(SUBLOC_WIELD)))
    {
	if (tool->call_event(E_REMOVE_TOOL, this_object(), tool))
	    return 0;
	slots = slot_types[SLT_HAND];
	for (i=sizeof(slots); i--;)
	{
	    if (wield[slots[i]] == tool)
		wield[slots[i]] = 0;
	}
	remove_from_subloc(SUBLOC_WIELD, tool);
	tool->set_wielder(0);
    }
    else if (IN_ARRAY(tool, query_subloc(SUBLOC_WORN)))
    {
	slots = map_indices(worn) - ({ -1 }); /* slots that contain armor */
	for (i=sz=sizeof(slots); i--;)
	{
	    if (!IN_ARRAY(tool, curr = worn[slots[i]]))
	    {
		slots[i] = 0;
		continue;
	    }
	    if (curr[0] != tool)
		return 0; /* something is worn above the armor */
	}
	if (tool->call_event(E_REMOVE_TOOL, this_object(), tool))
	    return 0;
	remove_from_subloc(SUBLOC_WORN, tool);
	for (i=sz; i--;)
	    if (slots[i])
		worn[slots[i]] -= ({ tool });
	tool->set_wearer(0);
    }
    
    if (!flag) /* dropped? */
	return 1;
    
    if (!(sz = sizeof(slots = slot_types[SLT_HAND])))
    {
	tool->set_coordinates(query_coordinates());
	tool->move(E(this_object()));
	return 1;
    }
    for (i=sz; i--;)
    {
	if (slot_space(slots[i], tool) > 0)
	{
	    add_tool(tool, slots[i]);
	    return 1;
	}
    }
    tool->set_coordinates(query_coordinates());
    tool->move(E(this_object()));
    
    return 1;
}

/*
 * Function name: query_wield
 * Description:   Returns the tools wieled in a specified location.
 *                Use SLT_ALL for a list of all wielded tools.
 * Arguments:	  slot - The toolslot
 * Return:        object array or single weapon objectpointer
 */
nomask mixed
query_wield(int slot)
{
    if (slot == SLT_ALL)
	return query_subloc(SUBLOC_WIELD);
    return wield[slot];
}
	
/*
 * Function name: query_worn
 * Description:   Returns the tools worn in a specified location.
 *                Use SLT_ALL for a list of all worn tools.
 * Arguments:	  slot - The toolslot
 * Return:        object array
 */
nomask object *
query_worn(int slot)
{
    if (slot == SLT_ALL)
	return query_subloc(SUBLOC_WORN);
    if (!worn[slot])
	return ({ });
    worn[slot] -= ({ 0 });
    return worn[slot];
}

/*
 * Function name: query_hold
 * Description:   Returns the tools hold in a specified location.
 *                Use SLT_ALL for a list of all held tools.
 * Arguments:	  slot - The toolslot
 * Return:        object array
 */
nomask object *
query_hold(int slot)
{
    if (slot == SLT_ALL)
	return query_subloc(SUBLOC_HOLD);
    if (!hold[slot])
	return ({ });
    hold[slot] -= ({ 0 });
    return hold[slot];
}

/*
 * Function name: query_hold_slot
 * Description:   Returns the slot in which the object is held
 *                used for correct heap splitting
 * Arguments:	  ob - the object to query
 * Return:        integer slot
 */
nomask int
query_hold_slot(object ob)
{
    if (!ob)
	return 0;
    return hold[ob];
}

/* 
 * Function name: add_tool_slot
 * Description:   creates a new slot
 * Arguments:     slot - the type + slotnumber (maybe (SLT_HAND + 3),
 *                                              if this is third hand)
 *                desc - a name for the toolslot ("third hand")
 *                size - slot size (a volume that can be carried in the slot)
 */
nomask void
add_tool_slot(int slot, string desc, int size)
{
    int   type;
    mixed data;
    
    if (slot < 1 || !(type = SLOT_TYPE(slot)))
	return;
    if (slot_sizes[slot])
	return;
    
    slot_descs[slot] = desc;
    
    if (wounds && wounds[slot])
    {
	if (data = this_object()->query_hitloc_data(slot))
	    cb_add_hitloc(slot, data);
	slot_sizes[-slot] = size;
	return;
    }
    
    if (!slot_types[type])
	slot_types[type] = ({ slot });
    else
	slot_types[type] += ({ slot });
    
    slot_sizes[slot] = size;
    slot_sizes[-slot] = 0; /* remove possible stored size */
    
    if (data = this_object()->query_hitloc_data(slot))
	cb_add_hitloc(slot, data);
    if (data = this_object()->query_attack_data(slot))
	cb_add_attack(slot, data);
}

/*
 * Function name: remove_tool_slot
 * Description:   removes a tool-slot
 * Arguments:     slot - slot-id (slottype + slotnumber)
 *                flag - true if hitloc should stay (e.g. unusable slots
 *                       will be removed but still possible to hit them)
 */
nomask varargs void
remove_tool_slot(int slot, int flag)
{
    int     i, sz, *conn, *co;
    object  env, ob, *conts;

    if (!slot_sizes[slot])
	return;

    co = query_coordinates();
    env = E(this_object());
    for (i=sizeof(conts = query_hold(slot)); i--;)
    {
	conts[i]->reset_split();
	conts[i]->set_coordinates(co);
	conts[i]->move(env);
    }
    if (ob = wield[slot])
    {
	ob->set_coordinates(co);
	ob->move(env);
    }
    if (!flag)
    {
	for (i=sizeof(conts = query_worn(slot)); i--;)
	{
	    conts[i]->set_coordinates(co);
	    conts[i]->move(env);
	}
	cb_remove_hitloc(slot);
    }
    cb_remove_attack(slot);
    
    slot_sizes[-slot] = slot_sizes[slot]; /* store for later restore */
    slot_sizes[slot] = 0;
    
    add_bloodloss( -query_bloodloss(), slot); /* stop bleeding on lost slots */
    slot_types[SLOT_TYPE(slot)] -= ({ slot });
    for (i=sizeof(conn = connections[slot]); i--;)
	remove_tool_slot(conn[i], flag);
}

/*
 * Function name: query_slots
 * Description:   returns an array containing all slots of this living
 * Returns:       array of integers
 */
nomask int *
query_slots()
{
    return map_indices(slot_descs);
}

/* 
 * Function name: query_slot_type
 * Description:   returns all slots according to given slot type
 * Arguments:     type - the slot type to query
 * Returns:       array or 0
 */
nomask int *
query_slot_type(int type)
{
    return slot_types[type];
}

/* 
 * Function name: query_slot_desc
 * Description:   returns a slot description
 * Arguments:     slot - the slot
 * Returns:       the description
 */
nomask string
query_slot_desc(int slot)
{
    return slot_descs[slot];
}

/* 
 * Function name: query_desc_slot
 * Description:   returns a slot for given description
 * Arguments:     desc - the description
 * Returns:       the slot
 */
nomask int
query_desc_slot(string desc)
{
    int    i, sz, *slots;

    slots = map_indices(slot_descs);
    for (i=0, sz=sizeof(slots); i<sz; i++)
	if (slot_descs[slots[i]] == desc)
	    return slots[i];
    return 0;
}

/* Function name: show_inventory
 * Description:   create a description for the inventory
 * Arguments:     for_ob - object to show the description 
 * Returns:       string, inventory description
 */
nomask string
show_inventory(object for_ob)
{
    string out, str;
    object *o;
    
    out = "";
    if (sizeof(o = query_subloc(SUBLOC_WIELD)) &&
	(str = COMPOSITE_UNSEEN(o, for_ob)))
    {
	if (for_ob == this_object())
	    out += BS("You are wielding " + str + ".\n", 77, 1)[1..];
	else
	    out += BS(capitalize(query_pronoun()) + " is wielding " + str +
		      ".\n", 77, 1)[1..];
    }
    
    if (sizeof(o = query_subloc(SUBLOC_WORN)) &&
	(str = COMPOSITE_UNSEEN(o, for_ob)))
    {
	if (for_ob == this_object())
	    out += BS("You are wearing " + str + ".\n", 77, 1)[1..];
	else
	    out += BS(capitalize(query_pronoun()) + " is wearing " + str +
		      ".\n", 77, 1)[1..];
    }
    
    if (sizeof(o = query_subloc(SUBLOC_HOLD)) &&
	(str = COMPOSITE_UNSEEN(o, for_ob)))
    {
	if (for_ob == this_object())
	    out += BS("You are holding " + str + ".\n", 77, 1)[1..];
	else
	    out += BS(capitalize(query_pronoun()) + " is holding " + str +
		      ".\n", 77, 1)[1..];
    }
    return out;
}

/***************************************************************************
 * fractures
 */

/*
 * Function name: set_wound
 * Description:   set or remove a wound
 * Arguments:     slot - the slot that got broken or healed
 *                status - 0 to heal, or see definitions in <combat.h>
 */
nomask void
set_wound(int slot, int flag)
{
    int    i, sz, *conn;
    string s;
    object ob;
    
    if (!wounds)
	wounds = ([ slot:flag ]);
    else
	wounds[slot] = flag;
    
    if (!flag)
    {
	add_tool_slot(slot, slot_descs[slot], slot_sizes[-slot]);
	return;
    }
    if (flag == SLOT_LOST)
    {
	sz = slot_sizes[slot];	/* before remove_tool_slot */
	remove_tool_slot(slot, 0);
	for (i=sizeof(conn = connections[slot]); i--;)
	    wounds[conn[i]] = flag;

	ob = clone_object("/base/living/limb");
	ob->set_name(s = slot_descs[slot]);
	ob->set_short(s);
	ob->set_pshort(LANG_PSENT(s));
	ob->set_long("This is " + LANG_ADDART(s) + ".\n");
	ob->set_lwvs(0, query_weight() * sz / query_volume(), sz, sz / 50);
	ob->set_coordinates(query_coordinates());
	ob->move(E(this_object()));
    }
    else
    {
	remove_tool_slot(slot, 1); /* don't remove hitloc */
	for (i=sizeof(conn = connections[slot]); i--;)
	    if (!wounds[conn[i]])
		wounds[conn[i]] = SLOT_UNUSABLE;
    }
}

/*
 * Function name: query_wounds
 * Description:   Returns the wound map
 * Returns:       mapping wounds
 */
nomask mapping
query_wounds()
{
    if (!wounds)
	wounds = ([ ]);
    return wounds;
}

/*
 * Function name: bandage
 * Description:   add a slot as bandaged
 * Arguments:     slot - the bandaged slot
 *                healtime - how long does it take to heal the slot
 */
nomask void
bandage(int slot, int healtime)
{
    int i, t, min, *ind;
    
    if (!bandaged)
	bandaged = ([ ]);
    t = time();
    if (slot > -1) /* negative slot: just a call to update the call_out */
	bandaged[slot] = t + healtime;
    if ((min = remove_call_out("resolve_bandage")) < 0)
	min = t;
    for (i=sizeof(ind=map_indices(bandaged)); i--;)
    {
	if (bandaged[ind[i]] - t < min)
	    min = bandaged[ind[i]] - t;
    }
    call_out("resolve_bandage", min);
}

/*
 * Function name: remove_bandage
 * Description:   remove the bandages from a slot so it will no longer heal
 * Arguments:     slot - the slot id
 */
nomask void
remove_bandage(int slot)
{
    bandaged[slot] = 0;
    if (!sizeof(map_indices(bandaged)))
    {
	remove_call_out("resolve_bandage");
	return;
    }
    bandage(-1, 0); /* update call_out */
}

static nomask void
resolve_bandage()
{
    int i, t, min, *ind;

    min = t = time();
    for (i=sizeof(ind=map_indices(bandaged)); i--;)
    {
	if (bandaged[ind[i]] <= t)
	{
	    set_wound(ind[i], 0);
	    catch_tell("Your " + query_slot_desc(ind[i]) +
		       " feels well again.\n");
	    bandaged[ind[i]] = 0;
	}
	else if (bandaged[ind[i]] - t < min)
	    min = bandaged[ind[i]] - t;
    }
    if (map_sizeof(bandaged))
	call_out("resolve_bandage", min);
}

/*
 * Function name: query_bandaged
 * Description:   Returns the bandaged map
 * Returns:       mapping bandaged
 */
nomask mapping
query_bandaged()
{
    if (!bandaged)
	bandaged = ([ ]);
    return bandaged;
}
