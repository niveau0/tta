/*
 * Function name: drink
 * Description:   drink from something
 * Arguments:     str - from what to drink
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
drink(string str, object tp)
{
    int    i, sz;
    string out;
    object *list;
    
    if (!str || !sscanf(str, "from %s", str))
    {
	tp->catch_tell("Usage: drink from <item>\n");
	return -1;
    }
    list = parse(str, I(tp, 4) + I(E(tp), 1), tp, tp->query_step());
    if (!check_size(list, "No '" + str + "' in range to drink from.\n", tp))
	return -1;
    out = "";
    for (i=0, sz=sizeof(list); i<sz; i++)
    {
	if (!list[i]->drink_cmd())
	    out += "You cannot drink from the " + list[i]->short() + ".\n";
    }
    tp->catch_tell(out);
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
