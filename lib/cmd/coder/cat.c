/* **************************************************************************
 * cat - display the contents of a file
 */
nomask int
cat_file(string path)
{
    object tp;
    
    CHECK_LEVEL(_BUILDER);
    
    path = FPATH((tp = this_interactive())->query_path(), path);
    if (!path || !strlen(path))
    {
	tp->catch_tell("Bad file name format.\n");
	return -1;
    }
    if (!cat(path))
    {
	tp->catch_tell("No such file: " + path + "\n");
	return -1;
    }
    return 1;
}
