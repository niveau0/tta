/*
 * Function name: search 
 * Description:   search for something
 * Arguments:     str - where to search
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
search(string str, object tp)
{
    if (!str)
	tp->echo("You start to search.\n");
    else
	tp->echo("You start to search at the " + str + ".\n");
    tp->add_state(SEARCHING);
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " starts searching.\n" }), MSG_SEE, tp);
    call_out("searching", 2, tp, str, 0, ({ }));
    return 1;
}

static nomask void
searching(object tp, string where, int count, object *found)
{
    int    i, sz, mod, hide, per;
    object *inv, *newfound;
    mixed  msg;
    
    if (!tp || !(tp->query_state() & SEARCHING))
	return;
    if (++count > 20)
    {
	tp->delete_state(SEARCHING);
	tp->catch_tell("\nYou are tired of searching.\n");
	return;
    }
    if (count % 5)
    {
	tp->catch_tell(".", 1);
	call_out("searching", 1, tp, where, count, found);
	return;
    }
    inv = I(E(tp), 0) - found;
    if (!(sz = sizeof(inv)))
	return;
    for (i=sz, newfound=allocate(sz); i--;)
    {
	if (!(hide = inv[i]->query_prop(OBJ_HIDE)) || VISIBLE(tp, inv[i]))
	    continue;
	if (where && IN_ARRAY(where, inv[i]->query_prop(OBJ_HIDE_PLACES)))
	    mod = 30;
	else
	    mod = 0;
	
	if (hide < tp->skill_check(SK_PERCEPTION, mod, 1))
	{
	    newfound[i] = inv[i];
	    inv[i]->add_prop(OBJ_HIDE_PLACES, 0);
	    inv[i]->add_prop(OBJ_HIDE, 0);
	}
    }
    newfound -= ({ 0 });
    if (sizeof(newfound))
    {
	tp->catch_msg(({ "You found " }) +
		      (msg = COMPOSITE_ARRAY(newfound)) + ({ ".\n" }), 0, 0);
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " found " }) + msg + ({ ".\n" }),
		     MSG_SEE, tp);
	found += newfound;
    }
    call_out("searching", 2, tp, where, count, found);
}
