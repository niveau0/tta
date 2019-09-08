/* support to check if an item can be given
 * all items which have been moved are returned within an
 * object array
 */
#include <event.h>
#include <move.h>
#include <properties.h>
#include <slots.h>

private string give_error;

/*
 * Function name: query_give_error
 * Description:   returns an error message that was generated from the
 *                last call to give(). This can be used to send an explaining
 *                message to the giving object.
 * Returns:       string
 */
nomask string
query_give_error()
{
    if (!give_error)
	return "";
    return give_error;
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
 * Function name: try_give
 * Description:   try to give some object to someone (with hands)
 * Arguments:     ob - an array of objects to give
 *                from - the object that is the source of the objects
 *                to - the destination object (needs hands)
 * Returns:       array of given objects
 */
static nomask object *
try_give(object *ob, object from, object to)
{
    int    i, j, sz, sz2, move_err, vol, unit, *hands, *hand_sz;
    string prop;
    object split, *res;

    hands = to->query_slot_type(SLT_HAND);
    if (!(sz2 = sizeof(hands)))
    {
	give_error = "Ouch, " + to->query_pronoun() + " no hands.\n";
	return res;
    }
    
    hand_sz = allocate(sz2);
    for (i=0; i<sz2; i++)
	hand_sz[i] = to->slot_space(hands[i]);
    
    give_error = "";
    for (i=0, sz=sizeof(ob), res=allocate(sz); i<sz; i++)
    {
	for (j=0; j<sz2; j++)
	    if (hand_sz[j] > 0)
		break;
	if (j == sz2)
	{
	    give_error += capitalize(to->query_pronoun()) +
		     " has not that much hands free.\n";
	    break;
	}
	if (call_event(E_REMOVE_TOOL, from, ob[i]))
	    continue;
	if ((vol = ob[i]->query_unit_volume()) > 1) /* heap */
	{
	    unit = hand_sz[j] / vol;
	    if (unit < 1)
		ob[i]->split_heap(1);
	    else if (ob[i]->query_split_size() > unit)
		ob[i]->split_heap(unit);
	    vol *= unit;
	}
	else
	    vol = ob[i]->query_volume();
	
	if (!(move_err = to->add_tool(ob[i], hands[j])))
	{
	    if (split = ob[i]->last_split_obj())
		ob[i] = split;
	    hand_sz[j] -= vol;
	    res[i] = ob[i];
	    continue;
	}
	give_error = move_error(ob[i], give_error, move_err);
    }
    return res - ({ 0 });
}
