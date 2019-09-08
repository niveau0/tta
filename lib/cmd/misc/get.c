/*
 * Function name: get
 * Description:   get an item if possible
 * Arguments:     str - what to get
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
get(string what, object tp)
{
    int    state, sk_result, sz, *co_tp, *co_ob;
    string from, *comp;
    object here, cont, room, *items, *container;
    
    if (!what)
    {
	tp->catch_tell("Usage: get|take <item> [from <container>]\n");
	return -1;
    }
    here  = E(tp);
    co_tp = tp->query_coordinates();
    if (sscanf(what,"%s from %s", what, from) == 2)
    {
	container = tp->visible(I(here, 0));
	container = parse(from, I(tp, 7) + container, tp,
			  tp->query_step());
	if (!check_size(container, "No '" + from +
			"' in range to get from.\n", tp))
	    return -1;
	cont = container[0];
	if (living(cont))
	{
	    if (!(state = cont->query_state())) /* not helpless */
	    {
		tp->catch_tell("You cannot get things from " +
			       cont->query_objective() + ".\n");
		return -1;
	    }
	    if (!(state & UNCONSCIOUS))
	    {
		sk_result = tp->skill_check(SK_PERCEPTION, -50, 0);
		if (((state & SLEEPING) && sk_result > 0) ||
		    ((state & MEDITATING) && sk_result > 25))
		    wake(cont->query_name());
	    }
	    return -1;
	}
	else
	{
	    if (cont->query_prop(CONT_CLOSED))
	    {
		tp->catch_tell("The " + from + " is closed.\n");
		return -1;
	    }
	    if (room = cont->query_room())
		cont = room;
	}
	items = tp->visible(I(cont, 1) + cont->attached());
	items = parse(what, items, 0);
    }
    else
    {
	items = tp->visible(I(here, 1));
	items = parse(what, items, tp, tp->query_step());
    }

    if (!sizeof(items))
    {
	if (!sizeof(parse_items(what, ({ here }) + items)))
	    tp->catch_tell("No such item(s) in range to get.\n");
	else
	    tp->catch_tell("The " + what + " cannot be taken.\n");
	return -1;
    }
    items = try_get(items, tp, co_tp);
    if (!sizeof(items))
    {
	tp->catch_tell(query_get_error());
	return -1;
    }
    comp = COMPOSITE_ARRAY(items);
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    tp->catch_msg(({ "You get " }) + comp + ({ ".\n" }), 0, 0);
    if (cont)
    {
	if (living(cont))
	{
	    cont->catch_msg(({ QCTNAME(tp), " gets " }) + comp +
			    ({ " from you.\n" }), tp, MSG_SEE);
	}
	tell_objects(LISTEN(here) - ({ tp, cont }),
		     ({ QCTNAME(tp), " gets " }) + comp +
		     ({ " from ", QTNAME(cont), ".\n" }), MSG_SEE, tp);
	return 1;
    }
    tell_objects(LISTEN(here) - ({ tp }),
		 ({ QCTNAME(tp), " gets " }) + comp + ({ ".\n" }),
		 MSG_SEE, tp);
    return 1;
}  
