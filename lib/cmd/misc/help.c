/*
 * Function name: help
 * Description:   get help about commands
 * Arguments:     str - for what to get help, nothing for general help
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
help(string str, object tp)
{
    int    i;
    string main, out, *args, *ind;
    mixed  path;

    main = HELPPATH;
    if (!str)
    {
	more(main + "help.doc", tp);
	return 1;
    }
    if (str == "topics")
    {
	out = "Available help topics:\n";
	if (_ACCESS_LEVEL(geteuid(tp)) > 0)
	{
	    ind = get_dir(main + "creator/*.doc");
	    for (i=sizeof(ind); i--;)
		ind[i] = ind[i][..strlen(ind[i])-5];
	    out += sprintf("% : *78s\n", implode(ind, "\n"));
	}
	out += sprintf("% : *78s\n", implode(map_indices(helpindex), "\n"));
	smore(out, tp);
	return 1;
    }
    if (IN_ARRAY(str, get_dir(main + "*")))
    {
	if (str == "creator" && _ACCESS_LEVEL(geteuid(tp)) < 1)
	{
	    tp->catch_tell("No help available for that topic.\n");
	    return -1;
	}
	args = get_dir(main + str + "/*.doc");
	for (i=sizeof(args); i--;)
	{
	    args[i] = args[i][..strlen(args[i])-5];
	    args[i] = implode(explode(args[i], "_"), " ");
	}
	out = sprintf("% : *78s\n", implode(args, "\n"));
	tp->catch_tell(capitalize(str) + ", available topics:\n" + out);
	return 1;
    }
    if (!(path = helpindex[str]))
    {
	if (_ACCESS_LEVEL(geteuid(tp)) < 1 ||
	    file_size(main + (path = "creator/") + str + ".doc") < 1)
	{
	    tp->catch_tell("No help available for that topic.\n");
	    return -1;
	}
    }
    str = implode(explode(str, " "), "_");
    if (pointerp(path))
    {
	for (out="", i=sizeof(path); i--;)
	    out += read_file(main + path[i] + str + ".doc") + "\n\n";
	smore(out, tp);
	return 1;
    }
    more(main + path + str + ".doc", tp);
    return 1;
}
