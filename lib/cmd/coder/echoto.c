/* **************************************************************************
 * echoto - echo something to someone
 */
nomask int
echo_to(string str)
{
    string who, msg;
    object ob, tp;

    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || sscanf(str, "%s %s", who, msg) != 2)
    {
	tp->catch_tell("Usage: echoto <text>\n");
	return -1;
    }
    ob = find_living(lower_case(who));
    if (!ob)
    {
	tp->catch_tell("No living with that name.\n");
	return -1;
    }
    msg = implode(explode(msg, "@@"), "#");
    ob->catch_tell(msg + "\n");
    tp->catch_tell("You echo: " + msg + "\n");
#ifdef _LOG_ECHO
    syslog("ECHO", BS(tp->query_name() + " echoto: " + str + " to: " + who,
		      76, 1));
#endif
    return 1;
}
