#include <access.h>

nomask void
create()
{
    if (program_name(this_object()) != object_name(this_object()))
	destruct_object();
}

/*
 * Function name: resolve_path
 * Description:   resolves a path
 * Arguments:     path - the path to resolve
 * Returns:       string, resolved path
 */
nomask string
resolve_path(string path)
{
    int    i, count, sz;
    string home, tail, *split;
    
    if (!path || !strlen(path))
	return "";
    if (path[0] == '~')
    {
	if (sscanf(path, "~%s/%s", home, tail) != 2)
	{
	    home = path[1..];
	    tail = "";
	}
	if ((home = _SYSTEMD->query_coder_path(home)) ||
	    /* try to get path of single '~' */
	    (home = _SYSTEMD->query_coder_path(geteuid(previous_object()))))
	    path = home + "/" + tail;
    }
    i = strlen(path)-1;
    if (path[i] == '/')
	path += ".";
    else if (path[i-1..] != "/.") 
    {
	split = get_dir(path);
	if (sizeof(split) == 1)
	{
	    while (i >= 0 && path[i] != '/') i--;

	    if (path[i+1..] != split[0])
	    {
		path = path[..i] + split[0];
		if (file_size(path) == -2)
		    path += "/.";
	    }
	    else if (file_size(path) == -2)
		path += "/.";
	}
    }
    
    split = explode(path, "/");
    if (!(sz = sizeof(split)))
	return "";
    for (i=count=0; i<sz; i++)
    {
	if (split[i] == "..")
	{
	    if (count > 0)
		count--;
	}
	else if ((split[i] != "." || i >= sz-1) && split[i] != "")
	    split[count++] = split[i];
    }
    return "/" + implode(split[..count-1], "/");
}

/*
 * Function name: fix_path 
 * Description:   Resolves a combined path and subpath
 * Arguments:	  path - pathname
 *                subpath - filename possibly with path
 * Returns:       string, combined path
 */
nomask string
fix_path(string path, string subpath) 
{
    if (!path || !subpath)
	return "";
    if (strlen(subpath) && (subpath[0] == '/' || subpath[0] == '~'))
	return resolve_path(subpath);
    if (strlen(path) && path[strlen(path)-1] != '/')
	path += "/";
    return resolve_path(path + subpath);
}

/*
 * Function name: tilde_path
 * Description:   Makes a 'tildepath' of a path if possible
 * Arguments:	  path - The path we are trying to make a 'tildepath' of.
 * Returns:       string
 */
nomask string
tilde_path(string path)
{
    string name, tail;

    if (!path || !strlen(path))
	return path;
    if (sscanf(path, "/home/%s/%s", name, tail) == 2 &&	_ACCESS_LEVEL(name))
	return "~" + name + "/" + tail;
    if (sscanf(path, "/home/%s", name) && _ACCESS_LEVEL(name))
	return "~" + name;
    return path;
}
