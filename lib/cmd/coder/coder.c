/*
 * coder cmds
 */

inherit "/system/parse";

#include <access.h>
#include <combat.h>
#include <composite.h>
#include <config.h>
#include <event.h>
#include <filepath.h>
#include <kernel.h>
#include <language.h>
#include <limits.h>
#include <macros.h>
#include <magic.h>
#include <money.h>
#include <mudtime.h>
#include <properties.h>
#include <slots.h>
#include <status.h>

#define SHUT_REASON "_shut_reason"

/*
 * an object is not allowed to execute a command,
 * - if a call from another object has different effective userid
 * - if this objects effective userid is less then the given level
 */
#define CHECK_LEVEL(level) \
if ((previous_object() != this_object() &&       \
     geteuid(previous_object()) != geteuid()) || \
    _ACCESS_LEVEL(geteuid()) < level)            \
{seteuid("0");this_interactive()->catch_tell("Not permitted.\n");return -1;}

private void scan_props();
static void do_shut_down(mixed info);
    
static int     timestamp, memory; /* used for status cmd */
static mapping cmdlist, props;

static nomask void
create()
{
    cmdlist = ([
	"@":"god",
	":":"emote",
	";":"control_cmd",
	"I":"I_list",
	"E":"E_list",
	
	"access":"access",
	"addspell":"addspell",
	
	"banish":"banish",
	"blocksite":"blocksite",

	"c":"clone",
	"callouts":"call_outs",
	"cat":"cat_file",
	"cd":"cd",
	"cleanup":"cleanup",
	"clone":"clone",
	"cmds":"cmds",
	"cmdsoul":"cmdsoul",
	"control":"control",
	"cp":"cp_cmd",
	"create":"cr_create",
	"cstat":"cstat",

	"demote":"demote",
	"dd":"destruct_ob",
	"destruct":"destruct_ob",

	"echo":"echo",
	"echoto":"echo_to",
	"ed":"ed",
	"editplayer":"editplayer",
	"email":"email",
	"emote":"emote",
	"errorstat":"errorstat",
	"exec":"execute",

	"finger":"finger",
	"force":"force",

	"god":"god",
	"goto":"goto_dest",

	"heal":"heal",
	"home":"home",

	"idle":"do_idle",
	"indent":"indent",
	"invis":"invis",

	"ichlist":"ichlist",
	"iinfo":"iinfo",
	"ijoin":"ijoin",
	"ileave":"ileave",
	"ilist":"ilist",
	"imsg":"imsg",
	"iping":"iping",
	"itell":"itell",
	"iwho":"iwho",

	"listboni":"listboni",
	"load":"load",
	"loaddir":"loaddir",
	"lockout":"lockout",
	"log":"show_errorlog",
	"ls":"list",

	"man":"manual",
	"met":"metflag",
	"mkdir":"makedir",
	"money":"money",
	"more":"more_file",
	"msg":"msg_toggle",
	"mv":"mv_cmd",

	"objects":"objectlist",

	"paths":"paths",
	"people":"people",
	"possess":"possess",
	"project":"project",
	"promote":"promote",
	"props":"props",
	"pwd":"pwd",

	"resurrect":"resurrect",
	"review":"review",
	"rl":"reload",
	"reload":"reload",
	"rm":"rm_cmd",
	"rmdir":"removedir",

	"setmsg":"setmsg",
	"shutdown":"shutdown_game",
	"snoop":"snoop_on",
	"stat":"stat",
	"status":"mud_status",

	"tail":"tail_file",
	"tell":"tell",
	"tellall":"tellall",
	"trans":"trans",

	"uh":"uh",
	"unmet":"unmetflag",
	"upd":"update_ob",
	"update":"update_ob",
	"updir":"updir",
	"uptime":"uptime",
	"users":"user_list",

	"vis":"vis",
		
	"whereis":"whereis",
	]);
    
    scan_props();
}

/*
 * Function name: get_soul_id
 * Description:   gives a name for this soul
 */
nomask string
get_soul_id()
{
    return "coder";
}

/*
 * Function name: get_soul_level
 * Description:   minimum required level for this soul
 * Returns:       integer
 */
nomask int
get_soul_level()
{
    return _BUILDER;
}

/*
 * Function name: query_cmdlist
 * Description:   gives a command mapping
 * Returns:       the cmds in a mapping
 */
nomask mapping
query_cmdlist()
{
    return cmdlist;
}

/* 
 * Function name: do_command
 * Description:   Perform the given command, if present.
 * Returns:	  1/0 depending on success.
 */
nomask int
do_command(string cmd, string arg)
{
    string func;

    if (cmd[0] == ';')
    {
	arg = cmd[1..] + (!arg ? "" : " "+arg);
	cmd = ";";
    }
    else if (cmd[0] == '*')
    {
	arg = cmd[1..] + (!arg ? "" : " "+arg);
	cmd = "exec";
    }
    else if (cmd[0] == '=')
    {
	if (strlen(arg = cmd[1..] + (!arg ? "" : " "+arg)))
	    arg = "return " + arg + ";";
	cmd = "exec";
    }

    if (!(func = cmdlist[cmd]))
	return 0;

    return call_other(this_object(), func, arg);
}

/*
 * Function name: scan_props
 * Description:   scan the <properties.h> file and create a mapping
 *                propname : code, for later use of the 'props' cmd.
 */
private void
scan_props()
{
    int    i, j, code;
    string prop, *lines;

    props = ([ ]);
    lines = explode(read_file("/include/properties.h"), "\n");
    
    for (i=sizeof(lines); i--;)
    {
	if (sscanf(lines[i], "#define %s %d%*s\n", prop, code) == 2)
	{
	    j = strlen(prop);
	    while (prop[--j] == ' ' || prop[j] == '\t');
	    prop = prop[..j]; /* remove trailing spaces */
	    props[code] = prop;
	}
    }
}

/*
 * Function name: find_file
 * Description:   try to find file <name> within one of the directories
 *                in this_interactive() CODER_PATH-property
 * Arguments:     name - the filename to search
 * Returns:       the full path if found, 0 else
 */
nomask string
find_file(string name)
{
    int    i, j, l;
    string current, *paths, *dir;
    object tp;
    
    if (!(tp = this_interactive()) ||
	!pointerp(paths = tp->query_prop(CODER_PATH)))
	return 0;

    for (i=sizeof(paths); i--;)
    {
	if (!(l = strlen(current = paths[i])))
	    continue;
	if (current[l-1] == '*') /* one wildcard is allowed */
	{
	    dir = get_dir(current);
	    for (j=sizeof(dir); j--;)
	    {
		current = current[..l-2] + dir[j] + "/" + name;
		if (file_size(current) > 0)
		    return current;
	    }
	    continue;
	}
	if (current[l-1] != '/')
	    current += "/";
	if (file_size(current + name) > 0)
	    return current + name;
    }
    return 0;
}

#include "I_E.c"
#include "access.c"
#include "addspell.c"
#include "banish.c"
#include "blocksite.c"
#include "callouts.c"
#include "cat.c"
#include "cd.c"
#include "cleanup.c"
#include "clone.c"
#include "cmds.c"
#include "cmdsoul.c"
#include "control.c"
#include "cp_mv_rm.c"
#include "create.c"
#include "cstat.c"
#include "demote.c"
#include "destruct.c"
#include "echo.c"
#include "echoto.c"
#include "ed.c"
#include "editplayer.c"
#include "email.c"
#include "emote.c"
#include "errorstat.c"
#include "exec.c"
#include "finger.c"
#include "force.c"
#include "god.c"
#include "goto.c"
#include "heal.c"
#include "home.c"
#include "idle.c"
#include "indent.c"
#include "intermud.c"
#include "invis.c"
#include "listboni.c"
#include "load.c"
#include "loaddir.c"
#include "lockout.c"
#include "log.c"
#include "ls.c"
#include "man.c"
#include "met.c"
#include "mkdir.c"
#include "money.c"
#include "more.c"
#include "msg.c"
#include "objects.c"
#include "paths.c"
#include "people.c"
#include "possess.c"
#include "project.c"
#include "promote.c"
#include "props.c"
#include "pwd.c"
#include "reload.c"
#include "resurrect.c"
#include "review.c"
#include "rmdir.c"
#include "setmsg.c"
#include "shutdown.c"
#include "snoop.c"
#include "stat.c"
#include "status.c"
#include "tail.c"
#include "tell.c"
#include "tellall.c"
#include "trans.c"
#include "uh.c"
#include "unmet.c"
#include "update.c"
#include "updir.c"
#include "uptime.c"
#include "users.c"
#include "vis.c"
#include "whereis.c"
