/*
 * Function name: change_ticks
 * Description:   change resource limits for a function
 * Arguments:     ob - object to call
 *                ticks - new ticks
 *                func - function to call in 'ob'
 *                args - function arguments
 * Returns:       return value of 'func'
 */
static nomask varargs mixed
change_ticks(mixed ob, int ticks, string func, mixed args...)
{
    if (!ob)
	BADARG(1, change_ticks);
    if (ticks <= 0)
	BADARG(2, change_ticks);
    if (!func)
	BADARG(3, change_ticks);
    rlimits (-1; -1)
    {
	rlimits (30; ticks)
	{
	    return call_other(ob, func, args...);
	}
    }
}

/*
 * Function name: object_time
 * Description:   gives object creation/compile time
 * Arguments:     obj - object to query
 * Returns:       integer, time
 */
static nomask int
object_time(object obj)
{
    if (!obj)
	BADARG(1, object_time);
    return status(obj)[O_COMPILETIME];
}

/*
 * Function name: program_name
 * Description:   gives program name for a clone or master object
 * Arguments:     obj - object to query
 * Returns:       string
 */
static nomask string
program_name(object obj)
{
    int    i;
    string n;
    
    if (!obj)
	BADARG(1, program_name);
    
    n = ::object_name(obj);
    for (i=strlen(n); i--;)
	if (n[i] == '#')
	    return n[..i-1];
    return n;
}

/*
 * Function name: implode
 * Description:   concat a string array, seperated by a pattern
 * Arguments:     arr - array to concat
 *                pat - concat pattern
 * Returns:       string
 */
static nomask string
implode(mixed arr, string pat)
{
    if (!sizeof(arr))
	return "";
    if (typeof(arr[0]) != T_STRING)
	return "";
    if (!pat)
	pat = "";
    return ::implode(arr, pat);
}

/*
 * Function name: explode
 * Description:   split a string by a given seperator
 * Arguments:     str - string to split
 *                pat - split seperator
 * Returns:       array, splitted string
 */
static nomask string *
explode(string str, string pat)
{
    if (!str)
	BADARG(1, explode);
    if (!strlen(str))
	return ({ });
    if (!pat)
	pat = "";
    return ::explode(str, pat);
}

/*
 * Function name: capitalize
 * Description:   upper case the first letter of a string
 * Arguments:     str - string to capitalize
 * Returns:       string
 */
static nomask string
capitalize(string str)
{
    if (!str)
	BADARG(1, capitalize);
    if (!strlen(str))
	return "";
    if (str[0] >= 'a' && str[0] <= 'z')
	str[0] -= 'a' - 'A';
    return str;
}

/*
 * Function name: lower_case
 * Description:   change all letters of a string to lower case letters
 * Arguments:     str - string to change
 * Returns:       string
 */
static nomask string
lower_case(string str)
{
    int i;

    if (!str)
	BADARG(1, lower_case);
    for (i=strlen(str); i--;)
        if (str[i] >= 'A' && str[i] <= 'Z')
	    str[i] |= 32;
    return str;
}

/*
 * Function name: map_indices
 * Description:   gives all indices of a mapping as array
 * Arguments:     map - mapping
 * Returns:       array, indices
 */
static nomask mixed *
map_indices(mapping map)
{
    if (!map)
	BADARG(1, map_indices);
    return ::map_indices(map);
}

/*
 * Function name: map_values
 * Description:   gives all mapping values as array
 * Arguments:     map - mapping
 * Returns:       array, values
 */
static nomask mixed *
map_values(mapping map)
{
    if (!map)
	BADARG(1, map_values);
    return ::map_values(map);
}

/*
 * Function name: map_sizeof
 * Description:   gives the number of mapping entries
 * Arguments:     map - mapping
 * Returns:       integer, the size
 */
static nomask int
map_sizeof(mapping map)
{
    if (!map)
	return 0;
    return ::map_sizeof(map);
}

/*
 * Function name: sizeof
 * Description:   gives the number of array entries
 * Arguments:     arr - array
 * Returns:       integer, the size
 */
static nomask int
sizeof(mixed arr)
{
    if (typeof(arr) == T_ARRAY)
	return ::sizeof(arr);
    return 0;
}

/*
 * Function name: member_array
 * Description:   search for an element in an array
 * Arguments:     elt - element to search
 *                arr - array to search through
 * Returns:       element index if found, else -1
 */
static nomask int
member_array(mixed elt, mixed arr)
{
    int i, sz;

    if (typeof(arr) != T_ARRAY)
	return -1;
    for (i=0, sz=sizeof(arr); i<sz; i++)
    {
	if (arr[i] == elt)
	    return i;
    }
    return -1;
}

/*
 * Function name: extract
 * Description:   get a substring from a string
 * Arguments:     str - string to change
 *                first - first letter
 *                last - last letter
 * Returns:       string, the substring
 */
static nomask varargs string
extract(string str, int first, int last)
{
    if (!str)
	BADARG(1, extract);
    if (first != 0 && last == 0)
	last = strlen(str) - 1;
    else if (first >= strlen(str) || last < first)
	return "";
    if (last > strlen(str) - 1)
	last = strlen(str) - 1;
    return str[first .. last];
}

/*
 * Function name: exclude_array
 * Description:   remove some array elements from an array
 * Arguments:     arr - array to change
 *                first - first element to exclude
 *                last - last element to exclude
 * Returns:       array, the reduced array
 */
static nomask mixed
exclude_array(mixed arr, int first, int last)
{
    int sz;

    if (!(sz = sizeof(arr)) || last < first)
	return ({ });

    if (last > sz-1)
	last = sz-1;
    else if (last < -1)
	last = -1;

    if (first >= sz)
	first = sz;
    else if (first < 0)
	first = 0;

    return arr[..(first-1)] + arr[(last+1)..];
}

private void
recursive_rm(object ob)
{
    object user, loc;
    
    if ((user = ob->query_user()) && ::query_ip_number(user) &&
	(loc = _ROOMD->load_room(DEFAULT_START)))
    {
	ob->set_coordinates(loc->query_coordinates());
	ob->move(loc);
	return;
    }
    remove_object(ob);
}

private void
do_destruct_object(object obj)
{
    object env, usr;

    if (env = obj->environment())
    {
	rlimits(-1; -1)
	{
	    env->_F_rm_inv(obj);
	}
    }
    if ((usr = obj->query_user()) && (usr->query_player() == obj))
    {
	rlimits(-1; -1)
	{
	    usr->disconnect();
	}
    }
    rlimits(-1; -1)
    {
	_DRIVER->delete_object(obj);
    }
    ::destruct_object(obj);
}

/*
 * Function name: remove_object
 * Description:   remove an object, also removes inventory, destruct should
 *                only be used if an error occurred here.
 * Arguments:     obj - object to remove
 */
varargs void
remove_object(object obj)
{
    int    i;
    object *inv;
    
    if (!obj)
	obj = this_object();
    else
    {
	obj->remove_object(0);
	return;
    }
    if (!_ACCESS_DESTRUCT(obj, "remove_object"))
	error("Illegal call to remove_object");
    for (i=sizeof(inv=obj->inventory(7)); i--;)
	recursive_rm(inv[i]);
    do_destruct_object(obj);
}

/*
 * Function name: destruct_object
 * Description:   remove an object (this doesn't affect its inventory!)
 * Arguments:     obj - object to destruct, defaults to this_object()
 */
static nomask varargs void
destruct_object(object obj)
{
    if (!obj)
	obj = this_object();
    if (!_ACCESS_DESTRUCT(obj, "destruct_object"))
	error("Illegal call to destruct_object");
    do_destruct_object(obj);
}

/*
 * Function name: clone_object
 * Description:   make a copy of a master object to get a clone
 * Arguments:     arg - object or filepath of object to clone
 * Returns:       object, the clone
 */
static nomask object
clone_object(mixed arg)
{
    if (!arg)
	BADARG(1, clone_object);

    rlimits(-1; -1)
    {
	if (stringp(arg) && !(arg = _DRIVER->call_object(arg)))
	    return 0;
	arg = ::clone_object(arg);
	_DRIVER->insert_object(arg);
	return arg;
    }
}

/*
 * Function name: compile_object
 * Description:   load and therefore compile an object, also recompile code
 *                of an already loaded object without losing data
 *                (if successful)
 * Arguments:     arg - file path of object to clone
 * Returns:       object
 */
static nomask object
compile_object(string arg)
{
    if (!arg)
	BADARG(1, compile_object);
    if (find_object(arg))
	return ::compile_object(arg); /* upgrade object */
    return _DRIVER->call_object(arg);
}

/*
 * Function name: query_idle
 * Description:   give idle time of an interactive user
 * Arguments:     obj - living to query
 * Returns:       integer, idle time
 */
static nomask int
query_idle(object obj)
{
    object usr;

    if (!obj)
	BADARG(1, query_idle);
    usr = obj->query_user();
    if (::query_ip_number(obj) == 0 && (!usr || ::query_ip_number(usr) == 0))
	return 0;
    return usr->_Q_idle();
}

/*
 * Function name: query_ip_number
 * Description:   give ipnumber of an interactive user
 * Arguments:     obj - object to query
 * Returns:       string, ipnumber
 */
static nomask varargs string
query_ip_number(object obj)
{
    object u;
    
    if (!obj)
	obj = this_object();
    if (u = obj->query_user())
	return ::query_ip_number(u);
    return ::query_ip_number(obj);
}

/*
 * Function name: shutdown
 * Description:   shutdown DGD
 */
static nomask void
shutdown()
{
    int    i;
    object *usr;

    if (getuid() != _ROOT)
	error("Illegal call to shutdown");
    rlimits(-1; -1)
    {
	usr = users();
	for (i=sizeof(usr); i--;)
	{
	    if (usr[i])
		catch(usr[i]->save());
	}
	_DRIVER->save_data(); /* e.g. save all rooms to disk */
	_DRIVER->prepare_dump();
	::dump_state();
	::shutdown();
    }
}

/*
 * Function name: exec
 * Description:   swap player objects from user to another user
 * Arguments:     to - object to connect to
 *                from - object to get user object
 * Returns:       1 if success, else 0
 */
static nomask int
exec(object to, object from)
{
    string fname, dummy;
    object user, other;

    fname = explode(object_name(this_object()), "#")[0];
    if (fname != _LOGIN && fname != _SYSTEMD)
	error("Illegal call to exec");

    if (!to)
	BADARG(1, exec);
    if (to == from)
	return 0;
    if (!from || !(user = from->query_user()))
	BADARG(2, exec);
    
    other = to->query_user();
    if (other != 0)
    {
	rlimits(-1; -1)
	{
	    other->disconnect();
	}
    }
    rlimits(-1; -1)
    {
	user->set_player(to);
	from->set_user(0);
    }
    if (this_player() == from)
	set_this_player(to);
    return 1;
}

/*
 * Function name: break_string
 * Description:   insert newline characters after a given number of letters
 * Arguments:     str - string to process
 *                num - maximum characters per line
 *                indent - indentation value per line
 * Returns:       string, the changed string
 */
static nomask varargs string
break_string(string str, int num, int indent)
{
    int    i, k, l;
    string dummy;

    if (!(strlen(str)))
	return "";
    if (indent < 0 || indent > 30)
	error("Bad indentation value");
    if (indent > 0)
	dummy = "                                      "[..indent-1];
    else
	dummy = "";
    
    str = dummy + str;
    if (num >= strlen(str))
	return str;
    else if (!num)
	num = 1;

    i = k = num;
    l = indent;
    while (1)
    {
	while (i > l)
	{
	    if (str[i] == ' ' || str[i] == '\n')
	    {
		str[i] = '\n';
		str = str[..i] + dummy + str[++i..];
		l = i + indent;
		k = i += num;
		if (i >= strlen(str))
		    return str;
	    }
	    else
		i--;
	}
	/* here now break words which are longer than given line length */
	str = str[..k-1] + "\n" + dummy + str[k++..];
	l = k + indent;
	i = k += num;
	if (i >= strlen(str))
	    return str;
    }
}

private string
make_table(string str, int width, string del)
{
    int    columns, maxwidth, i, j, d, num, lines, restlines;
    string *parts, result;

    parts = explode(str,"\n");

    if (!(num = sizeof(parts)))
	return "";

    if (!(d = strlen(del)))
    {
	del = " ";
	d = 1;
    }
    maxwidth = strlen(parts[0]);
    for (i=0; i<num; i++)
	if ((j = strlen(parts[i])) > maxwidth)
	    maxwidth = j;

    /* possible number of columns - take delimiter into account */
    lines     = (num - 1) / ((width + d) / (maxwidth + d)) + 1;
    /* maybe we need less columns */
    columns   = (num - 1) / lines + 1;
    restlines = num % lines;

    result = "";
    maxwidth--;
    if (restlines)
    {
	for (i=0; i<restlines; i++)
	{
	    for (j=0; j<columns-1; j++)
		result += extract(parts[ i + j * lines] +
				  "                                          ",
				  0, maxwidth) + del;
	    result += parts[i + j * lines] + "\n";
	}
	for (i=restlines; i<lines; i++)
	{
	    for (j=0; j<columns-2; j++)
		result += extract(parts[i + j * lines] +
				  "                                          ",
				  0, maxwidth) + del;
	    result += extract(parts[i + j * lines] +
			      "                                          ",
			      0, maxwidth) + del + "\n";
	}
    }
    else
    {
	for (i=0; i<lines; i++)
	{
	    for (j=0; j<columns-1; j++)
		result += extract(parts[i + j * lines]+
				  "                                          ",
				  0, maxwidth) + del;
	    result += parts[i + j * lines] + "\n";
	}
    }
    return result;
}

private string
check_indent(string start, string how, string arg)
{
    int    i, d;
    string line, del;

    if (!strlen(how))
	return start + arg;

    if (sscanf(how, "%s*%d", del, d))
	return start + make_table(arg, d, del);

    if (sscanf(how, "|%d", d))
    {
	if (strlen(arg) >= d)
	    return start + extract(arg, 0, d-1);
	for (del=" ", i=7; i--;)
	    del += del;
	return (start +
		del[.. (d - strlen(arg))     / 2 - 1] + arg +
		del[.. (d - strlen(arg) + 1) / 2 - 1]);
    }
    del = " ";
    if (sscanf(how, "%s/%d", del, d) != 2 &&
	sscanf(how, "%d", d) != 1)
	error("Unknown format to sprintf " + how);

    for (i=7; i--;)
	del += del;
    if (d < 0)
    {
	d = -d;
	if (strlen(arg) >= d)
	    return start + extract(arg, 0, d-1);
	return start + arg + del[..d - strlen(arg) - 1];
    }
    if (strlen(arg) >= d)
	return start + extract(arg, 0, d-1);
    return start + del[..d - strlen(arg) - 1] + arg;
}

/*
 * Function name: sprintf
 * Description:   create a formated string
 * Arguments:     format - string format, where:
 *                         %s is a string (with optional size: %<size>s)
 *                         %d is an integer (with optional size: %<size>d)
 *                         %o is an object (with optional size: %<size>o)
 *                         %*<size>s create a string table (the table entries
 *                                   need to be seperated by newlinecharacters
 *                argv - arguments to replace in format string
 * Returns:       string, formated string
 */
static nomask varargs string
sprintf(string format, mixed argv...)
{
    int    i, sz;
    string how, c1, c2, result;

    if (!format)
	BADARG(1, sprintf);
    
    result = "";
    for (i=0, sz=sizeof(argv); i<sz; i++)
    {
	if (typeof(argv[i]) == T_STRING)
	{
	    if (sscanf(format,"%s%%%ss%s", c1, how, c2) != 3)
		error("Bad sprintf format string. String found no \%s");
	    format = c2;
	    result += check_indent(c1, how, argv[i]);
	    continue;
	}
	else if (typeof(argv[i]) == T_INT)
	{
	    if (sscanf(format,"%s%%%sd%s", c1, how, c2) != 3)
		error("Bad sprintf format string. Integer found no \%d");
	    format = c2;
	    result += check_indent(c1, how, (string)argv[i]);
	    continue;
	}
	else if (typeof(argv[i]) == T_OBJECT)
	{
	    if (sscanf(format,"%s%%%so%s", c1, how, c2) != 3)
		error("Bad sprintf format string. Object found no \%o");
	    format = c2;
	    result += check_indent(c1, how, object_name(argv[i]));
	    continue;
	}
    }
    return result + format;
}

/*
 * Function name: gmt_ctime
 * Description:   gives the same output as ctime() but GMT
 * Arguments:     t - time value in seconds since 1970
 * Returns:       time description string
 */
nomask string
gmt_ctime(int t)
{
    int    hour, min, sec, day, year, leap;
    string month, wday;

    day = t / 86400;
    sec = t - day * 86400;
    leap = (day / 365 + 2) / 4;
    year = (day - leap) / 365;
    wday = ({ "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" })[(day+3)%7];
    day -= (year * 365) + leap - 1; /* 1970, 1. Jan ->  +1 */
    year += 1970;
    
    if (day <= 31)
	month = "Jan";
    else if ((day -= 31 + ((year % 4) ? 0 : 1)) <= 28)
    {
	month = "Feb";
	day += (year % 4);
    }
    else if ((day -= 28) <= 31)
	month = "Mar";
    else if ((day -= 31) <= 30)
	month = "Apr";
    else if ((day -= 30) <= 31)
	month = "May";
    else if ((day -= 31) <= 30)
	month = "Jun";
    else if ((day -= 30) <= 31)
	month = "Jul";
    else if ((day -= 31) <= 31)
	month = "Aug";
    else if ((day -= 31) <= 30)
	month = "Sep";
    else if ((day -= 30) <= 31)
	month = "Oct";
    else if ((day -= 31) <= 30)
	month = "Nov";
    else if ((day -= 30) <= 31)
	month = "Dec";
    hour = sec / 3600;
    min  = (sec - hour * 3600) / 60;
    sec %= 60;

    return sprintf("%s %s %2d %0/2d:%0/2d:%0/2d %d GMT",
		   wday, month, day, hour, min, sec, year);
}

/*
 * Function name: type_to_string
 * Description:   translate any data type into string form (for debug output)
 * Arguments:     arg - data value to transform
 *                indent - indentation value
 * Returns:       string, the transformed value
 */
static nomask string
type_to_string(mixed arg, int indent)
{
    int    i, sz, l;
    string space, str, tmp;
    mixed ind;

    if (!arg)
	return "0";
    if (intp(arg) || floatp(arg))
	return (string)arg;
    if (stringp(arg))
    {
	arg = implode(explode("\n" + arg + "\n", "\n"), "\\n");
	arg = implode(explode("\r" + arg + "\r", "\r"), "\\r");
	arg = implode(explode("\t" + arg + "\t", "\t"), "\\t");
	return "\"" + arg + "\"";
    }
    if (objectp(arg))
	return "OBJ(\"" + object_name(arg) + "\")";

    space = "                                                           " +
	    "                                                           ";
    if (mappingp(arg))
    {
	if (!map_sizeof(arg))
	    return "([ ])";
	ind     = map_indices(arg);
	str     = "([\n";
	for (i=0, sz=sizeof(ind); i<sz; i++)
	{
	    l = strlen(tmp = type_to_string(ind[i], 0));
	    str += (space[..indent + 3] + tmp + " : " +
		    type_to_string(arg[ind[i]], indent + l + 6) + ",\n");
	}
	return str + space[..indent] + "])";
    }
    if (pointerp(arg))
    {
	if (!sizeof(arg))
	    return "({ })";
	str = "({\n";
	for (i=0, sz=sizeof(arg); i<sz; i++)
	{
	    str += (space[..indent + 3] + type_to_string(arg[i], indent + 3) +
		    ",\n");
	}
	return str + space[..indent] + "})";
    }
    
    error("Unknown data type");
}
