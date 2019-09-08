/*
 * Function name: assist
 * Description:   assist someone in fight
 * Arguments:     str - whom to assist
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
assist(string str, object tp)
{
    string fail;
    object here, ob, *list;

    if (!str)
    {
	tp->catch_tell("Usage: assist <whom>\n");
	return -1;
    }
    here = E(tp);
    list = tp->visible(I(here, 2));
    list = parse(str, list, tp);
    if (!check_size(list, "No '" + str + "' to assist here.\n", tp))
	return -1;
    if (!(ob = list[0]->cb_query_attacker()))
    {
	tp->catch_tell(capitalize(list[0]->query_pronoun()) +
		       " is not fighting.\n");
	return -1;
    }
    if (tp == ob)
    {
	tp->catch_tell("Attacking yourself wouldn't be a good choice.\n");
	return -1;
    }
    if (tp->cb_query_attacker() == ob)
    {
	tp->catch_tell("Yes, yes.\n");
	return -1;
    }
    if (!tp->cb_no_melee_attack(ob))
    {
	tell_objects(LISTEN(here) - ({ tp, ob }),
		     ({ QCTNAME(tp), " attacks ", QTNAME(ob), ".\n" }),
		     MSG_SEE, tp);
	ob->catch_msg(({ QCTNAME(tp), " attacks you!\n" }), 0, 0);
    }
    tp->echo(({ "You attack ", QTNAME(ob), ".\n" }), 0, 0);
    tp->cb_attack_object(ob);
    return 1;
}
