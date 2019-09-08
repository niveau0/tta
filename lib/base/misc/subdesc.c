/* this object exits only as master (no cloned instances) and holds all
 * general descriptions for objects. Here subdescriptions can be added
 * 'on the fly' instead of creating an extra object to manage the subdesc
 */

#include <access.h>
#include <macros.h>

private mapping subdescs;

static nomask void
create()
{
    if (IS_CLONE)
    {
	destruct_object();
	return;
    }
    subdescs = ([ ]);
}

nomask void
add_general_subdesc(string subdesc, string descid, object ob)
{
    if (!subdescs[ob])
	subdescs[ob] = ([ ]);
    subdescs[ob][descid] = subdesc;
    ob->add_subdesc(descid, this_object());
}

nomask void
remove_general_subdesc(string descid, object ob)
{
    if (subdescs[ob])
	subdescs[ob][descid] = 0;
    ob->remove_subdesc(descid);
}

nomask mapping
query_general_subdescs()
{
    return subdescs;
}

/*
 * Function name: subdesc
 * Description:   return the subdescription for a subdesc id
 * Arguments:     ob - the object to describe (where the subdesc was added)
 *                for_ob - the object looking 
 *                descid - a unique subdescription id (see <description.h>)
 * Returns:       string
 */
nomask string
subdesc(object ob, object for_ob, string descid)
{
    if (!subdescs[ob])
	return "";
    return subdescs[ob][descid];
}

nomask int
_Q_destruct()
{
    return _ADMIN;
}
