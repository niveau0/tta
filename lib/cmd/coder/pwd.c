/* **************************************************************************
 * pwd - print working directory
 */
nomask int
pwd(string str)
{
    object tp;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (str == "-r")
	tp->catch_tell(TPATH(tp->query_path()) + "\n");
    else
	tp->catch_tell(tp->query_path() + "\n");
    return 1;
}
