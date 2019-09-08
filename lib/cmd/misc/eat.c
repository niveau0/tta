/*
 * Function name: eat
 * Description:   eat something
 * Arguments:     str - what to eat
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
eat(string str, object tp)
{
    int    i, sz;
    string out;
    object *list;
    
    if (!str)
    {
	tp->catch_tell("Usage: eat <item>\n");
	return -1;
    }
    list = parse(str, I(tp, 4), 0);
    if (!(sz = sizeof(list)))
    {
	tp->catch_tell("You don't have any edible '" + str + "'.\n");
	return -1;
    }
    out = "";
    for (i=0; i<sz; i++)
    {
	if (!list[i]->eat_cmd())
	    out += "You cannot eat " + list[i]->short() + ".\n";
    }
    tp->catch_tell(out);
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
