/* **************************************************************************
 * cd - change current directory
 */
nomask int
cd(string str)
{
    int    len;
    string new_path, file, *split;
    object tp, env, obj, *list;

    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || !strlen(str))
	str = _SYSTEMD->query_coder_path(geteuid(tp));
    
    if (str[0] == '-')
    {
	env = E(tp);
	str = str[1..];

	if (str == "here")
	    split = explode(env->query_room_file(), "/");
	else if (sizeof(list = parse(str, I(env, 0), tp)) ||
		 sizeof(list = parse(str, I(tp, 7), tp)))
	{
	    obj = list[0];
	    split = explode(object_name(obj), "/");
	}
	else
	{
	    tp->catch_tell("Usage: cd <directory>|-here|-<objectname>\n");
	    return -1;
	}
	file = implode(split[..(sizeof(split)-2)], "/");
	tp->catch_tell("Switching to /" + file + ".\n");
	tp->set_path("/" + file);
	return 1;
    }

    new_path = FPATH(tp->query_path(), str);
    if (!new_path || !strlen(new_path))
    {
	tp->catch_tell("Bad pathname format: " + str + "\n");
	return -1;
    }
    if (!_ACCESS_READ(new_path, tp, "cd"))
    {
	tp->catch_tell("Unaccessible directory: " + new_path + "\n");
	return -1;
    }
    if (file_size(new_path) != -2)
    {
	tp->catch_tell("Not a directory: " + new_path + "\n");
	return -1;
    }
    if (new_path[strlen(new_path)-1] == '/')
	new_path += ".";
    else if (new_path[strlen(new_path)-2..] != "/.")
	new_path += "/.";
    tp->catch_tell("Switching to " + new_path + "\n");
    tp->set_path(new_path);
    return 1;
}
