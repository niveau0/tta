/* **************************************************************************
 * tellall - tell something to everyone
 */
nomask int
tellall(string str)
{
    int    i, sz;
    string name;
    object tp, *list;

    tp = this_interactive();
    if (!str || !strlen(str))
    {
	tp->catch_tell("Usage: tellall <text>\n");
	return -1;
    }
    name = capitalize(tp->query_name());
    list = users() - ({ tp });
    for (i=0, sz=sizeof(list); i<sz; i++)
	list[i]->catch_tell(name + " tells everyone: " + str + "\n");

    tp->catch_tell("You tell everyone: " + str + "\n");
#ifdef _LOG_ECHO
    syslog("ECHO", BS(name + " tellall: " + str, 76, 1));
#endif
    return 1;
}
