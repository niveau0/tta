/* **************************************************************************
 * god - gozzip on the coder's line
 */
nomask int
god(string str)
{
    int    i, l;
    string name;
    object *list;

    CHECK_LEVEL(_BUILDER);

    if (!str || !strlen(str))
    {
	this_interactive()->catch_tell("Usage: @ <text>\n" +
				       "       god <text>\n");
	return -1;
    }

    name = capitalize(this_interactive()->query_name());
    list = users();
    l    = strlen(name);
    str  = "\n@ " + name + ":" + BS(str, 73 - l, l + 4)[l+3..] + "\n";
    for (i=sizeof(list); i--;)
	if (_ACCESS_LEVEL(geteuid(list[i])))
	    list[i]->catch_tell(str);
    return 1;
}
