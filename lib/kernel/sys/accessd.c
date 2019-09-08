/*
 * this demon handles all access checking/modifing
 */
#include <access.h>
#include <config.h>
#include <kernel.h>
#include <trace.h>

#define SAVEFILE "/kernel/data/ACCESSDATA"

#define _ACCESS_FILE(user) (PLAYER_DIR(user) + "/access")

#define P_MEMBERS     1
#define P_FILEACCESS  2
#define P_LEVEL       3
#define P_CREATOR     4
#define P_DESCRIPTION 5

private mapping parse_access_data(string *lines);

mapping projects, members, objaccess;
static mapping access_data, access_default;

static nomask void
create()
{
    restore_object(SAVEFILE);
    if (!projects)
	projects = ([
	    _ACCESS_DEFAULT : ([ 
		P_MEMBERS : ({ }),
		P_FILEACCESS : ({ }),
		P_LEVEL : _BUILDER,
		P_CREATOR : "System",
		P_DESCRIPTION : "The default access for all coders" ])
	    ]);
    if (!members)
	members = ([ ]);
    if (!objaccess)
	objaccess = ([ ]);
    access_data = ([ ]);
    access_default = parse_access_data(projects[_ACCESS_DEFAULT]
				       [P_FILEACCESS]);
}

/*
 * Function name: parse_access_data
 * Description:   parse the access data into mapping structure
 * Arguments:     lines - the access data
 * Returns:       mapping
 */
private mapping
parse_access_data(string *lines)
{
    int     i;
    string  line, *faccess;
    mapping data;

    if (!lines)
	return 0;
    data = ([ ]);
    faccess = ({ });
    for (i=sizeof(lines); i--;)
    {
	line = lines[i];
	if (!strlen(line) || line[0] == '#')
	    continue;
	if (line[0] == 'r' || line[0] == 'w')
	    faccess |= ({ line });
	else if (line[0] == 's')
	    data[_ADATA_SNOOP] = (int)line[1..];
	else if (line[0] == 'f')
	    data[_ADATA_FORCE] = (int)line[1..];
    }
    data[_ADATA_FILE] = faccess;
    return data;
}

/*
 * Function name: parse_access_file
 * Description:   read the access file into mapping structure
 * Arguments:     file - access file name
 * Returns:       mapping
 */
private mapping
parse_access_file(string file)
{
    string  data;

    if (!(data = read_file(file)))
	return 0;
    return parse_access_data(explode(data, "\n"));
}

/*
 * Function name: add_objaccess
 * Description:   set access values for special files
 * Arguments:     file - the file name of the object
 *                data - access data string to add/remove
 */
nomask void
add_objaccess(string file, string data)
{
    if (_ACCESS_LEVEL(geteuid(previous_object())) < _ADMIN)
	error("Illegal call to add_objaccess");
    if (!data || !strlen(data) || !find_object(file))
	return;
    if (!objaccess[file])
	objaccess[file] = ([ ]);
    if (data[0] == 's')
	objaccess[file][_ADATA_SNOOP] = data[1..];
    else if (data[0] == 'f')
	objaccess[file][_ADATA_FORCE] = data[1..];
    else if (data[0] == 'r' || data[0] == 'w')
    {
	if (!objaccess[file][_ADATA_FILE])
	    objaccess[file][_ADATA_FILE] = ({ data });
	else
	    objaccess[file][_ADATA_FILE] |= ({ data });
    }
    save_object(SAVEFILE);
}

/*
 * Function name: reload_access
 * Description:   rereads accessfile for an object
 * Arguments:     obj - the object which needs an access update
 */
nomask void
reload_access(object obj)
{
    string euid;
  
    if (!obj)
	return;
    euid = geteuid(obj);
    if (euid == "0")
      access_data[euid] = access_default;
    else 
      access_data[euid] = parse_access_file(_ACCESS_FILE(euid));
}

/*
 * Function name: show_access
 * Description:   gives a printable string of the access file contents
 * Arguments:     obj - object or name to query access data for
 * Returns:       string
 */
nomask string
show_access(mixed obj)
{
    string  out, euid;
    mapping data;
    
    if (stringp(obj))
	return read_file(_ACCESS_FILE(obj));
    if (!objectp(obj))
	return "";
    euid = geteuid(obj);
    if (!(data = access_data[euid]))
	reload_access(obj);
    if (!(data = access_data[euid]))
	return "";
    out = "";
    if (data[_ADATA_SNOOP])
	out += "s" + data[_ADATA_SNOOP] + "\n";
    if (data[_ADATA_FORCE])
	out += "f" + data[_ADATA_FORCE] + "\n";
    if (data[_ADATA_FILE])
	out += implode(data[_ADATA_FILE], "\n") + "\n";
    return out;
}

/*
 * Function name: read_or_write_access
 * Description:   check if someone can read/write some file or directory
 * Arguments:     path - the path to check
 *                type - what to check, 'r'ead or 'w'rite permission
 *                data - the access data array to use
 * Returns:       1 if read or write is allowed, 0 else
 */
private int
read_or_write_access(string path, int type, string *data)
{
    int    i, sz, len, access, matchlen;
    string str;

    if (!path || !type || !data)
	return 0;
    
    matchlen = 0;
    for (i=access=0, sz=sizeof(data); i<sz; i++)
    {
	if (!(len = strlen(str = data[i])))
	    continue;
	/* access if the path equals the access-entry or if
	 * the access-entry equals a directory containing the path
	 */
	len -= 2;
	if (str[0] == type && len <= strlen(path) &&
	    str[2..] == path[..len-1])
	{
	    if (len < matchlen)
		continue;
	    if (len == strlen(path)) /* exact match */
		return (data[i][1] == '+' ? 1 : 0);
	    if (path[len-1] == '/' || path[len] == '/') /* directory match */
		access = (data[i][1] == '+' ? 1 : 0);
	    matchlen = len;
	}
    }
    return access;
}

/*
 * Function name: _Q_access_data
 * Description:   gives the correct access data array for an object
 * Arguments:     obj - object or name to query access data for
 * Returns:       array 
 */
nomask mapping
_Q_access_data(object obj)
{
  string euid;
  mapping access;
  
  if (!obj)
    return 0; 
  if (access = objaccess[program_name(obj)])
    return access;
  if (access = access_data[geteuid(obj)])
    return access;
  reload_access(obj);
  return access_data[geteuid(obj)];
}

/*
 * Function name: _Q_seteuid_access
 * Description:   Checks if a certain user has the right to set a certain
 *                objects effective 'userid'.
 * Arguments:     pobj - Object that wants to set 'effective' user id.
 *                ob - Object to set 'effective' user id in.
 *                str - The euid to be set.
 * Returns:       1 if allowed, 0 else
 */
nomask int
_Q_seteuid_access(object pobj, object ob, string str)
{
    int    lev;
    string euid, oeuid;

    if (!pobj)
	pobj = ob;

    euid  = geteuid(pobj);
    oeuid = geteuid(ob);

    /* root can change all, no change and deletion "0" is allowed
     * also changing the euid to the original uid if the object does 
     * that to itself
     */
    if (!str || str == "0" || euid == _ROOT || str == oeuid ||
	(str == getuid(ob) && ob == pobj))
	return 1;
    /* coders can change to an euid lesser then their own */
    if ((lev = _ACCESS_LEVEL(euid)) > _ACCESS_LEVEL(oeuid) &&
	_ACCESS_LEVEL(str) <= lev)
	return 1;
    return 0;
}

/*
 * Function name: _Q_force_access
 * Description:   Checks if a coder is allowed to force someone
 * Arguments:     forcer - object trying to force
 *		  victim - object being forced
 * Returns:       1 if allowed, 0 else
 */
nomask int
_Q_force_access(object forcer, object victim)
{
    mixed d1, d2;
    
    if ((d1 = _Q_access_data(forcer)) &&
	(!(d2 = _Q_access_data(victim)) ||
	 (d1[_ADATA_FORCE] > d2[_ADATA_FORCE])))
	return 1;
    return 0;
}

/*
 * Function name: _Q_snoop_access
 * Description:   Checks if a certain user is allowed to snoop another user.
 *                Allowed if the initiator is allowed to snoop the victim.
 * Arguments:     iobj - initiator object
 *                snooper - the snooping object
 *		  snoopee - the snooped object
 * Returns:       1 if allowed, 0 else
 */
nomask int
_Q_snoop_access(object iobj, object snooper, object snoopee)
{
    mixed d1, d2;

    if (geteuid(iobj) == geteuid(snoopee))
	return 1;
    if ((d1 = _Q_access_data(iobj)) &&
	(!(d2 = _Q_access_data(snoopee)) ||
	 (d1[_ADATA_SNOOP] > d2[_ADATA_SNOOP])))
	return 1;
    return 0;
}

/*
 * Function name: _Q_destruct_access
 * Description:   check if an object is allowed to destruct an object
 * Arguments:     ob - object to destruct
 *                func - calling function
 * Returns:       1 if allowed, 0 else
 */
nomask int
_Q_destruct_access(object ob, string func)
{
    int    i, level;
    string objname;
    object destructor;
    mixed  trace;

    if (!ob || !func)
	return 1;
    /* find occurence of 'func' in trace */
    if ((i = sizeof(trace = call_trace())) < 1)
	return 0;
    i--; /* skip first element (this function) */
    while (i--)
    {
	if (trace[i][TRACE_FUNCTION] != func) 
	    break;
    }
    if (i < 0)
	return 0;
    if ((objname = trace[i][TRACE_OBJNAME]) == object_name(ob) ||
	objname == _DRIVER)
	return 1;
    if (!(destructor = find_object(objname)))
	return 0;
    level = ob->_Q_destruct();
    
    if (_ACCESS_LEVEL(geteuid(destructor)) >= level)
	return 1;
    return 0;
}

/*
 * Function name: _Q_write_access
 * Description:   Checks if a certain user has the right to write a file
 * Arguments:     file - file name of the file to be written
 *                writer - writing object
 *                func - calling function
 * Returns:       1 if allowed, 0 else
 */
nomask int
_Q_write_access(string file, object writer, string func)
{
    string euid;
    mixed  data;
    
    if (!writer || !file)
	return 0;
    if (writer == this_object()) /* avoid recursive calls */
	return 1;
    if (sscanf(file, "%*s %*s") || sscanf(file, "%*s/" + _EDITORLOG))
	return 0;
    /* the following check is necessary to catch the parent directory '..'
     * but allow filenames like /home/manwe/abc...o
     */
    if (sscanf(file, "%*s..%*s") && (sscanf(file, "%*s/../%*s") ||
				     sscanf(file, "../%*s") ||
				     file[strlen(file)-2..] == ".."))
	return 0;
    if ((euid = geteuid(writer)) == _ROOT)
	return 1;
    if (euid == _SYSTEM && (sscanf(file, "/syslog/%*s") ||
                   sscanf(file, "/system/%*s") ||
			    sscanf(file, "/players/%*s")))
	return 1;
    if (file == "/home/" + euid || sscanf(file, "/home/" + euid + "/%*s"))
	return 1;
    if (func == "save_object")
    {
	/* objects/rooms can save to a file that equals their file name
	 * (without extension) - clones of /base/room can save if the savefile
	 * equals the entry in the room mapper
	 */
	if ((data = program_name(writer)) == file ||
	    (strlen(data) > 5 && data[..5] == "/base/" &&
	     _ROOMD->query_room(file) == writer))
	    return 1;
	
	/* playerfiles can save themself */
	if (previous_program(1) == PLAYER_BASE)
	    return 1;
    }
    if (!(data = _Q_access_data(writer)))
	return 0;
    return read_or_write_access(file, 'w', data[_ADATA_FILE]);
}

/*
 * Function name: _Q_read_access
 * Description:   Checks if a certain user has the right to read a file
 * Arguments:     file - File name of the file to be read
 *                reader - reading object
 *                func - calling function
 * Returns:       1 if allowed, else 0
 */
nomask int
_Q_read_access(string file, object reader, string func)
{
    string euid;
    mixed  data;
    
    if (!reader || !file)
	return 0;
    if (reader == this_object()) /* avoid recursive calls */
	return 1;
    if (sscanf(file, "%*s %*s"))
	return 0;
    /* the following check is necessary to catch the parent directory '..'
     * but allow filenames like /home/manwe/abc...o
     */
    if (sscanf(file, "%*s..%*s") && (sscanf(file, "%*s/../%*s") ||
				     sscanf(file, "../%*s") ||
				     file[strlen(file)-2..] == ".."))
	return 0;
    if ((euid = geteuid(reader)) == _ROOT)
	return 1;
    if (euid == _SYSTEM &&
	(sscanf(file, "/syslog/%*s") ||
	 sscanf(file, "/system/%*s") ||
	 sscanf(file, "/players/%*c/%*s/") == 2))
	return 1;
    if (file == "/home/"+euid || sscanf(file, "/home/" + euid + "/%*s"))
	return 1;
    if (!(data = _Q_access_data(reader)))
	return 0;
    return read_or_write_access(file, 'r', data[_ADATA_FILE]);
}

/*****************************************************************************
 * project manager
 */

/*
 * Function name: update_access
 * Description:   Update the file access for a given list of coders.
 * Arguments:     list - the list of coder names
 */
nomask void
update_access(string *list)
{
    int     i, j, k, gr, gw, len;
    string  data, tmp, *tmpaccess, *access, *prjs;
    object  pl;
    mapping p;
    
    if (!(i = sizeof(list)))
	return;
	
    while (i--)
    {
	access = access_default[P_FILEACCESS];
	if (prjs = members[list[i]])
	{
	    for (j=sizeof(prjs); j--;)
	    {
		if (!(p = projects[prjs[j]]))
		    continue;
		access |= p[P_FILEACCESS];
	    }
	}
	/* specially granted directories overwrite negations,
	 * e.g w+/doc replaces w-/doc
	 */
	tmpaccess = access - ({ "r+/", "w+/" });
	for (j=sizeof(tmpaccess); j--;)
	{
	    data = tmpaccess[j];
	    if (data[1] == '+')
	    {
		data[1] = '-';
		access -= ({ data });
	    }
	}

	/* and shorten file if global access */
	if (member_array("r+/", access) != -1)
	    gr = 1;
	else
	    gr = 0;
	if (member_array("w+/", access) != -1)
	    gw = 1;
	else
	    gw = 0;

	for (j=sizeof(tmpaccess); j--;)
	{
	    data = tmpaccess[j];
	    if (data[1] != '+')
		continue;
	    if (gr && data[0] == 'r')
	    {
		len = strlen(data);
		for (k=sizeof(tmpaccess); k--;)
		{
		    tmp = tmpaccess[k];
		    if (tmp[0] == 'r' && tmp[1] == '-' && strlen(tmp) < len &&
			tmp[2..] == data[2..strlen(tmp)-1])
			break;
		}
		if (k < 0)
		    access -= ({ data });
	    }
	    else if (gw && data[0] == 'w')
	    {
		len = strlen(data);
		for (k=sizeof(tmpaccess); k--;)
		{
		    tmp = tmpaccess[k];
		    if (tmp[0] == 'w' && tmp[1] == '-' && strlen(tmp) < len &&
			tmp[2..] == data[2..strlen(tmp)-1])
			break;
		}
		if (k < 0)
		    access -= ({ data });
	    }
	}
	
	remove_file(_ACCESS_FILE(list[i]));
	data = implode(access, "\n") + "\n";
	write_file(_ACCESS_FILE(list[i]), data);
	if (pl = find_player(list[i]))
	{
	    reload_access(pl);
	    pl->catch_tell("Your file access has been updated.\n");
	}
    }
}

/*
 * Function name: list_project
 * Description:   give detailed info about a project
 * Arguments:     project - the projects name
 *                tp - the player that wants to receive the info
 * Returns:       1 if success, -1 else
 */
nomask int
list_project(string project, object tp)
{
    string  out, *projs;
    mapping p;
    
    if (!tp)
	return -1;
    
    if (project == "list")
	out = "Projectlist:\n" +
	      break_string(implode(map_indices(projects), " / "), 70, 2) +
	      "\n";
    else if (project == "own")
    {
	if (!(projs = members[tp->query_name()]))
	    out = "You are not a member of any project.\n";
	else
	    out = "You are member of the following projects:\n" +
		  break_string(implode(projs, " / "), 70, 2) + "\n";
    }
    else if (!project || !(p = projects[project]))
    {
	tp->catch_tell("No such project.\n");
	return -1;
    }
    else
	out = "Project " + project + " has been created by " +
	      capitalize(p[P_CREATOR]) + ".\n" +
	      (p[P_DESCRIPTION] ? break_string(p[P_DESCRIPTION], 70, 0) : "") +
	      "\nMembers are:\n" +
	      break_string(implode(p[P_MEMBERS], " / "), 70, 2) +
	      "\nProject fileaccess:\n" + implode(p[P_FILEACCESS], "\n") +
	      "\n";
    tp->catch_tell(out);
    return 1;
}

/*
 * Function name: add_project
 * Description:   add a new project to the project mapping
 * Arguments:     project - project name
 *                level - project level
 * Returns:       1 if success, -1 if failed
 */
nomask int
add_project(string project)
{
    int    level;
    object po;

    po = previous_object();
    if ((level = _ACCESS_LEVEL(geteuid(po))) < _CREATOR || projects[project])
	return -1;
    
    projects[project] = ([
	P_MEMBERS : ({ }),
	P_FILEACCESS : ({ }),
	P_LEVEL : level,
	P_CREATOR : geteuid(po),
	]);
    save_object(SAVEFILE);
    return 1;
}

/*
 * Function name: change_project_desc
 * Description:   set a new description for a project
 * Arguments:     project - the projects name
 *                desc - the new description
 * Returns:       1 if success, -1 if failed
 */
nomask int
change_project_desc(string project, string desc)
{
    object  po;
    mapping p;
    
    po = previous_object();
    if (!(p = projects[project]) || !desc)
	return -1;
    if (_ACCESS_LEVEL(geteuid(po)) <= p[P_LEVEL] &&
	p[P_CREATOR] != geteuid(po))
	return -1;
    projects[project][P_DESCRIPTION] = desc;
    save_object(SAVEFILE);
    return 1;
}

/*
 * Function name: remove_project
 * Description:   remove an existing project. Updates the file access for all
 *                project members
 * Arguments:     project - project name
 * Returns:       1 if success, -1 if failed
 */
nomask int
remove_project(string project)
{
    int     i;
    string  *mem;
    object  po;
    mapping p;

    po = previous_object();
    if (!(p = projects[project]))
	return -1;
    if (_ACCESS_LEVEL(geteuid(po)) <= p[P_LEVEL] &&
	p[P_CREATOR] != geteuid(po))
	return -1;
    mem = p[P_MEMBERS];
    for (i=sizeof(mem); i--;)
    {
	if (!members[mem[i]])
	    continue;
	members[mem[i]] -= ({ project });
	if (!sizeof(members[mem[i]]))
	    members[mem[i]] = 0;
    }
    projects[project] = 0;
    update_access(mem);
    save_object(SAVEFILE);
    return 1;
}

/*
 * Function name: add_member
 * Description:   add a coder to a project. Updates file access for
 *                the new member.
 * Arguments:     project - the projects name
 *                name - the coders name
 * Returns:       1 if success, -1 if failed
 */
nomask int
add_member(string project, string name)
{
    object  po, pl;
    mapping p;
    
    po = previous_object();
    if (!(p = projects[project]) || !name || _ACCESS_LEVEL(name) < 1)
	return -1;
    if (_ACCESS_LEVEL(geteuid(po)) <= p[P_LEVEL] &&
	p[P_CREATOR] != geteuid(po))
	return -1;
    projects[project][P_MEMBERS] |= ({ name });
    if (!members[name])
	members[name] = ({ project });
    else
	members[name] |= ({ project });
    if (pl = find_player(name))
	pl->catch_tell("You are now member of project '" + project + "'.\n");
    update_access(({ name }));
    save_object(SAVEFILE);
    return 1;
}

/*
 * Function name: remove_member
 * Description:   remove a coder from a project. Updates file
 *                access for the new member.
 * Arguments:     project - the projects name
 *                name - the coders name
 * Returns:       1 if success, -1 if failed
 */
nomask int
remove_member(string project, string name)
{
    object  po, pl;
    mapping p;

    po = previous_object();
    if (!(p = projects[project]) || !name)
	return -1;
    if (_ACCESS_LEVEL(geteuid(po)) <= p[P_LEVEL] &&
	p[P_CREATOR] != geteuid(po))
	return -1;
    projects[project][P_MEMBERS] -= ({ name });
    if (members[name])
	members[name] -= ({ project });
    if (pl = find_player(name))
	pl->catch_tell("You are no longer member of project '" +
		       project + "'.\n");
    update_access(({ name }));
    save_object(SAVEFILE);
    return 1;
}

/*
 * Function name: query_projects
 * Description:   gives all projects of someone
 * Arguments:     name - player name
 *                pl - the asking coder
 * Returns:       1 if success, -1 if failed
 */
nomask int
query_projects(string name, object pl)
{
    if (!members[name])
	return -1;
    pl->catch_tell(capitalize(name) + " is a member of:\n" +
		   break_string(implode(members[name], " / "), 70, 2) + "\n");
    return 1;
}

/*
 * Function name: change_access
 * Description:   change read write permissions for a project, update access
 *                for all members of this project.
 * Arguments:     project - project name
 *                type - add or remove (+ or -)
 *                perm - permissions to add or remove
 * Returns:       1 if success, -1 if failed
 */
nomask int
change_access(string project, int type, string perm)
{
    string  *access;
    object  po;
    mapping data, p;
    
    po = previous_object();
    if (!(p = projects[project]) || !perm || strlen(perm) < 3 || !type)
	return -1;
    if (_ACCESS_LEVEL(geteuid(po)) <= p[P_LEVEL] &&
	p[P_CREATOR] != geteuid(po))
	return -1;
    if (type != '+' && type != '-')
	return -1;
    if (perm[0] != 's' && perm[0] != 'f')
    {
	if ((perm[0] != 'r' && perm[0] != 'w') ||
	    (perm[1] != '+' && perm[1] != '-') || perm[2] != '/')
	    return -1;
	if (!(data = _Q_access_data(po)))
	    return -1;
	if (!read_or_write_access(perm[2..], perm[0], data[_ADATA_FILE]))
	    return -1;
    }
    if (!(access = p[P_FILEACCESS]))
	access = ({ });
    if (type == '+')
	access |= ({ perm });
    else
	access -= ({ perm });
    
    projects[project][P_FILEACCESS] = access;
    if (project == _ACCESS_DEFAULT)
    {
	access_default = parse_access_data(projects[_ACCESS_DEFAULT]
					   [P_FILEACCESS]);
	update_access(_SYSTEMD->query_coder_list(-1));
    }
    else
	update_access(p[P_MEMBERS]);
    update_access(p[P_MEMBERS]);
    save_object(SAVEFILE);
    return 1;
}
