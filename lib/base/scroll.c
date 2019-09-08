inherit "object";
inherit "/system/parse";

#include <event.h>
#include <language.h>
#include <macros.h>
#include <material.h>
#include <slots.h>
#include <skills.h>
#include <properties.h>

/*
 * Function name: create_scroll
 * Description:   creates and initializes the scroll
 */
static void
create_scroll()
{
}

/*
 * Function name: create_object
 * Description:   handles the create_object() call and serves it
 *                to the create_scroll() function
 */
static nomask void
create_object()
{
    add_prop(OBJ_MATERIAL, M_PAPER);
    create_scroll();
}

/*
 * Function name: short
 * Description:   modified short() function for handling closed/open scrolls
 * Returns:       string - the propper shortdescription
 */
string
short()
{
    if (query_prop(CONT_CLOSED))
	return "closed " + ::short();
    return ::short();
}

/*
 * Function name: plural_short
 * Description:   modified plural_short() function like 'short' above
 * Returns:       string - the propper pshortdesc
 */
string
plural_short()
{
    if (query_prop(CONT_CLOSED))
	return "closed " + ::plural_short();
    return ::plural_short();
}

/*
 * Function name: long
 * Description:   modified long() function for handling description of scrolls
 * Returns:       string - the propper longdesc
 */
string
long()
{
    string seal_owner;
    
    if (query_prop(CONT_CLOSED))
    {
	if (!seal_owner || !strlen((seal_owner = query_prop("SEAL"))))
	    return ::long() + "(closed)";
	if (seal_owner != "!broken!")
	{
	    if (this_player()->query_met(seal_owner))
		return ::long() + "(sealed by " + seal_owner + ")";
	    return ::long() + "(sealed)";
	}
	return ::long() + "(closed, but seal is broken)";
    }
    if (query_prop("SEAL") == "!broken!")
	return ::long() + "(seal is broken)";
    return ::long();
}

/*
 * Function name: study_cmd
 * Description:   handles the attemped to study (learn) a scroll.
 *                if there is nothing to learn from this scroll, the read_cmd()
 *                function will be called
 * Returns:       -1 if reading is not possible (i.e. scroll is closed)
 *                 1 else
 */
int
study_cmd()
{
    object tp;

    tp = this_player();
    if (query_prop(CONT_CLOSED))
    {
	tp->catch_tell("Maybe, you have to open the " +
		       ::short() + " first?\n");
	return -1;
    }
    tell_objects(LISTEN(E(tp)) - ({tp}),
		 ({QCTNAME(tp), " studies ", QNAME(this_object()),
		   ".\n" }), MSG_SEE, tp);
    tp->catch_tell("You start to study the " + ::short() + ".\n");
    return 1;
}

/*
 * Function name: read_cmd
 * Description:   tells the player, the writings on this scroll, if any on it
 * Returns:       -1 if reading is not possible (i.e. scroll is empty)
 *                 1 else
 */
int
read_cmd()
{
    object tp;

    tp = this_player();
    if (query_prop(CONT_CLOSED))
    {
	tp->catch_tell("May be, you have to open the "+ short() +" first?\n");
	return -1;
    }
    tell_objects(LISTEN(E(tp)) - ({tp}),
		 ({QCTNAME(tp), " reads ", QNAME(this_object()), 
		   ".\n" }), MSG_SEE, tp);
    tp->catch_tell("You are reading the " + ::short() + ".\n");
    return 1;
}

/*
 * Function name: write_cmd
 * Description:   will allow writings on the scroll (using ed)
 * Returns:        1 on success
 *                -1 else
 */
int
write_cmd()
{
    return 1;
}

/*
 * Function name: open_cmd
 * Description:   opens a scroll and destroys the seal if present
 * Returns:        1 if all works fine
 *                -1 else (i.e. the scroll was magically protected)
 */
int
open_cmd()
{
    string seal_owner;
    object tp;

    tp = this_player();
    
    if (!query_prop(CONT_CLOSED))
    {
	tp->catch_tell("But the " + ::short() + " is already open.\n");
	return -1;
    }

    add_prop(CONT_CLOSED,0);

    if (seal_owner && strlen((seal_owner = query_prop("SEAL"))) &&
	(seal_owner != "!broken!"))
    {
	add_prop("SEAL","!broken!");
	tell_objects(LISTEN(E(tp)) - ({tp}),
		     ({ QCTNAME(tp), " opens ", QNAME(this_object()), 
			".\n" }), MSG_SEE, tp);
	tp->catch_tell("You break the seal and open the " + ::short() + ".\n");
	return 1;
    }

    tell_objects(LISTEN(E(tp)) - ({tp}),
		 ({QCTNAME(tp), " opens ", QNAME(this_object()),
		   ".\n" }), MSG_SEE, tp);
    tp->catch_tell("You open the " + ::short() + ".\n");
    
    return 1;
}

/*
 * Function name: close_cmd
 * Description:   closes a scroll and places a seal with the id of the
 *                player, if a candle and a propper object for sealing
 *                is used
 * Arguments:     string - name of object used for sealing the scroll
 * Returns:        1 on success
 *                -1 else
 */
int
close_cmd(string argument)
{
    int    sz;
    string seal_name;
    object *found, seal_obj, tp;

    tp = this_player();
    
    if (query_prop(CONT_CLOSED))
    {
	tp->catch_tell("But the " + ::short() + " is already closed.\n");
	return -1;
    }
    
    add_prop(CONT_CLOSED,1);
    
    tell_objects(LISTEN(E(tp)) - ({tp}),
		 ({QCTNAME(tp), " closes ", QNAME(this_object()), 
		   ".\n" }), MSG_SEE, tp);
	    
    if (strlen(argument))
    {
	if (!(sz = sizeof(found = parse("candle", I(tp, 4), 0))))
 	{
	    tp->catch_tell("You close the " + ::short() + ", but you can't"+
			   " seal it without a candle.\n");
	    return 1;
	}
	
	switch (sz)
	{
	  case 0:
	    tp->catch_tell("You have no such thing to make a seal.\n");
	    return 1;
	    
	  case 1:
	    if (!(seal_name=found->query_seal_name()))
	    {
		tp->catch_tell("You close the " + ::short() +
			       ", but you can't make a propper " +
			       "seal with\n" +
			       "your " + argument + ".\n");
		return 1;
	    }
	    tp->catch_tell(BS("You close the " + ::short() +
			      ", and place a seal of " + seal_name +
			      " with your " +	seal_obj->short() +
			      " on it.\n", 77, 0));
	    add_prop("SEAL", seal_name);
	    return 1;
	    
	  default:
	    tp->catch_tell("Please be precise. There is more than one "+
			   argument + ".\n");
	    return -1;
	}
    }
    
    tp->catch_tell("You close the " + ::short() + ".\n");
    return 1;
}
    
/*
 * Function name: light_cmd
 * Description:   handles the attempt to light/burn the scroll.
 *                may be called, if scroll/player is hit by a fire-attack
 * Returns:       1 if success, -1 else
 */
int
light_cmd()
{
    object tp;

    tp = this_player();
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " burns ", QNAME(this_object()), 
			" to dust.\n" }), MSG_SEE, tp);

    tp->catch_tell("You burn the " + short() + " to dust.\n");
    remove_object();
    return 1;
}
