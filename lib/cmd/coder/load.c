/* **************************************************************************
 * load - (re)load a file
 */
nomask int
load(string str)
{
    int    flag, len;
    object tp;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || !(len = strlen(str)))
    {
	tp->catch_tell("Usage: load <filename>\n");
	return -1;
    }
    if (len > 1 && str[len-2..] != ".c")
	str += ".c";
    str = FPATH(tp->query_path(), str);
    if (!str || !(len = strlen(str)))
    {
	tp->catch_tell("Invalid file name.\n");
	return -1;
    }
    if (file_size(str) < 0)
    {
	tp->catch_tell("No such file.\n");
	return -1;
    }
    str = str[..len-3];
    if (find_object(str))
	flag = 1;
    if (!catch(compile_object(str)))
    {
	if (flag)
	    tp->catch_tell("Already loaded, code upgraded.\n");
	else
	    tp->catch_tell("Loaded: " + str + "\n");
    }
    return 1;
}
