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
	"a"  : "afunc",
	"b"  : "bfunc",
	"yi" : "yifunc",
	"ye" : "yefunc",
	"desti" : "destifunc",
	"deste" : "destefunc",
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

/* show_list - display function for yi and ye cmd
 * code comes from /cmd/coder/I_E.c but modified to display different stuff
 */
private int
show_list(string str, int flag)
{
    int    i, sz;
    object tp, ob, *list;

    tp = this_interactive();
    if (!str)
	ob = tp;
    else if (sizeof(list = parse(str, I(tp, 7) + I(E(tp), 0), 0)))
	ob = list[0];
    else if (!(ob = find_living(str)))
    {
	tp->catch_tell("No such object.\n");
	return -1;
    }
    if (!flag)
	list = I(ob, 0);
    else
	list = I(E(ob), 0);
    
    str = ("Name        Weight  Volume  Size Heap  Filename\n================"+
	   "==============================================================\n");
    for (i=0, sz=sizeof(list); i<sz; i++)
    {
	ob = list[i];
	ob->add_lwvs(0,5,5,5);
	str += sprintf("%./-10s.%./7d.%./7d.%./5d.%./4d..",
		       ob->query_name()+"", ob->query_weight(),
		       ob->query_volume(), ob->query_size(),
		       ob->query_heap_size()) +
	       BS(object_name(ob), 79, 40)[40..] + "\n" ;
    }
    tp->catch_tell(str);
    return 1;
}

/* show_list - display function for yi and ye cmd
 * code comes from /cmd/coder/I_E.c but modified to display different stuff
 */
private int
dest_list(string str, int flag)
{
    int    i, sz;
    object tp, ob, *list;

    tp = this_interactive();
    if (!str)
	ob = tp;
    else if (sizeof(list = parse(str, I(tp, 7) + I(E(tp), 0), 0)))
	ob = list[0];
    else if (!(ob = find_living(str)))
    {
	tp->catch_tell("No such object.\n");
	return -1;
    }
    if (!flag)
	list = I(ob, 0);
    else
	list = I(E(ob), 0);
    
    str = ("Name        Weight  Volume  Size Heap  Filename\n================"+
	   "==============================================================\n");
    for (i=0, sz=sizeof(list); i<sz; i++)
    {
	ob = list[i];
	if (!ob->query_presentation(tp))
	{
            str += sprintf("%./-10s.%./7d.%./7d.%./5d.%./4d..",
		           ob->query_name()+"", ob->query_weight(),
		           ob->query_volume(), ob->query_size(),
		           ob->query_heap_size()) +
	           BS(object_name(ob), 79, 40)[40..] + "\n";
	}
	else
	    str += ob->query_presentation(tp) + "\n";
    }
    tp->catch_tell(str);
    return 1;
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
    tp->catch_tell("Cmd a called with argument " + str + "\n");
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

/* yi - Yirel's inventory
 *   display relevant properties of someone's inventory and modify it
 */
nomask int
yifunc(string str)
{
    return show_list(str, 0);
}

/* ye - Yirel's environment
 *   display relevant properties of someone's environment and modify it
 */
nomask int
yefunc(string str)
{
    return show_list(str, 1);
}

/* desti - dest inventory
 *   dest everything in inventory
 */
nomask int
destifunc(string str)
{
    return dest_list(str, 0);
}

/* deste - deste environment
 *   dest everything in environment (non-player)
 */
nomask int
destefunc(string str)
{
    return dest_list(str, 1);
}
