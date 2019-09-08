/*
 * yavanna_cmd.c
 */
#include <std.h>

/*
 * Am I a soul, or what?
 */

#define CHECK_LEVEL \
if (_MASTER_OB->query_god_level(geteuid(this_interactive())) \
    <= MAIAR) return 0

static mapping cmdlist;

/*
 * Function name: create()
 * Description:   initialize some variables
 * Arguments:     none
 * Returns:       nothing
 */
public void
create()
{
}

/*
 * Function name: get_soul_id()
 * Description:   gives a name for this soul
 * Arguments:     none
 * Returns:       nothing
 */
public string
get_soul_id()
{
    return "yavanna";
}

/*
 * Function name: query_cmdlist()
 * Description:   gives a command mapping
 * Arguments:     none
 * Returns:       the cmds in a mapping
 */
public mapping
query_cmdlist()
{
    return ([
/*	     "ll":"ll",*/

	     "test":"test",

	     "help":"help",
	     ]);
}

/* 
 * Function name: do_command(string cmd, string arg
 * Description:   Perform the given command, if present.
 * Returns:	  1/0 depending on success.
 */
public nomask int
do_command(string cmd, string arg)
{
    string func;

    if (arg=="me") arg = "yavanna";

    if (!cmdlist)
	cmdlist = query_cmdlist();
    
    if (cmd[0] == '*')
    {
	arg = cmd[1..] + (!arg ? "" : " "+arg);
	cmd = "exec";
    }
    else if (cmd[0] == '#')
    {
	if (strlen(arg = cmd[1..] + (!arg ? "" : " "+arg)))
	    arg = "return " + arg + ";";
	cmd = "exec";
    }

    if (!(func = cmdlist[cmd]))
	return 0;

    return call_other(this_object(), func, arg);
}

#include "help.c"
#include "test.c"
