/* inherit to have the attachment functionality, i.e. the
 * object (which must have an inventory) gets the possibility to
 * hold items in its attach slots.
 * E.g. a bag which is attached to a belt.
 */

#include <composite.h>
#include <event.h>
#include <language.h>
#include <macros.h>
#include <properties.h>
#include <slots.h>

private int    slots;     /* number of attach slots */

/*
 * Function name: set_slots
 * Description:   set the number of attach slots this object should have
 * Arguments:     s - the number of attach slots
 */
static nomask void
set_attach_slots(int s)
{
    slots = s;
}

/*
 * Function name: query_slots
 * Description:   how many attach slots does this object have
 * Returns:       int, number of slots
 */
static nomask int
query_attach_slots()
{
    return slots;
}

/*
 * Function name: show_attached
 * Description:   return a description string for all attached items
 * Arguments:     tp - the looking living
 * Returns:       string, 0 if nothing visible
 */
nomask string
show_attached(object tp)
{
    object *obj;
    
    if (!sizeof(obj = tp->visible(this_object()->attached())))
	return 0;
    return capitalize(BS(COMPOSITE(obj, tp) +
			 ((sizeof(obj) > 1 || obj[0]->query_heap_size() > 1) ?
			  " are " : " is ") + "attached to it.\n", 75, 0));
}

/*
 * Function name: attach_filter
 * Description:   this function can be overwritten to have a filter
 *                functionality. If the filter function returns 0 for an
 *                object, the object cannot be attached.
 * Arguments:     ob - the object to check
 * Returns:       integer
 */
int
attach_filter(object ob)
{
    return 1;
}

/*
 * Function name: attach_check
 * Description:   check if an object is allowed to get attached to this_object
 * Arguments:     ob - the object to attach
 * Returns:       1 if allowed, else 0
 */
nomask int
attach_check(object ob)
{
    int size;
    
    if (sizeof(this_object()->attached()) >= slots ||
	!(size = ob->query_prop(OBJ_ATTACH_SIZE)) ||
	size > this_object()->query_size() ||
	!attach_filter(ob))
	return 0;
    return 1;
}

/*
 * Function name: attach_cmd
 * Description:   called from cmdsoul to attach an item
 * Arguments:     ob - the object to attach to this object
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
attach_cmd(object ob, object tp)
{
    if (!attach_check(ob))
    {
	tp->catch_tell("It's not possible to attach it there.\n");
	return -1;
    }
    ob->attach(this_object());
    tp->echo(({ "You attach ", QTNAME(ob), " to ", QTNAME(this_object()),
		    ".\n" }), 0, 0);
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " attaches ", QNAME(ob), " to ",
			QNAME(this_object()), ".\n" }), MSG_SEE, tp);
    return 1;
}
