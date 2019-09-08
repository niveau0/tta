/* **************************************************************************
 * resurrect - resurrect own body
 */
nomask int
resurrect()
{
    int    l;
    string str;
    object tp, ob;

    CHECK_LEVEL(_BUILDER);
    
    tp = this_interactive();
    str = tp->query_name();
    if (!str || !(ob = find_player(str)))
    {
	tp->catch_tell("Cannot find your corpse.\n");
	return -1;
    }
    if (living(ob))
    {
	tp->catch_tell("You are not dead.\n");
	return -1;
    }
    ob->resurrect();
    return 1;
}
