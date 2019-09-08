/* **************************************************************************
 * access - print fileaccess rights
 */
nomask int
access(string str)
{
    object tp;

    CHECK_LEVEL(_BUILDER);
    
    tp = this_player();
    if (!str)
	str = tp->query_name();
    if (!_SYSTEMD->exist_player(str))
    {
	tp->catch_tell("No such player.\n");
	return -1;
    }
    str = _ACCESSD->show_access(str);
    if (!str || !strlen(str))
    {
	tp->catch_tell("No read or write permissions.\n");
	return 1;
    }
    str = "Read write permissions (r read, w write, + access granted, " +
	  "- access denied)\n" + str;
    tp->catch_tell(str);
    return 1;
}
