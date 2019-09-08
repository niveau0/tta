/*
 * Function name: alias
 * Description:   set/remove/modify aliases
 * Arguments:     str - the alias definition string
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
alias(string str, object tp)
{
    int     i, sz;
    string  abbrev, *args;
    mapping aliases;

    if (str == "-l")
    {
	if (!mappingp(aliases = tp->query_aliases()) ||
	    !(sz = map_sizeof(aliases)))
	{
	    tp->catch_tell("You have no aliases defined.\n");
	    return 1;
	}
	str = "Your current aliases are:\n";
	args = map_indices(aliases);
	for (i=0; i<sz; i++)
	    str += (sprintf("%-15s", args[i]) + " - " +
		    BS(aliases[args[i]], 77, 18)[18..] + "\n");
	tp->catch_tell(str);
	return 1;
    }
    if (str && sscanf(str, "-d %s", str))
    {
	tp->modify_alias(str, 0);
	tp->catch_tell("Ok, alias removed.\n");
	return 1;
    }
    if (str && sscanf(str, "%s %s", abbrev, str) == 2)
    {
	tp->modify_alias(abbrev, str);
	tp->echo("Ok, new alias defined.\n");
	return 1;
    }
    tp->catch_tell("Usage: alias <abbrev> <command>\n" +
		   "       alias -l\n" +
		   "       alias -d <abbrev>\n");
    return -1;
}
