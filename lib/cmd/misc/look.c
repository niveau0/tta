/*
 * Function name: look_at
 * Description:   handle looking 'at' an object
 * Arguments:     ob - the object to look at
 *                where - which object to look at 'in' ob
 *                tp - looking player
 *                listen - all object listening to messages
 *                pl - a living if looking at a livings objects
 * Returns:       description string 
 */
private string
look_at(object ob, string where, object tp, object *listen, object at)
{
    int    i, sz;
    string out, attach, state;
    object *found_ob;

    if (where) /* look at an object inside of another */
    {
	if (living(ob) || !ob->query_prop(IS_CONT) ||
	    (!ob->query_prop(CONT_TRANSP) && ob->query_prop(CONT_CLOSED)))
	{
	    tp->catch_msg(({ "You can't see the inside of ",
				 QTNAME(ob), ".\n" }), tp, MSG_SEE);
	    return "";
	}
	
	if (objectp(ob->query_room()))
	    ob = ob->query_room(); /* get linked room to container */
	
	found_ob = tp->visible(I(ob, 0));
	found_ob = parse(where, found_ob, tp);
	if (!(sz = sizeof(found_ob)))
	{
	    return "You see no '" + where + "' inside the " +
		ob->short() + ".\n";
	}
	out = "";
	for (i=0; i<sz; i++)
	    out += found_ob[i]->long() + "\n";
	tell_objects(listen - ({ ob }),
		     ({ QCTNAME(tp), " looks into ", QNAME(ob), ".\n" }),
		     MSG_SEE, tp);
	return out;
    }
    
    if (ob != tp) /* don't show messages if looking at yourself */
    {
	tell_objects(listen - ({ ob, at }),
		     ({ QCTNAME(tp), " looks at ",
			    QTNAME(ob), ".\n" }), MSG_SEE, tp);
	if (living(ob))
	    ob->catch_msg(({ QCTNAME(tp), " looks at you.\n" }),
			  tp, MSG_SEE);
	if (at && living(at))
	    at->catch_msg(({ QCTNAME(tp), " looks at something of you.\n" }),
			  tp, MSG_SEE);
    }
    if (living(ob))
	return ob->long() + ob->query_state_desc(tp) + 
	    ob->show_inventory(tp) + "\n";
    if (!(attach = ob->show_attached(tp)))
	attach = "";
    if (!(out = ob->show_inside(tp)))
	out = "";
    return ob->long() +  ob->query_state_desc(ob) + 
	attach + out + "\n";
}

/*
 * Function name: look
 * Description:   look at or in an object or pseudo item
 * Arguments:     str - what to look at
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
look(string str, object tp)
{
    int    i, sz;
    string prep, what, where, out, *items;
    object at, here, *ob_list, *all;

    here = E(tp);
    if (!str || !strlen(str))
    {
	tp->look_room(here);
	return 1;
    }
    sscanf(str, "%s %s", prep, what);
    if (prep != "at" && prep != "in" && prep != "inside")
    {
	tp->catch_tell("Look needs a preposition 'at' or 'in' " +
		       "with an object.\n");
	return -1;
    }
    if (sscanf(what, "%s at %s", what, where) == 2)
    {
	all = I(tp, 7) + tp->visible(I(here, 0));
	if (!sizeof(ob_list = parse(where, all, tp)))
	{
	    tp->catch_tell("You don't see any '" + where + "'.\n");
	    return -1;
	}
	if (living(at = ob_list[0]) && (prep == "in" || prep == "inside"))
	{
	    tp->catch_tell("You can't see the inside of someones items.\n");
	    return -1;
	}
	ob_list = tp->visible(I(at, 7));
	ob_list = parse(what, ob_list, tp);
	if (!sizeof(ob_list))
	{
	    tp->catch_tell("You can't see such an item at " +
			   at->query_objective() + ".\n");
	    return -1;
	}
	where = 0;
    }
    else
    {
	if (prep == "at")
	    sscanf(what,"%s in %s", where, what);

	all = tp->visible(I(tp, 7) + I(here, 0));
        ob_list = parse(what, all, tp);
    }
    if (!sizeof(ob_list))	/* no object found, check for pseudo items */
    {
	if (where ||
	    !(sz = sizeof(items = parse_items(what, ({ here }) + all))))
	{
	    tp->catch_tell("There is nothing.\n");
	    return -1;
	}
	str = implode(items, "\n") + "\n";
	tp->catch_tell(str);
	tell_objects(LISTEN(here) - ({ tp }),
		     ({ QCTNAME(tp), " looks at the " + what + ".\n" }),
		     MSG_SEE, tp);
	return 1;
    }
    all = LISTEN(here) - ({ tp });
    out = "";
    if (prep == "at")
    {
	for (i=0, sz=sizeof(ob_list); i<sz; i++)
	    out += look_at(ob_list[i], where, tp, all, at);
    }
    else
    {
	for (i=0, sz=sizeof(ob_list); i<sz; i++)
	{
	    if (!(what = ob_list[i]->show_inside(tp)))
		tp->catch_msg(({ "You can't see the inside of ", 
				     QTNAME(ob_list[i]), ".\n" }),
			      tp, MSG_SEE); 
	    else
		out += what;
	}
    }
    smore(out, tp);
    return 1;
}

/*
 * Function name: examine
 * Description:   pseudonym for  look at, look in, etc
 * Arguments:	  str - tail of examine command or exa command
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
examine(string str, object tp)
{
    if (!str || !strlen(str))
    {
	tp->catch_tell("Examine needs an object as argument.\n");
	return -1;
    }
    return look("at " + str, tp);
}
