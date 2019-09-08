/* support to check if an item can be taken
 * all items which have been moved are returned within an
 * object array
 */
inherit "release";

#include <event.h>
#include <living.h>
#include <move.h>
#include <properties.h>
#include <slots.h>

private string get_error;

/*
 * Function name: query_get_error
 * Description:   returns an error message that was generated from the
 *                last call to try_get(). This can be used to send an
 *                explaining message to the giving object.
 * Returns:       string
 */
nomask string
query_get_error()
{
    if (!get_error)
	return "";
    return get_error;
}

/*
 * move error message
 */
private string
move_error(object ob, string str, int error, object tp)
{
    mixed tmp;
    
    if (ob->query_heap_size() > 1)
    {
	switch (error)
	{
	  case MOVE_NOGET:
	      release(ob, tp);
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
	      release(ob, tp);
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

/***************************************************************************
 * check if item can be taken
 */
static nomask object *
try_get(object *ob, object tp, int *co_tp)
{
    int    i, j, sz, sz2, move_err, vol, unit, *hands, *hand_sz;
    object split, *res;
    
    hands = tp->query_slot_type(SLT_HAND);
    if (!(sz2 = sizeof(hands)))
    {
	get_error = "Ouch, you have no hands.\n";
	return res;
    }
    hand_sz = allocate(sz2);
    for (i=0; i<sz2; i++)
	hand_sz[i] = tp->slot_space(hands[i]);
    
    get_error = "";
    for (i=0, sz=sizeof(ob), res=allocate(sz); i<sz; i++)
    {
	for (j=0; j<sz2; j++)
	    if (hand_sz[j] > 0)
		break;
	if (j == sz2)
	{
	    get_error += "You cannot carry that much in your hands.\n";
	    break;
	}
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
	
	if (!(move_err = tp->add_tool(ob[i], hands[j])))
	{
	    if (split = ob[i]->last_split_obj())
		ob[i] = split;
	    hand_sz[j] -= vol;
	    res[i] = ob[i];
	    continue;
	}
	get_error = move_error(ob[i], get_error, move_err, tp);
    }
    return res - ({ 0 });
}
