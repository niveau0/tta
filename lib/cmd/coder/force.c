/* **************************************************************************
 * force - force a player to do something
 */
nomask int
force(string str)
{
    string who, what;
    object ob, tp;

    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || !strlen(str))
    {
	tp->catch_tell("Usage: force <name> <command>\n");
	return -1;
    }
#ifdef _LOG_FORCE
    syslog("FORCE", BS(tp->query_name() + ": " + str, 76, 1));
#endif
    if (sscanf(str, "%s to %s", who, what) == 2 ||
	sscanf(str, "%s %s", who, what) == 2)
    {
	ob = find_living(who);
	if (!ob)
	{
	    tp->catch_tell("No such living.\n");
	    return -1;
	}

	if (ob->command(what))
	{
	    ob->catch_msg(({ QCTNAME(tp), " forced you to: " + what + "\n" }),
			  0, 0);
	    tp->catch_tell("Ok.\n");
	}
	else
	{
	    ob->catch_msg(({ QCTNAME(tp), " tried to force you to: " + what +
				 "\n" }), 0, 0);
	    tp->catch_tell("You failed.\n");
	}
	return 1;
    }
    return -1;
}
