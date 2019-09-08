/* **************************************************************************
 * indent - indent a file
 */
nomask int
indent(string str)
{
    object tp;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || !strlen(str))
    {
	tp->catch_tell("Usage: indent <file>\n");
	return -1;
    }
    tp->catch_tell(cindent(FPATH(tp->query_path(), str)));
    return 1;
}
