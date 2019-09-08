/* ls command */

#include <access.h>
#include <filepath.h>
#include <kernel.h>

private string ls_path;

static nomask void
create()
{
    seteuid(0);
}

private string
show_fname(string path, string filename)
{
    if (filename[strlen(filename)-2..] == ".c")
    {
	if (find_object(path + filename[..strlen(filename) - 3]))
	    return "*" + filename;
	return "-" + filename;
    }
    return " " + filename;
}

static nomask int
sort_time(string a, string b)
{
    return file_time(ls_path + a) > file_time(ls_path + b);
}

nomask string
ls(string path)
{
    int    i, sz, size, x, numf, du;
    string options, file, out, *slices, *files;
    
    seteuid(_ROOT);
    seteuid(geteuid(previous_object()));
    options = "";
    if (!path || path == "")
	path = ".";
    else
    {
	for (i=0, sz=sizeof(slices = explode(path, " ")); i<sz; i++)
	{
	    if (strlen(slices[i]) && slices[i][0] == '-')
		options += slices[i][1..];
	    else
		break;
	}
	if (strlen(options) == 1 && options == "h")
	{
	    return ("Usage: ls [-<options>] [<path>]\n" +
		    "Options: t - sort by file time\n" +
		    "         l - show long info\n" +
		    "         h - this help\n");
	}
	if (i == sz)
	    path = ".";
	else
	    path = slices[sz-1];
    }
    path = RPATH(path);
    if (!path || !(sz = strlen(path)))
	return "";
    if (!(_ACCESS_READ(path, this_object(), "ls")))
	return "No read access.\n";
    
    numf = sizeof(files = get_dir(path)); /* get matches */
    for (i=strlen(path); i--;)
	if (path[i] == '/')
	{
	    file = path[i+1..];
	    path = path[..i];
	    break;
	}
    slices = explode(options, "");
    if (!numf)
    {
	if (file_size(path) == -2 && member_array("l", slices) > -1) 
	{
	    /* empty directory, show only . & .. if long list */
	    if (file == "." || file == "*")
	    {
		out = sprintf("total 2\n" +
			      "drwsrws---   1 mud      mud%14d %12s .\n" +
			      "drwsrws---   1 mud      mud%14d %12s ..\n" +
			      "0 Bytes.\n",
			      512, ctime(file_time(path + "."))[4..15],
			      512, ctime(file_time(path + ".."))[4..15]);
	    }
	    else
		out = "total 0\n";
	    return out;
	}
	return path + file + ": File not found.\n";
    }
    if (member_array("t", slices) > -1) /* sort by file time */
	files = sort_array(files, "sort_time", this_object());
    if (member_array("l", slices) > -1 || member_array("L", slices) > -1) /* show full info */
    {
	int     year;
	string  editor, timestr;
	mapping editors;
	
	editors = _DRIVER->load_file_editor(path);
	if (file == "." || file == "*")
	{
	    out = sprintf("total " + (numf+2) + "\n" + 
			  "drwsrws---   1 mud      mud%14d %12s .\n" +
			  "drwsrws---   1 mud      mud%14d %12s ..\n",
			  512, ctime(file_time(path + "."))[4..15],
			  512, ctime(file_time(path + ".."))[4..15]);
	}
	else
	    out = "";
	
	year = (int)ctime(time())[20..];
	for(i=0; i<numf; i++)
	{
	    timestr = ctime(file_time(path + files[i]));
	    if ((int)timestr[20..] < year)
		timestr = timestr[4..10] + timestr[19..];
	    else
		timestr = timestr[4..15];
	    
	    size = file_size(path + files[i]);
	    if ((size = file_size(path + files[i])) == -2)
		out += sprintf("drwsrws---   1 mud      mud%14d %12s %s\n",
			       512, timestr, files[i]);
	    else if (editor = editors[files[i]])
	    {
		if (editor == _SYSTEM)
		    editor = "system";
		else if (editor == _ROOT)
		    editor = "root";
		out += sprintf("-rwsrws---   1 %-8s mud%14d %12s %s\n",
			       editor, size, timestr, files[i]);
	    }
	    else
		out += sprintf("-rwsrws---   1 mud      mud%14d %12s %s\n",
			       size, timestr, files[i]);
	    du += (size > 0 ? size : 0);
	}
	if (file == "." || file == "*")
	    return out + du + " Bytes.\n";
	return out;
    }
    for (i=0; i<numf; i++)
    {
	file = path + files[i];
	if (file_size(file) == -2)
	    files[i] += "/";
	else if (file[strlen(file)-2..] == ".c")
	{
	    file = file[..strlen(file)-3];
	    if (find_object(file))
		files[i] += "*";
	}
    }
    if (member_array("1", slices) > -1) /* one line each file */
	return implode(files, "\n");
    return sprintf("%*77s", implode(files, "\n"));
}
