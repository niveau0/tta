/*
 * Function name: light
 * Description:   light an item (e.g. a torch)
 * Arguments:     str - what to light
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
light(string str, object tp)
{
    int    i;
    object ob, *list;

    if (!str)
    {
	tp->catch_tell("Usage: light <item>\n");
	return -1;
    }
    list = I(tp, 4);
    for (i=sizeof(list); i--;)
	if (list[i]->query_burning())
	    break;
    if (i < 0)
    {
	tp->catch_tell("You need tinder and stone or something already " +
		       "burning.\n");
	return -1;
    }
    list = list + tp->visible(I(E(tp), 1));
    list = parse(str, list, tp, tp->query_step());
    if (!check_size(list, "No '" + str + "' in range to light here.\n", tp))
	return -1;
    if (!list[0]->light_cmd())
    {
	tp->catch_tell("It cannot be lit.\n");
	return -1;
    }
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
