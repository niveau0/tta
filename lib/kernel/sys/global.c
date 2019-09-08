#include <config.h>
#include <kernel.h>
#include <net.h>
#include <status.h>

private object  command_giver, /* last active living object */
                interactive;   /* last interactive living object */
private mapping players,       /* player mapping */
                monsters;      /* monster mapping */

nomask int
_Q_destruct()
{
    return _ADMIN;
}

static nomask void
create(int arg)
{
    players = ([ ]);
    monsters = ([ ]);
}

/*
 * Function name: delete_interactive
 * Description:   
 * Arguments:     
 * Returns:       
 */
nomask void
delete_interactive()
{
    interactive = 0;
}

/*
 * Function name: global_interactive
 * Description:   
 * Arguments:     
 * Returns:       
 */
nomask void
global_interactive()
{
    if (object_name(previous_object())[..strlen(_USER)-1] != _USER)
	error(object_name(previous_object()) +
	      " illegally called global_interactive");
    
    interactive = previous_object();
}

/*
 * Function name: query_interactive
 * Description:   
 * Arguments:     
 * Returns:       
 */
nomask object
query_interactive()
{
    return interactive;
}

/*
 * Function name: global_this_player
 * Description:   
 * Arguments:     
 * Returns:       
 */
nomask void
global_this_player(object player)
{
    command_giver = player;
}

/*
 * Function name: this_player
 * Description:   
 * Arguments:     
 * Returns:       
 */
nomask object
query_this_player()
{
    return command_giver;
}

/*
 * Function name: global_living_name
 * Description:   
 * Arguments:     
 * Returns:       
 */
nomask void
global_living_name(string name)
{
    object obj, *list;

    obj = previous_object();
    if (!name || !living(obj))
	return;
    
    if (query_ip_number(obj) != 0)
	players[name] = obj;
    else
    {
	if (!pointerp(list = monsters[name]))
	    list = ({ obj });
	else 
	    list = ({ obj }) + (list - ({ 0 }));
	
	monsters[name] = list;
    }
}

/*
 * Function name: query_find_player
 * Description:   
 * Arguments:     
 * Returns:       
 */
nomask object
query_find_player(string name)
{
    return players[name];
}

/*
 * Function name: find_living
 * Description:   
 * Arguments:     
 * Returns:       
 */
nomask object
query_find_living(string name)
{
    object obj, *list;

    if (obj = players[name])
	return obj;

    if (sizeof(list = monsters[name]))
    {
	list -= ({ 0 });
	monsters[name] = list;
	return list[0];
    }
    return 0;
}
