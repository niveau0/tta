/* **************************************************************************
 * echo - print a message to the room
 */
nomask int
echo(string str)
{
    object tp;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_player();
    if (!str || !strlen(str))
    {
	tp->catch_tell("Usage: echo <text>\n");
	return -1;
    }
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 str + "\n", 0, 0);
    tp->catch_tell("You echo: " + str + "\n");
#ifdef _LOG_ECHO
    syslog("ECHO", BS(tp->query_name() + " echo: " + str, 76, 1));
#endif
    return 1;
}
