/* **************************************************************************
 * whereis - find the cmdsoul of a command 
 */
nomask int
whereis(string str)
{
    int    i;
    string out, *souls;
    object tp;

    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str)
    {
	tp->catch_tell("Usage: whereis <command>\n");
	return -1;
    }
    if (!sizeof(souls = tp->query_cmdsoul_list()))
	souls = ({ });
    souls += ({ CODER_SOUL });
    if (!(i = sizeof(souls)))
    {
	tp->catch_tell("Command not found.\n");
	return -1;
    }
    out = "";
    while (i--)
    {
	if (souls[i]->query_cmdlist()[str])
	    out += souls[i] + "\n";
    }
    if (out == "")
    {
	tp->catch_tell("Command not found.\n");
	return -1;
    }
    tp->catch_tell("Command found in:\n" + out);
    return 1;
}
