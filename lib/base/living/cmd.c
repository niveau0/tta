/* command related functions for a living */

#include <access.h>
#include <config.h>
#include <macros.h>
#include <kernel.h>
#include <net.h>
#include <properties.h>
#include "prototypes.h"

#define QUEUELENGTH 20 /* cmd queue length */

string *cmdsoul_list; /* saved command souls */

private mixed primary_actions, /* all primary actions on this object */
              *cmd_queue;      /* all pending commands */

/*
 * Function name: cmd_reset
 * Description:   set some default values, init arrays
 */
static nomask void
cmd_reset()
{
    primary_actions = ({ });
    cmdsoul_list = ({ });
}

/*
 * Function name:   add_cmdsoul(string soul)
 * Description:	    Add a command soul to the list of command souls.
 * Arguments:       soul - string with the filename of the command soul.
 * Returns:         1 if success, 0 else
 */
nomask int
add_cmdsoul(string soul)
{
    string euid;
    
    if (!soul || !soul->get_soul_id())
	return 0;
    if (soul->get_soul_level() > 0) /* no coder souls can be added */
	return 0;
    if ((euid = geteuid(previous_object())) != geteuid() &&
	_ACCESS_LEVEL(euid) < _ADMIN)
	return 0;
    cmdsoul_list |= ({ soul });
    return 1;
}

/*
 * Function name:   remove_cmdsoul(string soul)
 * Description:	    Remove a command soul from the list.
 * Arguments:       soul - the filename of the soul to remove
 * Returns:         1 if successful, 0 else
 */
nomask int
remove_cmdsoul(string soul)
{
    string euid;
    
    if ((euid = geteuid(previous_object())) != geteuid() &&
	_ACCESS_LEVEL(euid) < _ADMIN)
	return 0;
    cmdsoul_list -= ({ soul });
    return 1;
}

/*
 * Function name:   query_cmdsoul_list()
 * Description:	    Give back the array with filenames of command souls.
 * Returns:         The command soul list.
 */
nomask string *
query_cmdsoul_list()
{
    return cmdsoul_list; 
}

/*
 * Function name: quit
 * Description:   quitting a possible possession
 * Returns:       1 if there was a possession, 0 else
 */
static int
quit()
{
    if (break_possession(this_object(), this_object()))
	return 1;
    return 0;
}

/*
 * Function name: define_primary
 * Description:   catch all commands in a function
 * Arguments:     func - function to call
 *                obj - object to call function in
 */
nomask void
define_primary(string func, mixed obj)
{
    if (!func)
	BADARG(1, define_primary);
    if (!obj)
	BADARG(2, define_primary);

    primary_actions += ({ obj, func });

    if (_ACCESS_LEVEL(geteuid()) > _BUILDER)
	this_object()->catch_tell("*** define_primary from: " +
				  object_name(previous_object()) + " ***\n");
}

/*
 * Function name: undefine_primary
 * Description:   remove a primary called function
 * Arguments:     func - primary called function
 *                obj - primary called object 
 */
nomask void
undefine_primary(string func, mixed obj)
{
    int    i, sz;

    if (!func)
	BADARG(1, define_primary);
    if (!obj)
	BADARG(2, define_primary);

    for (i=0, sz=sizeof(primary_actions); i<sz; i+=2)
    {
	if (primary_actions[i] == obj && primary_actions[i+1] == func)
	{
	    primary_actions = exclude_array(primary_actions, i, i+1);
	    sz -= 2;
	}
    }
    if (_ACCESS_LEVEL(geteuid()) > _BUILDER)
	this_object()->catch_tell("*** undefine_primary from: " +
				  object_name(previous_object()) + " ***\n");
}

/*
 * Function name: query_primary
 * Description:   give all primary functioncalls added on this object
 * Returns:       array, primary functions and objects
 */
nomask mixed
query_primary()
{
    return primary_actions[..];
}

/*
 * Function name:   action(string cmd, string str)
 * Description:     Try to find and perform a command.
 * Arguments:       cmd - the command
 *                  arg - the argument string
 * Returns:         command return value
 */
static nomask int
action(string cmd, string arg)
{
    int    i, sz, result, state, level;
    object ob;
    
    if (cmd == "quit")
	return quit();
    if (cmd == "password")
    {
	catch_tell("Please input your old password: ");
	input_to("old_password", 1);
	return 1;
    }
    
    for (i=0, sz=sizeof(primary_actions); i<sz; i+=2)
    {
	if (!(ob = primary_actions[i]))
	{
	    primary_actions = exclude_array(primary_actions, i, i+1);
	    sz -= 2;
	}
 	else if (result = call_other(ob, primary_actions[i+1], cmd, arg))
	    return result;
    }
    
    if (!sizeof(cmdsoul_list))
        cmdsoul_list = DEFAULT_SOULS;

    /* process coder commands first */
    if ((level = _ACCESS_LEVEL(geteuid())) > 0)
    {
	if (!(ob = find_object(CODER_SOUL)) &&
	    (catch(ob = compile_object(CODER_SOUL)) || !ob))
	    catch_tell("\nFailed to compile " + CODER_SOUL + ".\n");
	else if (result = call_with_euid(ob, "do_command", cmd, arg))
	    return result;
    }
    
    state = query_state();
    for (i=sizeof(cmdsoul_list); i--;)
    {
	if (!(ob = find_object(cmdsoul_list[i])) &&
	    (catch(ob = compile_object(cmdsoul_list[i])) || !ob))
	{
	    if (level > 0)
		catch_tell("\nFailed to compile " + cmdsoul_list[i] + ".\n");
	    else
		catch_tell("\nYikes, bad soul!\n");
	    continue;
	}
	if (result = call_with_euid(ob, "do_command", cmd, arg, state))
	    return result;
    }
    return 0;
}

/*
 * Function name: command
 * Description:   execute a command
 * Arguments:     cmd - command to process
 * Returns:       1 if success, -1 else, 0 if not found
 */
int
command(string cmd)
{
    int    result;
    string arg;
    object oplayer;

    if (!cmd || !this_object() || !living(this_object()))
	return 0;
    
    sscanf(cmd, "%s %s", cmd, arg);
    if (!strlen(cmd))
    {
	catch_tell("> ", 1);
	return 1;
    }

    oplayer = this_player();
    set_this_player(this_object());
    _SYSTEMD->npc_command();
    
    if (result = action(cmd, arg)) /* cmd successful? */
    {
	set_this_player(oplayer);
	if (!this_object())
	    return result;
	catch_tell("> ", 1);
	return result;
    }
    
    set_this_player(oplayer);
    if (!this_object()) 
	return result;
    catch_tell("It's not possible to '" + cmd + "'.\n> ", 1);
    return 0;
}
    
/*
 * Function name: stop_cmdqueue
 * Description:   stop processing further cmds and clear queue
 */
static nomask void	
stop_cmdqueue()
{
    remove_call_out("next_command");
    cmd_queue = ({ });
}

/*
 * Function name: next_command
 * Description:   process cmdqueue, execute next cmd in queue, called by
 *                call_out()
 */
static nomask void
next_command()
{
    mixed cmd;

    cmd = cmd_queue[0];         /* get current cmd */
    cmd_queue = cmd_queue[2..]; /* remove current cmd and its delay */
    if (sizeof(cmd_queue) > 1)
	/* restart call_out if queue is not empty */
	call_out("next_command", cmd_queue[1]);
    
    if (!pointerp(cmd))
    {
	command(cmd);
	return;
    }
    if (!cmd[0]) /* command object gone? */
	return;
    /* special cmd, ({ object, function, function argument }) */
    set_this_player(this_object());
    call_with_euid(cmd[0], cmd[1], cmd[2], this_object());
}

/*
 * Function name: add_command
 * Description:   add a cmd to the cmdqueue. this can be used to add
 *                special cmds continuation (see next_command()).
 * Arguments:     arg - cmd value to add to cmdqueue
 */
nomask void
add_command(mixed arg)
{
    int    delay;
    string func;
    object user;
    
    if (previous_program() != _USER && geteuid(previous_object()) != geteuid())
	error("Illegal call to add_cmd");
    
    if (stringp(arg))
    {
	if (arg == "cancel")
	{
	    stop_cmdqueue();
	    cmd_queue = ({ });
	    command(arg);
	    return;
	}
	if (!sscanf(arg, "%s %*s", func))
	    func = arg;
	if ((delay = "/cmd/delay"->query_delay(func, this_object())) < 0)
	    return; /* timestopped, command not accepted */

	if (sizeof(cmd_queue) > 1)
	{
	    /* add new command to queue if queue is not full */
	    if (sizeof(cmd_queue) > QUEUELENGTH)
		return;
	    cmd_queue += ({ arg, delay });
	    return;
	}
	if (delay < 1)
	{
	    /* execute non delayed cmd */
	    command(arg);
	    return;
	}
	cmd_queue = ({ arg, delay });
	call_out("next_command", delay);
	return;
    }
    if (pointerp(arg) && sizeof(arg) != 3)
	error("Bad argument 1 to function add_command");
    if (sizeof(cmd_queue))
    {
	/* add new command to queue if queue is not full */
	if (sizeof(cmd_queue) > 20)
	    return;
	cmd_queue += ({ arg, 1 });
	return;
    }
    cmd_queue = ({ arg, 1 });
    call_out("next_command", 1);
}

/*
 * Function name: query_cmdqueue()
 * Description:   Give the current cmdqueue array
 * Returns:       mixed array
 */
nomask string *
query_cmdqueue()
{
    return cmd_queue[..];
}
