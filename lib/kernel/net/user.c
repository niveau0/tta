#include <config.h>
#include <kernel.h>
#include <status.h>
#include <type.h>

private int     echo,       /* is input echoing turned on */
                timestamp,  /* last time something was typed */
                in_cmd,     /* prompt flag */
                line,       /* line offset of last read file */
                offset,     /* byte offset of last read file */
                ftime;      /* file time of last read file */
private string  exit_func,  /* editor/input_to exit function */
                file;       /* last read file */
private object  player,     /* player object (active) */
                splayer,    /* saved player object */
                snooper,    /* snooper of this object */
                editor,     /* editor object */
                exit_obj;   /* editor/input_to exit object */
private mixed   input_args; /* optional arguments to input_to */

/*
 * Function name: open
 * Description:   start a user connection on this object
 */
static nomask void
open()
{
    rlimits(30; 10000000)
    {
	player = clone_object(_LOGIN);
	player->set_user(this_object());
	echo = 1;
	timestamp = time();
	set_this_player(player); /* needed for calling input_to */
	player->logon();
    }
}

/*
 * Function name: close
 * Description:   close this user connection on this object.
 * Arguments:     flag - 1 if the object is already destructed,
 *                       0 if connection is gone
 */
nomask void
close(int flag)
{
    rlimits(30; 10000000)
    {
	if (flag)
	    return;
	if (editor)
	{
	    editor->rescue_file();
	    destruct_object(editor);
	}
	if (player || splayer)
	{
	    if (splayer)
	    {
		player->set_linkdead(1);
		player = splayer;
	    }
	    _SYSTEMD->remove_player(player); /* fix people list */
	    player->set_linkdead(1);
	    _LOGIN->notify_coders(player, 2); /* linkdead msg */
	}
	if (this_object())
	    destruct_object();
    }
}

/*
 * Function name: disconnect
 * Description:   forcibly break connection.
 */
nomask void
disconnect()
{
    if (!PRIVILEGED())
	return;
    rlimits(30; 10000000)
    {
	if (editor)
	{
	    editor->rescue_file();
	    destruct_object(editor);
	}
	_LOGIN->notify_coders(player, 3); /* log out msg */
	destruct_object();
    }
}

/*
 * Function name: ed
 * Description:   start an editor session. should be called from cmdsoul.
 * Arguments:     file - file to edit
 *                func - function to call after exiting
 */
nomask varargs void
ed(string file, string func)
{
    if (!player || !::query_ip_number(this_object()) ||
	geteuid(player) != geteuid(previous_object()))
	error("Illegal call to ed");
    if (editor) 
	error("Editor already active");

    editor = clone_object(_EDITOR);
    if (func)
    {
	exit_obj = previous_object();
	exit_func = func;
    }
    if (!file || !strlen(file))
	editor->edit("");
    else 
	editor->edit("e " + file);
}

/*
 * Function name: receive_message
 * Description:   receive user input from telnet port
 * Arguments:     str - input string
 */
static nomask void
receive_message(string str)
{
    int    i;
    string func;
    object obj;
    mixed  args;
    
    rlimits(100; 10000000)
    {	
	if (!player)
	{
	    send_message("You are possessed and cannot do anything yet.\n> ");
	    return;
	}
	set_this_player(player);
	timestamp = time();
	if (echo && snooper)
	{
	    if (splayer) /* possessed splayer */
		snooper->catch_tell(str + "\n");
	    else /* only snooping */
		snooper->catch_tell("%" + capitalize(player->query_name()) +
				    "> " + str + "\n");
	}
	else
	    send_message(echo = 1);

	if (str && strlen(str) > 0 && str[0] == '!') /* 'escape' command */
	    str = str[1..];
	else if (editor)	/* editor active */
	{
	    editor->edit(str);
	    if (!(func = query_editor(editor)))
	    {
		destruct_object(editor);
		if (exit_obj == 0)
		    player->catch_tell("Editor left.\n");
		else
		{
		    obj = exit_obj;
		    func = exit_func;
		    exit_obj = 0;
		    exit_func = 0;
		    call_other(obj, func);
		}
	    }
	    else
		player->catch_tell((func == "insert" ? "*" : ":"), 1);
	    return;
	}
	else if (exit_obj)	/* input_to */
	{
	    obj  = exit_obj;
	    func = exit_func;
	    args = input_args;
	    exit_obj  = 0;
	    exit_func = 0;
	    input_args  = 0;
	    if (sizeof(args))
		obj->_F_call(func, str, args...);
	    else
                obj->_F_call(func, str); 
	    return;
	}
	if ((i = strlen(str) - 1) > 0 && str[i--] == ' ')
	{
	    while (i > 0 && str[i] == ' ') /* remove trailing spaces */
		i--;
	    str = str[..i];
	}
	player->add_command(str);
	return;
    }
}

/*
 * Function name: set_player
 * Description:   set the playerobject connected to this user
 * Arguments:     obj - player object
 */
nomask void
set_player(object obj)
{
    if (!PRIVILEGED())
	return;
    player = obj;
    rlimits(10; 10000)
    {
	obj->set_user(this_object());
    }
}

/*
 * Function name: query_player
 * Description:   gives the playerobject connected to this user
 * Returns:       object
 */
nomask object
query_player()
{
    return player;
}

/*
 * Function name: query_status
 * Description:   returns a status flag, depending on input mode
 * Returns:       0 if user is in standard command mode
 *                1 if user is in editor mode
 *                2 if user is in input_to mode
 */
nomask int
query_status()
{
    if (editor)
	return 1;
    if (exit_obj)
	return 2;
    return 0;
}

/*
 * Function name: set_interactive
 * Description:   switches between in-command and inactivity (for security),
 *                by setting the interactive object.
 * Arguments:     flag - true if preparing command, 0 else
 */
nomask int
set_interactive(int flag)
{
    if (previous_object() != player)
	error("Illegal call to set_interactive");
    
    if (flag)
    {
	_GLOBAL->global_interactive(); 
	in_cmd = 1;
    }
    else
    {
	_GLOBAL->delete_interactive();
	in_cmd = 0;
    }
}

/*
 * Function name: catch_tell
 * Description:   send a message to the telnet port and send a prompt
 * Arguments:     str - string to send
 *                no_prompt - if true, no prompt will be sent
 */
nomask varargs void
catch_tell(string str, int no_prompt)
{
    string name;

    if (!str)
	return;

    if (snooper)
    {
	if (splayer) /* I'm a demon (possessing) */
	    snooper->catch_tell(str);
	else /* snooped */
	    snooper->catch_tell("%" +
				(name = capitalize(player->query_name())) +
				":\n" + str + "%" + name + "\n");
    }
    if (in_cmd || no_prompt || exit_obj || editor)
    {
	send_message(str);
	return;
    }
    send_message(str + "> ");
}

/*
 * Function name: _F_set_input_to
 * Description:   initialise input catching 
 * Arguments:     func - function to call at next input
 *                flag - if true send no echo
 *                args - extra argument to 'func'
 * Returns:       1 if success, else 0
 */
nomask varargs int
_F_set_input_to(string func, int flag, mixed args...)
{
    if (!PRIVILEGED() || exit_obj != 0)
	return 0;

    exit_obj  = previous_object();
    exit_func  = func;
    input_args = args;
    if (flag)
	send_message(echo = 0);
    return 1;
}

/*
 * Function name: _F_snoop
 * Description:   set a snoop on this object
 * Arguments:     snoopy - the snooping object
 */
nomask void
_F_snoop(object snoopy)
{
    if (PRIVILEGED()) 
	snooper = snoopy;
}

/*
 * Function name: _Q_snoop
 * Description:   gives the snooping object if anyone is snooping
 * Returns:       snooping object
 */
nomask object
_Q_snoop()
{
    if (PRIVILEGED()) 
	return snooper;
}

/*
 * Function name: set_file_offset
 * Description:   store the offset and line number for a file
 * Arguments:     f - filename
 *                l - line
 *                o - file offset
 *                t - filetime
 */
nomask void
set_file_offset(string f, int l, int o, int t)
{
    if (PRIVILEGED())
    {
	file = f;
	line = l;
	offset = o;
	ftime = t;
    }
}

/*
 * Function name: query_file_offset
 * Description:   return the line and offset of a file, if stored  
 * Arguments:     f - the filename
 * Returns:       array ({ line, offset, filetime })
 */
nomask int *
query_file_offset(string f)
{
    if (file == f) 
	return ({ line, offset, ftime });
    return 0;
}

/*
 * Function name: _Q_idle
 * Description:   gives the idle time (time since last input)
 * Returns:       int, idle time
 */
nomask int
_Q_idle()
{
    return (time() - timestamp);
}

/*
 * Function name: switch_player
 * Description:   switch to a new playerobject or possible none
 * Arguments:     new - new player object to switch to
 */
nomask void
switch_player(object new)
{
    if (PRIVILEGED())
    {
	splayer = player;
	player  = new;
    }
}

/*
 * Function name: query_splayer
 * Description:   returns the saved player
 * Returns:       object, saved player
 */
nomask object
query_splayer()
{
    return splayer;
}

/*
 * Function name: _F_reset_splayer
 * Description:   reset the saved player
 */
nomask void
_F_reset_splayer()
{
    if (PRIVILEGED()) 
	splayer = 0;
}