/* **************************************************************************
 * tail - display the end a file
 */
nomask int
tail_file(string path)
{
    int    lines;
    object tp;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();

    if (!path || !strlen(path))
    {
	tp->catch_tell("Usage: tail [lines] <filename>\n");
	return -1;
    }
    sscanf(path, "%d %s", lines, path);
    path = FPATH(tp->query_path(), path);
    if (!path)
    {
	tp->catch_tell("Bad file name format.\n");
	return -1;
    }
    if ((file_size(path) < 0) && (file_size(path+".c") > 0)) 
      path = path+".c";
    
    if (file_size(path) < 0)
    {
	tp->catch_tell("No such file: " + path + "\n");
	return -1;
    }
    tail(path, lines);
    return 1;
}
