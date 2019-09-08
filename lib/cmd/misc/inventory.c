/*
 * Function name: show_inventory
 * Description:   shows the inventory of the player
 * Arguments:	  str - not used
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
show_inventory(string str, object tp)
{
    int    i, szi, *slots;
    string out, desc;
    object *obj;
    
    if (!sizeof(I(tp, 7)))
    {
	tp->catch_tell("You are carrying nothing.\n");
	return 1;
    }
    if (tp->query_prop(LIVE_LONG_INVENTORY))
    {
	obj = ({ });
	out = "You carry the following:\n";
	slots = tp->query_slots();
	
	for (i=0, szi=sizeof(slots); i<szi; i++)
	{
	    obj = tp->query_hold(slots[i]) +
		  tp->query_worn(slots[i]) +
		  ({ tp->query_wield(slots[i]) });
	    obj -= ({ 0 });
	    if (!sizeof(obj))
		continue;
	    desc = tp->query_slot_desc(slots[i]);
	    out += BS(" " + desc + ": " + COMPOSITE_UNSEEN(obj, tp), 77,
		      strlen(desc)+3)[strlen(desc)+3..] + "\n";
	}
	tp->catch_tell(out);
	return 1;
    }
    tp->catch_tell(tp->show_inventory(tp));
    return 1;
}
