/* **************************************************************************
 * rmdir - delete a directory
 */
nomask int
removedir(string str)
{
    int    i;
    object tp;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || !strlen(str))
    {
	tp->catch_tell("Usage: rm <directory>\n");
	return -1;
    }
    str = FPATH(tp->query_path(), str);
    i = strlen(str)-1;
    while (str[i] != '/') --i;
    if (remove_dir(str[..i]))
	tp->catch_tell("Ok.\n");
    else
	tp->catch_tell("Fail.\n");
    return 1;
}
