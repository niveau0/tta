/*
 * Function name: analyse
 * Description:   analyse an item
 * Arguments:     str - what to analyse
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
analyse(string str, object tp)
{
    int    i, sz;
    object *list;

    if (!str)
    {
	tp->catch_tell("Usage: analyse <item>\n");
	return -1;
    }
    list = tp->visible(I(tp, 7));
    list = parse(str, list, tp);
    if (!(sz = sizeof(list)))
    {
	tp->catch_tell("Usage: analyse <item>\n");
	return -1;
    }
    str = "";
    for (i=0; i<sz; i++)
    {
	if (!list[i]->analyse_cmd(tp))
	    str += "You can't find out anything special about the " +
		   list[i]->short() + ".\n";
    }
    tp->catch_tell(str);
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
