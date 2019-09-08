/* **************************************************************************
 * cstat - displays combatstats of someone
 */
nomask int
cstat(string str)
{
    object ob, tp, *obj;

    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || !strlen(str))
	ob = tp;
    else if (sizeof(obj = parse(str, I(E(tp), 2), 0)))
	ob = obj[0];
    else if (!(ob = find_living(str)))
    {
	tp->catch_tell("No such living.\n");
	return -1;
    }
    if (!living(ob))
    {
	tp->catch_tell("That's no living object.\n");
	return -1;
    }
    tp->catch_tell(ob->combatstat());
    return 1;
}
