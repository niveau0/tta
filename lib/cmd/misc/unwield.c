/*
 * Function name: unwield
 * Description:   unwield a wieled object
 * Arguments:     str - what to unwield
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
unwield(string str, object tp)
{
    int    i, sz;
    object *obj;
    mixed  rem;
    
    if (!str)
    {
	tp->catch_tell("Unwield what?\n");
	return -1;
    }
    if (!(sz = sizeof(obj = parse(str, tp->query_wield(SLT_ALL), 0))))
    {
	tp->catch_tell("You don't wield any '" + str + "'.\n");
	return -1;
    }
    for (i=0, rem=allocate(sz); i<sz; i++)
    {
	if (call_event(E_REMOVE_TOOL, tp, obj[i]))
	    continue;
	tp->remove_tool(obj[i], 1);
	rem[i] = obj[i];
    }
    rem -= ({ 0 });
    if (!sizeof(rem))
	return -1;
    rem = COMPOSITE_ARRAY(rem) + ({ ".\n" });
    tp->echo(({ "You unwield " }) + rem, 0, 0);
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " unwields " }) + rem, MSG_SEE, tp);
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
