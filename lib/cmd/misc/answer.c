/*
 * Function name: answer
 * Description:   answer a formlery asked question
 * Arguments:     str - whom and what to answer
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
answer(string str, object tp)
{
    string whom;
    object here, ob, tar, *list;
    
    if (!str || sscanf(str, "%s %s", whom, str) != 2)
    {
	tp->catch_tell("Usage: answer <whom> <what>?\n");
	return -1;
    }
    here = E(tp);
    list = tp->visible(I(here, 2));
    list = parse(whom, list, tp);
    if (!check_size(list, "No '" + whom + "' to answer here.\n", tp))
	return -1;
    tar = list[0];
    if (!objectp(ob = tar->query_prop(LIVE_LAST_QUESTION)))
    {
	tp->catch_tell("There was no question.\n");
	return -1;
    }
    tp->echo("You answer: " + str + "\n");
    tell_objects(LISTEN(here) - ({ tp, tar }),
		 ({ QCTNAME(tp), " says to ", QTNAME(tar), ": " +
			str + "\n" }), MSG_HEAR, tp, 2000);
    if (CAN_HEAR(tar, tp, 2000))
    {
	tar->catch_msg(({ QCTNAME(tp), " answers: " + str + "\n" }), tp, 
		       MSG_HEAR, 2000);
	ob->answer_cmd(str, tp);
	tar->add_prop(LIVE_LAST_QUESTION, 0);
    }
    return 1;
}
