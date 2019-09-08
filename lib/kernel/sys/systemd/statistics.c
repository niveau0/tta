#define _LEVEL 0
#define _PRINT 1
#define _BIRTH 2

mapping login_count;

static int     total, npctotal, call;
static object  *list;
static mapping cmd_costs, log_costs, people, errors;

static nomask void
init_statistics()
{
    if (!mappingp(log_costs))
	log_costs = ([ ]);
    if (!mappingp(cmd_costs))
	cmd_costs = ([ ]);
    if (!mappingp(login_count))
	login_count = ([ ]);
    if (!mappingp(errors))
	errors = ([ ]);
    call = time();
}

/* called by _LOGIN */
nomask void
notify_login(object ob, int level)
{
    int    day, year, hour, min, sec, sz;
    string wday, mon;

    sz = sizeof(users());
    sscanf(ctime(time()), "%s %s %d %d:%d:%d %d",
	   wday, mon, day, hour, min, sec, year);
    if (login_count[wday] < sz)
	login_count[wday] = sz;
    login_count[hour] = sz;
}

nomask mapping
query_login_count()
{
    return login_count;
}

nomask void
write_logincount()
{
    int    i;
    string str;
    mixed  ind;

    notify_login(0, 0); /* update */
    remove_file("/syslog/logincount.dat");
    ind = map_indices(login_count);
    str = "";
    for (i=sizeof(ind); i--;)
	str += sprintf("%10s %10s\n", ind[i] + "", login_count[ind[i]] + "");
    write_file("/syslog/logincount.dat", str);
}

/*****************************************************************************/

nomask void
log_cost(object ob, string cmd, string euid, int cost)
{
    log_costs[euid] += cost/1000;
    cmd_costs[program_name(ob) + " - " + cmd] += cost/1000;
}

nomask void
euid_cost()
{
    int i, sz;
    string str, *ind;

    str = "\nEuid:                               "+
	  "Costs:\n";
    
    ind = map_indices(log_costs);
    for (i=0, sz=sizeof(ind); i<sz; i++)
	str += sprintf("%-30s  %10d\n", ind[i], log_costs[ind[i]]);
    smore(str, this_player());
}

nomask void
cmd_cost()
{
    int    i, sz;
    string str, *ind;

    str = "\nCommand:                            "+
	  "                              Costs:\n";
    
    ind = map_indices(cmd_costs);
    for (i=0, sz=sizeof(ind); i<sz; i++)
	str += sprintf("%-60s  %10d\n", ind[i], cmd_costs[ind[i]]);
    smore(str, this_player());
}

nomask void
user_command()
{
    total++;
}

nomask void
npc_command()
{
    npctotal++;
}
 
nomask string
cmd_per_second()
{
    int    difference;
    string npc, pls;
    float  npccurrent, current;

    if ((difference = time() - call) > 15)
    {
	current    = (float)total / (float)difference;
	npccurrent = (float)npctotal / (float)difference;
	npctotal   = 0;
	total      = 0;
	call       = time();
	npc = (string)npccurrent;
	pls = (string)current;
	if (strlen(npc) > 4)
	    npc = npc[..3];
	if (strlen(pls) > 4)
	    pls = pls[..3];
    }
    return pls + " cmd/s, " + npc + " npc cmd/s";
}

/*****************************************************************************/

nomask string
object_list(string str)
{
    int    i, from, to;
    string list, n, uid, euid;
    object o;

    to = status()[ST_NOBJECTS];
    if (str)
	sscanf(str, "%d-%d", from, to);
    if (from < 1)
	from = 1;
    if (to < from)
	to = from;
    
    o = find_object(_DRIVER);
    for (i=1; i<from; i++)
    {
	if (!(o = o->_Q_next()))
	    return "";
    }
    list = "Number Name       UID      EUID     Filename\n";
    for (i=from; i<=to; i++)
    {
	uid = getuid(o);
	euid = geteuid(o);
	list += sprintf("%6d %-10s %-8s %-8s %-40s\n", i,
			(n = o->query_name()) ? n : "",
			uid == "-1" ? "system" :
			(uid == "-2" ? "root" : uid + ""),
			euid == "-1" ? "system" :
			(euid == "-2" ? "root" : euid + ""),
			object_name(o));
	
	if (!(o = o->_Q_next()))
	    break;
    }
    return (i - from + 1) + " objects total, " + from + " - " +
        i + ":\n" + list;
}

/*****************************************************************************/

nomask void
set_hostname(string host, object player)
{
    mixed pdata;
    
    if (!player || !(pdata = people[player]))
	return;
    pdata[1] = pdata[1][..14] + sprintf("%-35s ", host) + pdata[1][51..]; 
}

nomask void
add_player(object player)
{
    int    i, sz, level, birth, pos;
    string name;
    mixed  entry;
   
    if (!mappingp(people))
	people = ([ ]);
    if (!pointerp(list))
	list = ({ });
    else
	list -= ({ 0 });
     
    if (member_array(player, list) >= 0)
	return;
    
    if (!(name = player->query_name()))
	return;
    
    level = _Q_coder_level(name);
    birth = player->query_birth();
    
    for (i=0, pos=sz=sizeof(list); i<sz; i++)
    {
	if (list[i])
	{
	    if (level < (entry = people[list[i]])[_LEVEL])
		pos = i;
	    else if (level == entry[_LEVEL] && birth < entry[_BIRTH])
		pos = i;
	}
    }
    if (pos < sz)
	list = list[..pos] + ({ player }) + list[pos+1..];
    else
	list = ({ player }) + list;
	
    people[player] = ({ level, 
			sprintf("%-10s %2s  %-35s ", 
				capitalize(name) + "",
				(level < _BUILDER
				 ? "M"
				 : (level < _CREATOR
				    ? "B"
				    : (level < _ADMIN
				       ? "C" : "A"))),
				query_ip_number(player) + ""),
			birth });
    /*_RESOLVE_P->query(query_ip_number(player), "set_hostname", player);*/
    set_hostname(query_ip_name(player->query_user()), player);
}

nomask void
remove_player(object player)
{
    if (sizeof(list))
    {
	list -= ({ player });
	people[player] = 0;
    }
}

nomask string
userlist(int arg)
{
    int    i, sz, idle;
    string path, e, ret;
    object pl, env;

    list -= ({ 0 });
    for (i=sz=sizeof(list), idle=0, ret=""; i--;)
    {
	pl = list[i];
	if (query_idle(pl) > 300)
	{
	    idle++;
	    ret += "I ";
	}
	else
	    ret += "  ";
	env = pl->environment();
	if (arg == 1)
	    ret += people[pl][1] + geteuid(pl) + "\n";
	else if (!env || !(e = env->query_room_file()))
	    ret += people[pl][1] + "In the big black void\n";
	else if (strlen(path = object_name(env)) > 15 &&
		 path[..15] == "/system/creator/")
	    ret += people[pl][1] + "(editing room)\n";
	else
	{
	    if (sscanf(e, "/d/%s/%*s", path))
		ret += people[pl][1] + "/d/" + path + "/*\n";
	    else if (sscanf(e, "/home/%s/%*s", path))
		ret += people[pl][1] + "/home/" + path + "/*\n";
	    else
		ret += people[pl][1] + e + "\n";
	}
    }
    return ("There are now " + sz + " players (" + (sz-idle) + " active) " +
	    cmd_per_second() + "\n" + ret);
}

/*****************************************************************************/
nomask void
count_error(int flag, string file, string func)
{
    if (flag)
	errors[file] = 1;
    else
	errors[file] = func;
}

nomask string
list_error_stat()
{
    int    i;
    string out, err, *files;
    
    out = "Error statistics:\n";
    files = map_indices(errors);
    for (i=sizeof(files); i--;)
	out += sprintf("%-50s %20s\n", files[i],
		       (stringp(errors[files[i]])
			? errors[files[i]] : "failed compilation"));
    return out;
}
