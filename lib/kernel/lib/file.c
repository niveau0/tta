/*
 * Function name: dump_state
 * Description:   create a state dump of the memory
 */
static nomask void
dump_state()
{
    if (getuid() != _ROOT)
	error("Illegal call to dump_state");
    rlimits(-1; -1)
    {
	::dump_state();
    }
}

/*
 * Function name: file_size
 * Description:   gives file size in bytes 
 * Arguments:     file - filepath
 * Returns:       integer
 */
static nomask int
file_size(string file)
{
    int *sizes;

    if (!file)
	BADARG(1, file_size);

    sizes = ::get_dir(file)[1];
    if (sizeof(sizes) != 1) 
	return -1;
    return sizes[0];
}

/*
 * Function name: syslog
 * Description:   log info in /syslog
 * Arguments:     file - file name (added to /syslog)
 *                str - string to write in log file
 */
static nomask void
syslog(string file, string str)
{
    int    *sizes;

    if (!file)
	BADARG(1, syslog);
    if (!str)
	BADARG(2, syslog);
    if (sscanf(file, "%*s/") || strlen(file) > 30) 
	error("Illegal file name to syslog: " + file);
    file = "/syslog/" + file;
    sizes = ::get_dir(file)[1];
    if (sizeof(sizes) == 1 && sizes[0] >= _LOG_SIZE)
	::rename_file(file, file + "." + sizeof(::get_dir(file + ".*")[0]));
    str = ctime(time()) + " " + object_name(this_object()) + "\n" +
	  str + "\n";
    ::write_file(file, str);
}

/*
 * Function name: log
 * Description:   log info in /log
 * Arguments:     file - file name (added to /log)
 *                str - string to write in log file
 */
static nomask void
log(string file, string str)
{
    int    *sizes;

    if (!file)
	BADARG(1, log);
    if (!str)
	BADARG(2, log);
    if (sscanf(file, "%*s/") || strlen(file) > 30) 
	error("Illegal file name to log: " + file);
    file = "/log/" + file;
    sizes = ::get_dir(file)[1];
    if (sizeof(sizes) == 1 && sizes[0] >= _LOG_SIZE)
	::rename_file(file, file + "." + sizeof(::get_dir(file + ".*")[0]));
    str = ctime(time()) + " " + object_name(this_object()) + "\n" +
	  str + "\n";
    ::write_file(file, str);
}

/*
 * Function name: restore_object
 * Description:   read object data file
 * Arguments:     file - file path (without .o suffix)
 * Returns:       1 if success, 0 else
 */
static nomask int
restore_object(string file)
{
    if (!file)
	BADARG(1, restore_object);

    return ::restore_object(file + ".o");
}

/*
 * Function name: save_object
 * Description:   save object to data file
 * Arguments:     file - file path (without .o suffix)
 * Returns:       1 if success, 0 else
 */
static nomask void
save_object(string file)
{
    if (!file)
	BADARG(1, save_object);

    if (!_ACCESS_WRITE(file, this_object(), "save_object"))
	error("Illegal use of save_object");
    ::save_object(file + ".o");
}

/*
 * Function name: editor
 * Description:   send an editor command to the DGD builtin 'ed'.
 * Arguments:     cmd - command to send
 * Returns:       string
 */
static nomask string
editor(string cmd)
{
    if (previous_program() != _INDENT &&
	previous_program() != _EDITOR)
	error("Illegal call to editor");
    return ::editor(cmd);
}

/*
 * Function name: cindent
 * Description:   indent lpc file
 * Arguments:     file - filepath
 * Returns:       string
 */
static nomask string
cindent(string file)
{
    return _INDENT->indent(file);
}

/*
 * Function name: file_time
 * Description:   gives file time
 * Arguments:     file - filepath
 * Returns:       integer
 */
static nomask int
file_time(string file)
{
    int *sizes;

    if (!file)
	BADARG(1, file_time);

    sizes = ::get_dir(file)[2];
    if (sizeof(sizes) != 1) 
	return -1;
    return sizes[0];
}

/*
 * Function name: get_dir
 * Description:   read directory contents
 * Arguments:     file - directory path to read
 * Returns:       array, directory contents
 */
static nomask string *
get_dir(string file)
{
    int len;

    if (!file)
	BADARG(1, get_dir);
    if (!_ACCESS_READ(file, this_object(), "get_dir"))
	return ({ });

    len = strlen(file);
    if (len < 1 || file[len-1] == '/') 
	file += "*";
    else if (len > 1 && file[len-2 ..] == "/.") 
	file[len - 1] = '*';
    
    return ::get_dir(file)[0] - ({ _EDITORLOG });
}

/*
 * Function name: make_dir
 * Description:   create directory
 * Arguments:     file - directory path
 * Returns:       1 if success, 0 else
 */
static nomask int
make_dir(string file)
{
    if (!file)
	BADARG(1, make_dir);
    return (_ACCESS_WRITE(file, this_object(), "make_dir") &&
	    ::make_dir(file));
}

/*
 * Function name: read_lines
 * Description:   read a file and split into lines
 * Arguments:     file - filepath
 *                from - line where to start reading
 *                num - number of lines to read
 * Returns:       array, lines
 */
static nomask string *
read_lines(string file, int from, int num)
{
    int    line, offset, size, ftime, *saved;
    string str, *lines;
    object user;

    if (!file)
	BADARG(1, read_lines);
    if (from < 1)
	BADARG(2, read_lines);
    if (num < 1)
	BADARG(3, read_lines);

    if (!_ACCESS_READ(file, this_object(), "read_lines"))
	return 0;
    
    line  = 1;
    ftime = file_time(file);
    
    if ((user = this_player()) && (user = user->query_user()))
    {
	saved = user->query_file_offset(file);
	if (saved != 0 && saved[2] == ftime)
	{
	    line = saved[0];
	    if (from < line / 2)
		/* start reading from beginnig */
		line = 1;
	    else
		/* start reading from offset */
		offset = saved[1]; 
	}
    }
    /* now search the 'from' line */
    while (1)
    {
	if (line == from) /* found the line */
	{
	    if (!(str = ::read_file(file, offset, _FILE_CHUNK)))
		return 0;
	    /* add a newline (empty lines must count too) */
	    lines = explode("\n" + str, "\n");
	    if (!(size = sizeof(lines)))
		return 0;
    	    if (user)
	    {
		/* store new file offset */
		rlimits(-1; -1)
		{
		    user->set_file_offset(file, line, offset, ftime);
		}
	    }
	    if (size == 1) /* only one line read */
	    {
		if (strlen(str) < _FILE_CHUNK)
		    return lines;
		error("Line too long");
	    }
	    if (size < num) /* not enough lines read */
	    {
		if (strlen(str) < _FILE_CHUNK) /* ok, if at file end */
		    return lines;
		error("Line range too large");
	    }
	    return lines[..num-1];
	}
	
	if (line < from) /* forward searching */
	{
	    if (!(str = ::read_file(file, offset, _FILE_CHUNK)))
		return 0;
	    /* add a newline (empty lines must count too) */
	    lines = explode("\n" + str, "\n");
	    if (!(size = sizeof(lines)))
		return 0;
	    if (line + size > from) /* passed the searched line */
	    {
		offset += strlen(implode(lines[..(from - line - 1)],
					 "\n")) + 1; 
		line = from;
	    }
	    else
	    {
		/* last 'lines' element might be no full line */
		offset += strlen(implode(lines[..size-2], "\n")) + 1;
		line += size - 1;
	    }
	    continue;
	}
	
	/* backward searching */
	offset -= _FILE_CHUNK;
	if (offset < 0)
	{
	    offset = 0;
	    line = 1;
	    continue;
	}
	if (!(str = ::read_file(file, offset, _FILE_CHUNK)))
	    return 0;
	/* add a newline (empty lines must count too) */
	lines = explode("\n" + str, "\n");
	if (!(size = sizeof(lines)))
	    return 0;
	/* add first 'lines' element to be at start of a line again */
	offset += strlen(lines[0]) + 1;
	line -= size - 1;
	if (offset < 0 || line < 1)
	{
	    offset = 0;
	    line = 1;
	}
    }
}

/*
 * Function name: read_file
 * Description:   read file contents
 * Arguments:     file - file path
 *                from - first byte where to start reading
 *                num - number of bytes to read
 * Returns:       string
 */
static nomask varargs string
read_file(string file, int from, int num)
{
    if (!file)
	BADARG(1, read_file);
    if (!_ACCESS_READ(file, this_object(), "read_file"))
	return 0;
    return ::read_file(file, from, num);
}

/*
 * Function name: remove_file
 * Description:   delete a file 
 * Arguments:     file - file path
 * Returns:       1 if success, 0 else
 */
static nomask int
remove_file(string file)
{
    int    l;
    object ob;
    
    if (!file)
	BADARG(1, remove_file);
    if ((l = strlen(file)) > 2 && file[l-2..] == ".c" &&
	(ob = find_object(file[l-3..])))
    {
	remove_object(ob);
	if (ob)
	    destruct_object(ob);
    }
    return (_ACCESS_WRITE(file, this_object(), "remove_file") &&
	    ::remove_file(file));
}

/*
 * Function name: remove_dir
 * Description:   delete a directory
 * Arguments:     file - directory path
 * Returns:       1 if success, 0 else
 */
static nomask int
remove_dir(string file)
{
    int len;
    
    if (!file)
	BADARG(1, remove_dir);
    if (!_ACCESS_WRITE(file, this_object(), "remove_dir"))
	return 0;
    len = strlen(file);
    if (len < 1)
	return 0;
    if (file[len-1] != '/') 
	file += "/";
    if (sizeof(::get_dir(file + "*")[0] - ({ _EDITORLOG })) > 0)
	return 0;
    ::remove_file(file + _EDITORLOG);
    return ::remove_dir(file);
}

/*
 * Function name: rename_file
 * Description:   change name of a file
 * Arguments:     from - old file path
 *                to - new file path
 * Returns:       1 if success, 0 else
 */
static nomask int
rename_file(string from, string to)
{
    if (!from)
	BADARG(1, rename_file);
    if (!to)
	BADARG(2, rename_file);
    
    if (!_ACCESS_READ(from, this_object(), "rename_file") ||
	!_ACCESS_WRITE(to, this_object(), "rename_file"))
	return 0;
    
    return ::rename_file(from, to);
}

/*
 * Function name: write_file
 * Description:   write file contents
 * Arguments:     file - file path
 *                str - string to write
 *                offset - byte offset where to start writing
 * Returns:       string
 */
static nomask varargs int
write_file(string file, string str, int offset)
{
    if (!file)
	BADARG(1, write_file);
    if (!str)
	BADARG(2, write_file);
    if (!_ACCESS_WRITE(file, this_object(), "write_file"))
	return 0;
    ::write_file(file, str, offset);
    _DRIVER->write_file_editor(file, geteuid());
    return 1;
}

/*
 * Function name: cat
 * Description:   show part of file
 * Arguments:     file - file path
 *                from - first line where to start reading
 *                num - number of lines to read
 *                tp - living to show contents to
 * Returns:       1 if success, 0 else
 */
static nomask varargs int
cat(string file, int from, int num, object tp)
{
    string *lines;
    
    if (!file)
	BADARG(1, cat);
    if (!tp && !(tp = this_player()))
	return 0;
    if (from < 1) 
	from = 1;
    if (num < 1)
	num = _CAT_LINES;
    else if (num > _CAT_LINES)
	num = _CAT_LINES + 1;
    if (!(lines = read_lines(file, from, num))) 
	return 0;
    if (sizeof(lines) < _CAT_LINES + 1) 
	tp->catch_tell(implode(lines, "\n") + "\n", 1);
    else
	tp->catch_tell(implode(lines[0 .. num-1], "\n") +
		       "\n***TRUNCATED***\n", 1);
    return sizeof(lines);
}

/*
 * Function name: smore
 * Description:   string-more, show a string in small chunks
 * Arguments:     str - string to show
 *                tp - player to show to
 *                func - function to call after
 */
static nomask varargs void
smore(string str, object tp, string func)
{
    int chunk;
    
    if (!str)
	BADARG(1, smore);
    if (!tp)
	BADARG(2, smore);
    if (!living(tp))
	error("Receiver must be a living object");
    if ((chunk = tp->query_prop(8000)) < 3)
    {
	tp->add_prop(8000, 17);	
	chunk = 17;
    }
    tp->_F_more("", explode(str, "\n"), func, 1, chunk);
}

/*
 * Function name: more
 * Description:   show a file in small chunks
 * Arguments:     file - file path of file to show
 *                tp - player to show to
 *                func - function to call after
 *                line - starting line
 */
static nomask varargs void
more(string file, object tp, string func, int line)
{
    int    chunk;
    
    if (!file)
	BADARG(1, more);
    if (!tp)
	BADARG(2, more);
    if (!living(tp))
	error("Receiver must be a living object");
    if ((chunk = tp->query_prop(8000)) < 3)
    {
	tp->add_prop(8000, 17);	
	chunk = 17;
    }
    if (line < 1)
	line = 1;
    tp->_F_more("", file, func, line, chunk);
}

nomask void
_F_more(string input, mixed data, string func, int line, int chunk, object po)
{
    object to;
    mixed  trace;

    if (func)
    {
	if (sizeof(trace = call_trace()) < 2)
	    return;
	if (trace[sizeof(trace)-2][TRACE_FUNCTION] != "_F_call") 
	    po = previous_object(); /* first call not from input_to */
    }
    to = this_object();
    if (input == "q")
    {
	if (func && po)
	    call_other(po, func);
	else
	    to->catch_tell("Ok.\n");
	return;
    }
    if (input && sscanf(input, "%d", line) && line < 1)
	line = 1;
    if (typeof(data) == T_ARRAY)
    {
	line--;
	if (sizeof(data) < line + chunk)
	{
	    to->catch_tell(implode(data[line..sizeof(data)-1], "\n") +
			   "\n", 1);
	    if (func && po)
		call_other(po, func);
	    else
		to->catch_tell("[END]\n");
	    return;
	}
	to->catch_tell(implode(data[line..line+chunk-1], "\n") + "\n", 1);
	line++;
    }
    else if (cat(data, line, chunk, to) != chunk)
    {
	if (func && po)
	    call_other(po, func);
	else
	    to->catch_tell("[END]\n");
	return;
    }
    line += chunk;
    to->catch_tell("More: line " + (line - 1) + " -- <cr>, q, # -- ", 1);
    set_this_player(to);
    input_to("_F_more", 0, data, func, line, chunk, po);
}

/*
 * Function name: tail
 * Description:   show tail of a file
 * Arguments:     file - file path
 *                num - number of lines to show
 * Returns:       1 if success, 0 else
 */
static nomask varargs int
tail(string file, int num)
{
    int    size, *sizes;
    string str, *lines;

    if (!file)
	BADARG(1, tail);
    if (!_ACCESS_READ(file, this_object(), "tail"))
	return 0;
    sizes = ::get_dir(file)[1];
    if (sizeof(sizes) != 1)
	return 0;
    if (_TAIL_CHUNK > sizes[0]) 
	size = sizes[0];
    else
	size = _TAIL_CHUNK;
    if (!(str = ::read_file(file, -size, size)))
	return 0;
    if (num < 3)
	num = 20;
    else if (num > _TAIL_LINES)
	num = _TAIL_LINES;
    lines = explode("\n" + str, "\n");
    if ((size = sizeof(lines)) < num + 1)
	this_player()->catch_tell(implode(lines, "\n") + "\n", 1);
    else
	this_player()->catch_tell(implode(lines[size - num ..], "\n") +
				  "\n", 1);
    return 1;
}
