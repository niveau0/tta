/* **************************************************************************
 * list - list files/directories
 */
nomask int
list(string str)
{
    string list, s1, s2;
    object tp;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str)
	str = tp->query_path();
    else if (strlen(str))
    {
	if (sscanf(str, "%s %s", s1, s2) == 2)
	    str = s1 + " " + FPATH(tp->query_path(), s2);
	else if (str[0] == '-')
	    str += " " + tp->query_path();
	else 
	    str = FPATH(tp->query_path(), str);
    }
    list = _LS(str);
    if (!list || !strlen(list))
	return -1;
    tp->catch_tell(list);
    return 1;
}
