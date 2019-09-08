/*
 * Function name: smash
 * Description:   try to destroy something
 * Arguments:     str - what to smash 
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
smash(string str, object tp)
{
    string with;
    object ob, wob, *list;

    if (!str)
    {
	tp->catch_tell("Usage: smash <item> [with <item>]\n");
	return -1;
    }
    sscanf(str, "%s with %s", str, with);
    list = I(tp, 4) + tp->visible(I(E(tp), 1));
    list = parse(str, list, tp, tp->query_step());
    if (!check_size(list, "No '" + str + "' in range to smash here.\n", tp))
	return -1;
    ob = list[0];
    if (with)
    {
	if (!sizeof(list = parse(str, I(tp, 4), 0, 0)))
	{
	    tp->catch_tell("You have no such item.\n");
	    return -1;
	}
	wob = list[0];
    }
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    if (!ob->smash_cmd(wob, tp))
    {
	tp->catch_tell("Nothing happened.\n");
	return -1;
    }
    return 1;
}
