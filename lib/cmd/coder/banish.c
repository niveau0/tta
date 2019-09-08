/* **************************************************************************
 * banish - banish a name
 */
nomask int
banish(string arg)
{
    int    lev;
    string what, name, *argv;
    object tp;
    mixed  ret;

    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!arg || !strlen(arg))
    {
	tp->catch_tell("Usage: banish -i|-r|-a <name>\n");
	return -1;
    }
    argv = explode(arg, " ");
    if (sizeof(argv) == 2)
    {
	what = argv[0];
	name = argv[1];
    }
    else
    {
	tp->catch_tell("Usage: banish -i|-r|-a <name>\n");
	return -1;
    }
    
    if (_SYSTEMD->exist_player(name))
    {
	tp->catch_tell(capitalize(name) + " is a player in the game.\n");
	return -1;
    }

    lev = _ACCESS_LEVEL(geteuid(tp));
    
    switch (what)
    {
      case "-a":
      case "-add":
	/*
	 * Only admin may banish.
	 */
	if (lev < _ADMIN)
	{
	    tp->catch_tell("You are not allowed to banish players.\n");
	    return -1;
	}
	    
	ret = _SYSTEMD->banish(name, 2);
	
	if (sizeof(ret))
	    tp->catch_tell(capitalize(name) + " was banished by " + 
			   capitalize(ret[0]) + " at " +
			   ctime(ret[1]) + ".\n");
	else
	    tp->catch_tell("You just banished: " + capitalize(name) + ".\n");
	return 1;
	
      case "-i":
      case "-info":
	ret = _SYSTEMD->banish(name, 0);
	if (sizeof(ret))
	    tp->catch_tell(capitalize(name) + " was banished by " + 
			   capitalize(ret[0]) + " at " +
			   ctime(ret[1]) + ".\n");
	else
	    tp->catch_tell(capitalize(name) + " has not been banished.\n");
	return 1;
	
      case "-r":
      case "-remove":
	/*
	 * Only admin may remove banishment.
	 */
	if (lev < _ADMIN)
	{
	    tp->catch_tell("You are not allowed to remove banishment.\n");
	    return -1;
	}
	ret = _SYSTEMD->banish(name, 1);

	if (sizeof(ret))
	{
	    if (ret[0] != tp->query_name())
	    {
		tp->catch_tell("You have not banished " + 
			       capitalize(name) + ".\n");
		return -1;
	    }
	}
	else
	{
	    tp->catch_tell(capitalize(name) + " has not been banished.\n");
	    return -1;
	}
	tp->catch_tell("Ok.\n");
	return 1;
    }
    tp->catch_tell("Wrong option, possible are: add, info or remove\n");
    return -1;
}
