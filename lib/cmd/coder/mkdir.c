/* **************************************************************************
 * mkdir - make a directory
 */
nomask int
makedir(string str)
{
    object tp;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || !strlen(str))
    {
	tp->catch_tell("Usage: mkdir <directory>\n");
	return -1;
    }
    if (make_dir(FPATH(tp->query_path(), str)))
	tp->catch_tell("Ok.\n");
    else
	tp->catch_tell("Failed.\n");
    return 1;
}
