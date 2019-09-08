/* **************************************************************************
 * paths - set paths to search for objects
 */
nomask int
paths(string str)
{
    int    i, sz;
    string *cur, *new;
    object tp;

    tp = this_interactive();
    if (!str)
    {
	tp->catch_tell("Usage:   paths <directory 1>,<directory 2>,...\n"+
		       "         prefix '-' removes a directory.\n"+
		       (sizeof(cur = tp->query_prop(CODER_PATH))
			? "Current:\n" + BS(implode(cur, ", "), 60, 9) + "\n"
			: ""));
	return 1;
    }
    if (!pointerp(cur = tp->query_prop(CODER_PATH)))
	cur = ({ });
    new = explode(str, ",");
    for (i=0, sz=sizeof(new); i<sz; i++)
    {
	if (new[i][0] == '-')
	    cur -= ({ new[i][1..] });
	else 
	    cur |= ({ new[i] });
    }
    tp->add_prop(CODER_PATH, cur);
    tp->catch_tell("Ok.\n");
    return 1;
}
