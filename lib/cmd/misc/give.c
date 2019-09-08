/*
 * Function name: give
 * Description:   give an item to someone
 * Arguments:     str - what to give
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
give(string str, object tp)
{
    int    i, *splitsizes;
    string item, pname, *comp;
    object here, whom, *to, *items;

    if (!str || sscanf(str, "%s to %s", item, pname) != 2)
    {
	tp->catch_tell("Usage: give <item> to <someone|something>\n");
	return -1;
    }
    here  = E(tp);
    items = parse(item, tp->query_hold(SLT_ALL), 0);
    if (!sizeof(items))
    {
	tp->catch_tell("You don't hold such an item.\n");
	return -1;
    }
    to = tp->visible(I(here, 2));
    to = parse(pname, to, tp, tp->query_step());
    if (!check_size(to, "No '" + pname + "' in range to give it to.\n", tp))
	return -1;
    whom = to[0];
    if (whom == tp)
    {
	tp->catch_tell("Give things to yourself?\n");
	return -1;
    }
    for (i=sizeof(items), splitsizes=allocate(i); i--;)
	splitsizes[i] = items[i]->query_split_size();
    
    comp = COMPOSITE_ARRAY(items);
    whom->catch_msg(({ QCTNAME(tp), " offers you " }) +
		    comp +  ({ ".\n" }), tp, MSG_SEE | MSG_HEAR, 590);
    tell_objects(LISTEN(here) - ({ tp, whom }),
		 ({ QCTNAME(tp), " offers " }) + comp +
		 ({ " to ", QTNAME(whom), ".\n" }), MSG_SEE, tp);
    tp->catch_msg(({ "You offer " }) +
		  comp + ({ " to ", QTNAME(whom), ".\n" }), 0, 0);
    if (whom->query_prop(OBJ_ACCEPT_STATUS))
	return acceptance(tp, whom, ({ time(), whom, items, splitsizes }));
    tp->add_prop(OBJ_OFFERED_ITEMS, ({ time(), whom, items, splitsizes }));
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
