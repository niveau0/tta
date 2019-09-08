/* **************************************************************************
 * possess - possess a living object
 */
nomask int
possess(string arg)
{
    int    argc;
    string v_name, p_name, *argv;
    object tp, victim, usr1, usr2;

    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!arg || !stringp(arg))
    {
	tp->catch_tell("Possess what?\n");
	return -1;
    }
    argv = explode(arg, " ");
    argc = sizeof(argv);
    if (!argc)
    {
	tp->catch_tell("Possess what?\n");
	return -1;
    }
    if (argc == 1)
	v_name = lower_case(argv[0]);
    else
	v_name = lower_case(argv[1]);
    victim = present(v_name, E(tp));
    if ((!victim && !(victim = find_living(v_name))) || !living(victim))
    {
	tp->catch_tell("No living object with that name.\n");
	return -1;
    }
    if (victim == tp)
    {
	tp->catch_tell("Hey! You're already in your head.\n");
	return -1;
    }
    /* Only admin can break or force a possession */
    if (argc == 2)
    {
	if (_ACCESS_LEVEL(geteuid(tp)) < _ADMIN)
	{
	    tp->catch_tell("You are not allowed to break a possession.\n");
	    return -1;
	}
	if (argv[0] != "break")
	{
	    if (::break_possession(victim, tp))
		tp->catch_tell("You broke the possession.\n");
	    else
		tp->catch_tell("Failed.\n");
	    return 1;
	}
	tp->catch_tell("Wrong argument '" + argv[0] + "'.\n");
	return -1;
    }
    usr1 = tp->query_user();
    usr2 = victim->query_user();
	
    /* possess now */
    if (possession(victim, tp))
    {
	if (victim->query_npc())
	    victim->init_msg_flags();
	else if (usr2)
	    usr2->catch_tell("Suddenly something evil invades your mind.\n");
	usr1->catch_tell("You try to take control over " +
			 capitalize(victim->query_name()) + ".\n");
	return 1;
    }
    tp->catch_tell("Failed.\n");
    return -1;
}
