/*
 * Function name: put
 * Description:   put an item somewhere if possible
 * Arguments:     str - what to put
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
put(string str, object tp)
{
    string what, where;
    object here, cont, room, *items, *into, *all_inv, surface, *onto;
    mixed  comp;

    if (!str)
    {
	tp->catch_tell("Usage: put <item> in[to]|on <object>\n");
	return -1;
    }
    
    here = E(tp);
    if (sscanf(str, "%s in %s", what, where) != 2 &&
	sscanf(str, "%s into %s", what, where) != 2)
    {
	if (sscanf(str, "%s on %s", what, where) == 2)
	{
	    all_inv = I(tp, 0);
	    onto = all_inv + tp->visible(I(here, 1));
	    onto = parse(where, onto, tp, tp->query_step());
	    if (!check_size(onto, "There's no "+where+
		    " in range to put something on.\n", tp))
		return -1;
	    surface = onto[0];

	    items = parse(what, all_inv, tp);
	    if (!sizeof(items))
	    {
		tp->catch_tell("You don't have this "+what+
			       " with you.\n");
		return -1;
	    }
	    
	    items = try_put_on(items, surface, tp);
	    if (sizeof(items))
	    {
		comp = ((comp = COMPOSITE_ARRAY(items)) +
			({ " on the " + where + ".\n" }));
		tell_objects(LISTEN(here) - ({ tp }),
			     ({ QCTNAME(tp), " puts " }) + comp, MSG_SEE, tp);
		tp->catch_msg(({ "You put " }) + comp);
		tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
	    }
	    return 1;
	}
 	tp->catch_tell("You cannot put '"+str+"'.\n");
	return -1;
    }
    all_inv = I(tp, 0);
    into = all_inv + tp->visible(I(here, 1));
    into = parse(where, into, tp, tp->query_step());
    if (!check_size(into, "No '" + where +
		    "' in range to put something in.\n", tp))
	return -1;
    cont = into[0];
    if (cont->query_prop(CONT_CLOSED))
    {
	tp->catch_tell("But the " + where + " is closed.\n");
	return -1;
    }
    if (room = cont->query_room())
	cont = room;
    else
	all_inv -= ({ cont });
    items = parse(what, all_inv, tp);
    if (!sizeof(items))
    {
	tp->catch_tell("You don't have that item.\n");
	return -1;
    }
    items = try_drop(items, cont, tp, 0);
    if (sizeof(items))
    {
	comp = ((comp = COMPOSITE_ARRAY(items)) +
		({ " into the " + where + ".\n" }));
	
	tell_objects(LISTEN(here) - ({ tp }),
		     ({ QCTNAME(tp), " puts " }) + comp, MSG_SEE, tp);
	tp->catch_msg(({ "You put " }) + comp);
	tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    }
    else
	tp->catch_tell(query_drop_error());
    return 1;
}
