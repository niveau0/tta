/* **************************************************************************
 * manual - print and search source code information
 */
nomask int
manual(string str)
{
    int    i, j, k, szargs, sz, sz2, line, *flags;
    string out, s, e, file, *args, *fullpath, *dirs, *curr, **conts;
    object tp;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str)
    {
	tp->catch_tell("Usage: man <function> <function> ...\n"+
		       "E.g.: man add_item query_*\n"+
		       " All wildcarded (*) arguments will be "+
		       "interpreted as query\n for a list of "+
		       "available manuals, only none wildcarded\n"+
		       " will return the whole manual for the argument.\n"+
		       " Use: man * for all available subsections,\n"+
		       "      / as suffix for a list of their contents, " +
		       "try: man living/\n");
	return -1;
    }

    args = explode(str," ");
    flags = allocate(sizeof(args));

    for (i=sizeof(args); i--;) /* filter wildcards */
    {
	if (args[i] == "*" || !sscanf(args[i],"%*s*%*s"))
	    continue;
	args[i] = implode(explode("*" + args[i] + "*", "*"), "%*s");
	flags[i] = 1; /* mark argument as 'wildcarded' */
    }

    /* available subdirs */
    dirs = get_dir("/doc/man/") - ({ "SRC" });
    j = sizeof(dirs);
    dirs += get_dir("/doc/extraman/");
    
    for (i=sizeof(dirs), fullpath=allocate(i), conts=allocate(i); i--;)
    {
	if (i < j)
	    fullpath[i] = "/doc/man/" + dirs[i];
	else
	    fullpath[i] = "/doc/extraman/" + dirs[i];
	/* contents of subdirs */
	conts[i] = get_dir(fullpath[i] + "/*");
    }

    out = "";
    for (i=0, szargs=sizeof(args); i<szargs; i++)
    {
	str = "";
	if (args[i] == "*") /* list subsections */
	{
	    out += BS(implode(dirs,", "), 77, 0) + "\n\n";
	    continue;
	}
	for (j=0, sz=sizeof(conts); j<sz; j++)
	{
	    curr = conts[j];
	    if (sscanf(args[i], "%s/", s) && s == dirs[j]) 
	    {
		str += implode(curr, "\n"); /* get subsection contents */
		flags[i] = 1;
		continue;
	    }
	    for (k=0, sz2=sizeof(curr); k<sz2; k++)
	    {
		file = curr[k]; /* the actual help file */
		if (flags[i])
		{
		    if (sscanf(file, args[i]))
			str += dirs[j] + "/" + file + "\n";
		}
		else if (file == args[i]) /* test exact match */
		{
		    if (line)
			str += "-------------------------------------------" +
			       "----------------------------------\n";
		    str += read_file(fullpath[j] + "/" + file);
		    line++;
		}
	    }
	}
	out += "======================================================" +
	       "=======================\n";
	if (!strlen(str))
	    out += "No manuals for " + (i + 1) + ". argument.\n\n";
	else if (flags[i])
	    out += sprintf("%*76s", str);
	else
	    out += str;
    }
    smore(out, tp);
    return 1;
}
