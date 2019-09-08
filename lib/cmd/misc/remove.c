/*
 * Function name: remove
 * Description:   remove a worn object
 * Arguments:     str - what to remove or objects still to go
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
remove(mixed str, object tp)
{
    int    i, sz, sz2, *hands;
    string msg;
    object ob, *obj;
    
    if (!str)
    {
	tp->catch_tell("Usage: remove <item>\n");
	return -1;
    }
    if (pointerp(str))
    {
	obj = str & tp->query_worn(SLT_ALL);
	if (!(sz = sizeof(obj)))
	    return -1;
    }
    else
    {
	if (!(sz = sizeof(obj = parse(str, tp->query_worn(SLT_ALL), 0))))
	{
	    tp->catch_tell("You don't wear such equipment.\n");
	    return -1;
	}
    }
    hands = tp->query_slot_type(SLT_HAND);
    if (!(sz2 = sizeof(hands)))
    {
	tp->catch_tell("Ouch, you have no hands.\n");
	return -1;
    }
    ob = obj[0];
    for (i=0; i<sz2; i++)
	if (tp->slot_space(hands[i]) > 0)
	    break;
    if (i == sz2)
    {
	tp->catch_tell("You cannot carry that much in your hands.\n");
	return -1;
    }
    if (sz > 1)
	tp->add_command(({ this_object(), "remove", obj[1..] }));
    if (!tp->remove_tool(ob, 1))
    {
	tp->catch_msg(({ "You failed to remove ", QTNAME(ob), ".\n" }), 0, 0);
	return -1;
    }
    tp->catch_msg(({ "You remove ", QTNAME(ob), ".\n" }), 0, 0);
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " removes ", QNAME(ob), ".\n" }),
		 MSG_SEE, tp);
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
