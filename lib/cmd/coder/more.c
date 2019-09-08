/* **************************************************************************
 * more - display the contents of a file
 */
nomask int
more_file(string path)
{
    int    line;
    object tp;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    
    if (!path || !strlen(path))
    {
	tp->catch_tell("Usage: more [line] <filename>\n");
	return -1;
    }
    sscanf(path, "%d %s", line, path);
    path = FPATH(tp->query_path(), path);
    if (!path || !strlen(path))
    {
	tp->catch_tell("Bad file name format.\n");
	return -1;
    }
    if (file_size(path) < 0)
    {
	tp->catch_tell("No such file: " + path + "\n");
	return -1;
    }
    tp->catch_tell(" --- showing file " + path + "\n");
    more(path, tp, 0, line);
    return 1;
}
