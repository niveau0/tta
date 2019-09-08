/*
 * personal cmds
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
    
static mapping cmdlist;

static nomask void
create()
{
    cmdlist = ([
	"a":"afunc",
	"b":"bfunc",
	]);
}

/*
 * Function name: get_soul_id
 * Description:   gives a name for this soul
 */
nomask string
get_soul_id()
{
    return "personal";
}

/*
 * Function name: get_soul_level
 * Description:   minimum required level for this soul,
 *                here: 0, no coder souls can be added with cmdsoul-cmd
 * Returns:       integer
 */
nomask int
get_soul_level()
{
    return 0;
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

    if (!(func = cmdlist[cmd]))
	return 0;

    return call_other(this_object(), func, arg);
}

/***************************************************************************
 * cmd functions
 */
									       
nomask int
afunc(string str)
{
    object tp;

    tp = this_player();
    if (!str)
    {
	tp->catch_tell("Usage: a <arg>\n");
	return -1; /* failure */
    }
    tp->catch_tell("Cmd a called with argument" + str + "\n");
    return 1;
}

									       
nomask int
bfunc(string str)
{
    object tp;

    tp = this_player();
    if (!str)
    {
	tp->catch_tell("Usage: b <arg>\n");
	return -1; /* failure */
    }
    tp->catch_tell("Cmd b called with argument" + str + "\n");
    return 1;
}
