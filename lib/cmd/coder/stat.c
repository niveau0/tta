/* **************************************************************************
 * stat - displays various stats of someone
 */
nomask int
stat(string str)
{
    int    all;
    object ob, tp, *obj;

    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str)
    {
	tp->catch_tell("Usage: stat [-a] <living>\n");
	return -1;
    }
    
    if (str && sscanf(str, "-a %s", str))
	all = 1;
    
    if (!str || !strlen(str))
	ob = tp;
    else if (sizeof(obj = parse(str, I(E(tp), 0), tp)))
	ob = obj[0];
    else if (!(ob = find_living(str)) &&
	     !(ob = _SYSTEMD->finger_player(str)))
    {
	tp->catch_tell("No such living.\n");
	return -1;
    }
    if (!all && !living(ob))
    {
	tp->catch_tell("That's no living object.\n");
	return -1;
    }
    tp->catch_tell(ob->stat_living());
    return 1;
}
