/* **************************************************************************
 * money - create some money
 */
nomask int
money(string str)
{
    int    v;
    object tp;

    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || !sscanf(str, "%d", v))
    {
	tp->catch_tell("Usage: money <amount in copperbit>\n");
	return -1;
    }

    MONEY_CHANGE(v, tp);
    tp->catch_tell("Ok.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " summons some money.\n" }), MSG_SEE, tp);
    return 1;
}
