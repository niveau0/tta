/*
 * Function name: pull
 * Description:   pull something if possible
 * Arguments:     str - what to pull
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
pull(string str, object tp)
{
    int    res;
    string of;
    object *list;

    if (!str)
    {
	tp->catch_tell("Usage: pull <item> [of <item>]\n");
	return -1;
    }
    sscanf(str, "%s of %s", str, of);
    list = tp->visible(I(E(tp), 0));
    list = parse(str, list, tp, tp->query_step());
    if (!check_size(list, "No '" + str + "' in range to pull here.\n", tp))
	return -1;
    if (!(res = list[0]->pull_cmd(of, tp)))
    {
	tp->catch_tell("You cannot pull that.\n");
	return -1;
    }
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    if (res < 0)
    {
	tp->catch_tell("You failed to pull " + list[0]->query_objective() +
		       ".\n");
	tell_objects(LISTEN(E(tp)) - ({ list[0], tp }),
		     ({ QCTNAME(tp), " failed to pull ", QNAME(list[0]),
			    ".\n" }), MSG_SEE, tp);
	if (living(list[0]))
	    list[0]->catch_msg(({ QCTNAME(tp), " failed to pull you.\n" }),
			       0, 0);
	return -1;
    }
    return 1;
}
