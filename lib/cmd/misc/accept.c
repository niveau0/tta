/*****************************************************************************
 * accept the offered items
 */
static nomask int
acceptance(object from, object tp, mixed prop)
{
    int    i, *splitsizes;
    object *list;
    mixed  comp;
    
    list = prop[2];
    for (i=sizeof(list), splitsizes=prop[3]; i--;)
	list[i]->split_heap(splitsizes[i]);
    list = try_give(list, from, tp);
    if (!sizeof(list))
    {
	from->catch_tell(query_give_error());
	return -1;
    }
    comp = COMPOSITE_ARRAY(list);
    from->catch_msg(({ QCTNAME(tp), " accepts your offer.\n" }),
		    tp, MSG_HEAR | MSG_SEE, 590);
	
    tell_objects(LISTEN(E(tp)) - ({ tp, from }),
		 ({ QCTNAME(from), " gives " }) + comp +
		 ({ " to ", QTNAME(tp), ".\n" }), MSG_SEE, from);
    tp->catch_msg(({ "You get " }) +
		  comp + ({ " from ", QTNAME(from), ".\n" }),
		  0, 0);
    from->add_prop(OBJ_OFFERED_ITEMS, 0);
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}

/*
 * Function name: accept
 * Description:   accept items from someone
 * Arguments:     str - what to accept
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
accept(string str, object tp)
{
    int    i;
    string what;
    object *list;
    mixed  prop;

    if (str == "all")
    {
	tp->catch_tell("You will now accept all items by default.\n");
	tp->add_prop(OBJ_ACCEPT_STATUS, 1);
	
	list = tp->visible(I(E(tp), 0));
	for (i=sizeof(list); i--;)
	{
	    if (sizeof(prop = list[i]->query_prop(OBJ_OFFERED_ITEMS)) != 4 ||
		prop[0] < time() - 20 || prop[1] != tp)
		continue;
	    acceptance(list[i], tp, prop);
	}
	return 1;
    }
    if (str == "nothing")
    {
	tp->catch_tell("You have to accept items now by command.\n");
	tp->add_prop(OBJ_ACCEPT_STATUS, 0);
	return 1;
    }
    if (!str || !sscanf(str, "%s from %s", what, str))
    {
	tp->catch_tell("Usage: accept all from <name>\n" +
		       "       accept <item> from <name>\n"+
		       "       accept all\n" +
		       "       accept nothing\n");
	return -1;
    }
    list = tp->visible(I(E(tp), 0));
    list = parse(str, list, tp, tp->query_step());
    if (!check_size(list, "No '" + str + "' in range.\n", tp))
	return -1;
    if (what == "all")
    {
	if (sizeof(prop = list[0]->query_prop(OBJ_OFFERED_ITEMS)) != 4 ||
	    prop[0] < time() - 20 || prop[1] != tp)
	{
	    tp->catch_tell(capitalize(list[0]->query_pronoun()) +
			   " doesn't offer anything to you.\n");
	    return -1;
	}
	return acceptance(list[0], tp, prop);
    }
    if (sizeof(prop = list[0]->query_prop(OBJ_OFFERED_ITEMS)) != 4 ||
	prop[0] < time() - 20 || prop[1] != tp )
    {
	tp->catch_tell(capitalize(list[0]->query_pronoun()) +
		       " doesn't offer it to you.\n");
	return -1;
    }
    for (i=0;i<=sizeof(prop[2])-1;i++)
    	if(prop[2][i]->query_name() == what)
	    return acceptance(list[0], tp, ({ prop[0],
					      prop[1],
					      ({ prop[2][i] }),
					      prop[3] }));
    tp->catch_tell(capitalize(list[0]->query_pronoun()) +
		   " doesn't offer it to you.\n");
    return -1;
}
