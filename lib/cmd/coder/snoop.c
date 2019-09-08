/* **************************************************************************
 * snoop - snoop someone
 */
nomask int
snoop_on(string str)
{
    int    i;
    string *argv;
    object on, by, tp, *u;
    mixed  ret;

    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || !strlen(str))
    {
	u = users();
	str = "";
	for (i=sizeof(u); i--;)
	{
	    if (by = query_snoop(u[i]))
		str += sprintf("%-20s snooped by %-20s\n",
			       capitalize(u[i]->query_name()),
			       capitalize(by->query_name()));
	}
	if (strlen(str))
	    tp->catch_tell(str);
	else
	    tp->catch_tell("Noone is snooped by anyone.\n");
	return 1;
    }

    argv = explode(str, " ");

    if (sizeof(argv) == 1)
    {
	if (!(on = find_player(argv[0])))
	{
	    tp->catch_tell("No such player.\n");
	    return -1;
	}
	if (objectp(ret = snoop(tp, on)))
	{
	    tp->catch_tell("You stopped the snoop.\n");
	    if (ret != tp)
		ret->catch_tell(capitalize(tp->query_name()) +
				" broke your snoop.\n");
	}
	else if (ret == 1)
	    tp->catch_tell("Snooping " + capitalize(argv[0]) + ".\n");
	else
	    tp->catch_tell("Failed to snoop " + capitalize(argv[0]) + ".\n");
	return 1;
    }
    by = find_player(argv[0]);
    on = find_player(argv[1]);
    if (!on || !by)
    {
	tp->catch_tell("No such players.\n");
	return -1;
    }
    if (snoop(by, on))
    {
	by->catch_tell("You are now snooping " + capitalize(argv[1]) + ".\n");
	tp->catch_tell(capitalize(argv[0]) + " is now snooping " +
		       capitalize(argv[1]) + ".\n");
    }
    else
	tp->catch_tell("Failed to set snoop on " + capitalize(argv[1]) +
		       " by " + capitalize(argv[0]) + ".\n");
    return 1;
}
