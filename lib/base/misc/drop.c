/* support to check if an item can be dropped
 * all items which have been moved are returned within an
 * object array
 */
#include <event.h>
#include <move.h>
#include <properties.h>
#include <slots.h>

private string drop_error;

/*
 * Function name: query_drop_error
 * Description:   returns an error message that was generated from the
 *                last call to try_drop(). This can be used to send
 *                an explaining message to the giving object.
 * Returns:       string
 */
nomask string
query_drop_error()
{
    if (!drop_error)
	return "";
    return drop_error;
}

/*
 * move error message
 */
private string
move_error(object ob, string str, int error)
{
    mixed tmp;
    
    if (ob->query_heap_size() > 1)
    {
	switch (error)
	{
	  case MOVE_NOGET:
	    if (stringp(tmp = ob->query_prop(OBJ_NO_GET)))
		str += "The " + ob->plural_short() + " " + tmp;
	    else 
		str += "The " + ob->plural_short() + " cannot be taken.\n";
	    break;
	  case MOVE_2HEAVY:
	    str += "The " + ob->plural_short() + " are too heavy.\n";
	    break;
	  case MOVE_2BIG:
	    str += "The " + ob->plural_short() + " are too big.\n";
	    break;
	  case MOVE_NOCONT:
	    str += "The " + ob->plural_short() + " cannot be put there.\n";
	    break;
	}
    }
    else
    {
	switch (error)
	{
	  case MOVE_NOGET:
	    if (stringp(tmp = ob->query_prop(OBJ_NO_GET)))
		str += "The " + ob->short() + " " + tmp;
	    else 
		str += "The " + ob->short() + " cannot be taken.\n";
	    break;
	  case MOVE_2HEAVY:
	    str += "The " + ob->short() + " is too heavy.\n";
	    break;
	  case MOVE_2BIG:
	    str += "The " + ob->short() + " is too big.\n";
	    break;
	  case MOVE_NOCONT:
	    str += "The " + ob->short() + " cannot be put there.\n";
	    break;
	}
    }
    return str;
}

/*
 * Function name: try_drop
 * Description:   check if item can be dropped or put somewhere
 * Arguments:     ob - an array of objects to drop
 *                container - the destination object
 *                tp - the dropping object
 *                co_tp - coordinates of the dropping object
 * Returns:       array of dropped objects
 */
static nomask object *
try_drop(object *ob, object container, object tp, int *co_tp)
{
    int    i, sz, move_err, room;
    string prop;
    object split, *res;

    room = container->query_prop(IS_ROOM);
    /* move to the ground */
    if (co_tp)
	co_tp[2] = (room
		    ? container->calc_z_coordinate(co_tp)
		    : container->query_coordinates()[2]);
    drop_error = "";
    for (i=0, sz=sizeof(ob), res=allocate(sz); i<sz; i++)
    {
	if (ob[i]->query_wearer())
	{
	    drop_error += "You better remove it first.\n";
	    continue;
	}
	else if (ob[i]->query_wielder())
	{
	    drop_error += "You better unwield it first.\n";
	    continue;
	}	
	if (call_event(E_REMOVE_TOOL, tp, ob[i]))
	    continue;
	if (room && co_tp) /* set coordinates if dropped in room */
	    ob[i]->set_coordinates(co_tp);
	if (!(move_err = ob[i]->move(container)))
	{
	    if (split = ob[i]->last_split_obj())
		ob[i] = split;
	    res[i] = ob[i];
	    continue;
	}
	drop_error = move_error(ob[i], drop_error, move_err);
    }
    return res - ({ 0 });
}

/*
 * Function name: try_put_on
 * Description:   check if items can be put on something
 * Arguments:     ob - an array of objects to drop
 *                surface - the destination object
 *                tp - the dropping player
 * Returns:       array of dropped objects
 */
static nomask object *
try_put_on(object *ob, object surface, object tp)
{
    int    i, sz, move_err, room, *co_tp;
    string prop;
    object split, *res;

    if(!tp)
	return 0;
    if ( surface->query_prop(IS_ROOM))
	/* You cannot put things ON a room */
	return 0;
    drop_error = "";
    for (i=0, sz=sizeof(ob), res=allocate(sz); i<sz; i++)
    {
	/* check, if object is worn or wielded */
	if (ob[i]->query_wearer())
	{
	    drop_error += "You better remove it first.\n";
	    continue;
	}
	else if (ob[i]->query_wielder())
	{
	    drop_error += "You better unwield it first.\n";
	    continue;
	}
	/* notify about removing */
	if (call_event(E_REMOVE_TOOL, tp, ob[i]))
	    continue;

	if ( surface->place_object(ob[i]) )
	{
	    if (split = ob[i]->last_split_obj())
		ob[i] = split;
	    res[i] = ob[i];
	    continue;
	}
    }
    return res - ({ 0 });
}