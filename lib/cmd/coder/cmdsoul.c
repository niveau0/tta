/* **************************************************************************
 * cmdsoul - affect your command souls
 */
nomask int
cmdsoul(string str)
{
    string *cmdsoul_list;
    object tp;

    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    cmdsoul_list = tp->query_cmdsoul_list();
    if (!str || !strlen(str))
    {
	tp->catch_tell("Current tools: " +
		       BS(implode(cmdsoul_list, ", "), 76, 15)[15..] + "\n");
	return 1;
    }
    str = FPATH(tp->query_path(), str);
    if (IN_ARRAY(str, cmdsoul_list))
    {
	if (tp->remove_cmdsoul(str))
	    tp->catch_tell("Cmdsoul removed.\n");
	else
	    tp->catch_tell("Failed.\n");
	return 1;
    }
    if (tp->add_cmdsoul(str))
	tp->catch_tell("Cmdsoul added.\n");
    else
	tp->catch_tell("Failed.\n");
    return 1;
}
