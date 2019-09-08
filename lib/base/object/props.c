#include <composite.h>
#include <macros.h>
#include <properties.h>
#include <state.h>

static mapping  obj_props;	/* Object properties */

static nomask void
init_props()
{
    obj_props = ([ ]);
}

/*
 * Function name: query_prop
 * Description:   Find the value of a property.
 * Arguments:	  prop - The property searched for.
 * Returns:       The value or 0.
 */
nomask mixed
query_prop(mixed prop)
{
    if (!obj_props)
	return 0;
    if (stringp(prop = obj_props[prop]))
	return process_value(prop);
    return prop;
}

/*
 * Function name: change_int_prop
 * Description:   increase a property
 *                Only works if property is an integer
 * Arguments:	  prop - The property string to be added.
 *                val - The value to add
 * Returns:       1 if success
 */
nomask int
change_int_prop(mixed prop, int val)
{
    int    oval;
    object e;
    
    if (val && intp(oval = obj_props[prop]))
    {
	obj_props[prop] = oval + val;
	if (e = E(this_object()))
	    e->notify_change_prop(prop, oval + val, oval);
	return 1;
    }
    return 0;
}


/*
 * Function name: add_prop
 * Description:   Add a property to the property list.
 *                If the property already exist, the value is replaced
 * Arguments:	  prop - The property string to be added.
 *                val - The value of the property
 */
nomask void
add_prop(mixed prop, mixed val)
{
    object e;
    mixed  oval;

    oval = query_prop(prop);
    obj_props[prop] = val;

    if (val != oval && (e = E(this_object())))
	e->notify_change_prop(prop, query_prop(prop), oval);
}

/*
 * Function name: query_props
 * Description:   Give all the existing properties
 * Returns:       The propertie map
 */
nomask mixed
query_props()
{
    return obj_props;
}

/*
 * Function name: add_state
 * Description:   set state bit(s) for this object (property LIVE_STATE) 
 * Arguments:     s - the bit(s) to set (see <state.h>)
 */
nomask void
add_state(int s)
{
    obj_props[LIVE_STATE] |= s;
}

/*
 * Function name: delete_state
 * Description:   clear state bit(s)
 * Arguments:     s - bit(s) to clear
 */
nomask void
delete_state(int s)
{
    s &= obj_props[LIVE_STATE]; /* get all matching bits */
    if (s)
	obj_props[LIVE_STATE] -= s;
}

/*
 * Function name: query_state
 * Description:   return current state
 * Returns:       integer
 */
nomask int
query_state()
{
    return obj_props[LIVE_STATE];
}

/*
 * Function name: query_state_desc
 * Description:   returns a string describing the objects states
 * Arguments:     tp - living to describe the state for
 * Returns:       describing string of the form
 *                desc1, desc2, desc3, ... and descn
 */
nomask string
query_state_desc(object tp)
{
    int    state;
    string *statedesc;

    if (!(state = obj_props[LIVE_STATE]))
	return "";
    statedesc = allocate(15);

    if (state & RESTING) 
	statedesc[0] = "resting";
    else if (state & SLEEPING)
	statedesc[1] = "sleeping";
    else if (state & MEDITATING)
	statedesc[2] = "meditating";
    else if (state & ON_GROUND)
	statedesc[3] = "sitting on the ground";
    if (state & PARALYSED)
	statedesc[4] = "paralysed";
    if (state & UNCONSCIOUS)
	statedesc[5] = "outknocked";
    if (state & COMBAT)
	statedesc[6] = "in battle";
    if (state & CONCENTRATE)
	statedesc[7] = "concentrating";
    if (state & WALKING)
	statedesc[8] = "walking";
    if (state & SEARCHING)
	statedesc[9] = "currently searching";
    if (state & HIDING) 
	statedesc[10] = "lurking in the shadows";
    if (state & GHOST) 
	statedesc[11] = "in an etheral state";
    if (state & WORKING)
	statedesc[12] = "working";
    /* if (state & MOUNT)
     *     statedesc[13] = "riding";
     */
    statedesc -= ({ 0 });
    if (!sizeof(statedesc))
	return "";
    return ((this_object() == tp ? "You are " :
	     capitalize(tp->query_pronoun()) + " is ") + 
	    COMPOSITE_WORDS(statedesc) + ".\n");
}
