/* **************************************************************************
 * loaddir - load a directory
 */
nomask int
loaddir(string str)
{
    int    i, sz, l;
    string curr, *conts;
    object ob, tp;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || !strlen(str))
	str = tp->query_path();
    else 
	str = FPATH(tp->query_path(), str);
    
    if (file_size(str) != -2)
    {
	tp->catch_tell("Invalid directory name.\n");
	return -1;
    }
    if (str[strlen(str)-1] != '/')
	str += "/";
    conts = get_dir(str);
    if (!(sz = sizeof(conts)))
    {
	tp->catch_tell("No files to load.\n");
	return -1;
    }
    for (i=0; i<sz; i++)
    {
	curr = conts[i];
	if ((l = strlen(curr)) < 3 || curr[l-2..] != ".c")
	    continue;
	curr = str+curr[..l-3];
	if (ob = find_object(curr))
	{
	    tp->catch_tell(curr + " is already loaded.\n");
	    continue;
	}
	if (!catch(compile_object(curr)))
	    tp->catch_tell(curr + " is now loaded.\n");
    }
    return 1;
}
