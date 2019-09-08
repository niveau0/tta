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
		    "Options: f - show only files\n" +
		    "         d - show only directories\n" +
		    "         t - sort by file time\n" +
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
    if (file != "." && numf == 1 && file_size(path + files[0]) == -2)
    {
	path += files[0] + "/";
	if (!(numf = sizeof(files = get_dir(path))))
	{
	    /* empty directory, show only . & .. if long list */
	    out = "";
	    if (member_array("l", slices) > -1) 
	    {
		out += sprintf("total 2\n" +
			       "drwsrws---  1 mud     mud%10d %12s .\n" +
			       "drwsrws---  1 mud     mud%10d %12s ..\n" +
			       "0 Bytes.\n",
			       512, ctime(file_time(path + "."))[4..15],
			       512, ctime(file_time(path + ".."))[4..15]);
	    }
	    return out;
	}
    }
    if (!numf)
	return path + ": No such file or (empty) directory.\n";
    if (member_array("f", slices) > -1) /* show only files */
    {
	for (i=numf; i--;)
	    if (file_size(path + files[i]) == -2)
	    {
		files[i] = 0;
		numf--;
	    }
	files -= ({ 0 });
    }
    else if (member_array("d", slices) > -1) /* show only directories */
    {
	for (i=numf; i--;)
	    if (file_size(path + files[i]) != -2)
	    {
		files[i] = 0;
		numf--;
	    }
	files -= ({ 0 });
    }
    if (member_array("t", slices) > -1) /* sort by file time */
	files = sort_array(files, "sort_time", this_object());
    if (member_array("l", slices) > -1) /* show full info */
    {
	string  editor;
	mapping editors;
	
	editors = _DRIVER->load_file_editor(path);
	out = sprintf("total " + (numf+2) + "\n" + 
		      "drwsrws---  0 mud     mud%10d %12s .\n" +
		      "drwsrws---  0 mud     mud%10d %12s ..\n",
		      512, ctime(file_time(path + "."))[4..15],
		      512, ctime(file_time(path + ".."))[4..15]);
	
	for(i=0; i<numf; i++)
	{
	    size = file_size(path + files[i]);
	    if ((size = file_size(path + files[i])) == -2)
		out += sprintf("drwsrws---  0 mud     mud%10d %12s %s\n",
			       512,
			       ctime(file_time(path + files[i]))[4..15],
			       files[i]);
	    else if (editor = editors[files[i]])
	    {
		if (editor == _SYSTEM)
		    editor = "system";
		else if (editor == _ROOT)
		    editor = "root";
		out += sprintf("-rwsrws---  0 %-7s mud%10d %12s %s\n",
			       editor, size,
			       ctime(file_time(path + files[i]))[4..15],
			       files[i]);
	    }
	    else
		out += sprintf("-rwsrws---  0 mud     mud%10d %12s %s\n",
			       size,
			       ctime(file_time(path + files[i]))[4..15],
			       files[i]);
	    du += (size > 0 ? size : 0);
	}
	return out + du + " Bytes.\n";
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
