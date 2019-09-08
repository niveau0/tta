/*
 * Function name: attack
 * Description:   attack someone
 * Arguments:     str - whom to attack
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
attack(string str, object tp)
{
    string fail;
    object here, *list;

    if (!str)
    {
	tp->catch_tell("Usage: attack <whom>\n");
	return -1;
    }
    here = E(tp);
    list = tp->visible(I(here, 2) - ({ tp }));
    list = parse(str, list, tp);
    if (!check_size(list, "No '" + str + "' to attack here.\n", tp))
	return -1;
    if (tp->cb_query_attacker() == list[0])
    {
	tp->catch_tell("Yes, yes.\n");
	return -1;
    }
    if (!tp->cb_no_melee_attack(list[0]))
    {
	tell_objects(LISTEN(here) - ({ tp, list[0] }),
		     ({ QCTNAME(tp), " attacks ", QTNAME(list[0]), ".\n" }),
		     MSG_SEE, tp);
	list[0]->catch_msg(({ QCTNAME(tp), " attacks you!\n" }), 0, 0);
    }
    tp->echo(({ "You attack ", QTNAME(list[0]), ".\n" }), 0, 0);
    tp->cb_attack_object(list[0]);
    return 1;
}
