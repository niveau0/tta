/* supports a check-function if some action succeeds to release/losen
 * an item which is fixed with the OBJ_NO_GET property,
 * e.g.: a lantern is fixed to the ground but might be pulled away with
 *       enough strength
 */

#include <living.h>
#include <properties.h>

/*
 * Function name: release
 * Description:   tries to release an item via strength check.
 * Arguments:     ob - the object to release
 *                tp - the acting living
 */
nomask void
release(object ob, object tp)
{
    int    heap;
    string desc;
    mixed  tmp;

    if ((heap = ob->query_heap_size()) < 2)
    {
	heap = 1;
	desc = ob->short();
    }
    else
	desc = ob->plural_short();
    
    if (stringp(tmp = ob->query_prop(OBJ_NO_GET)))
	desc = "The " + desc + " " + tmp;
    else if (tmp < 0)
	desc = "The " + desc + " cannot be taken.\n";
    else
    {
	int result;
	
	if ((result = tp->stat_check(STAT_ST, -tmp*heap, 0)) > 0)
	{
	    if (tmp - result < 0)
		ob->add_prop(OBJ_NO_GET, 0);
	    else
		ob->change_int_prop(OBJ_NO_GET, -result);
	}
	desc = "The " + desc +
	       (heap > 1
		? " are fixed but seem to move a bit.\n"
		: " is fixed but seems to move a bit.\n");
    }
    tp->catch_tell(desc);
}
