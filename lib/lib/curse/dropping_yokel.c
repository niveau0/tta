inherit "/base/curse";

#include <event.h>
#include <macros.h>
#include <slots.h>
#include <state.h>

#define INTERVAL random(100)+200
#define LEVEL    30


static nomask void
curse_effect(object tp)
{
    int i;
    object *list;
    
    if (!tp || !(cursed_players[tp]) || !living(tp))
	return;

    call_out("curse_effect", INTERVAL, tp);

    if(tp->query_state() & (SLEEPING | UNCONSCIOUS))
	return;
    
    list = tp->query_hold(SLT_ALL);

    if (!(i=sizeof(list)))
	return;
    i=random(i);
    if (call_event(E_REMOVE_TOOL, tp, list[i]))
	return;
    if (random(4))
	return;
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " accidentally drops something.\n" }),
		 MSG_SEE);
    if (!random(4))
	tp->catch_tell("Oops, seems that you lost something.\n");
    list[i]->set_coordinates(tp->query_coordinates());
    list[i]->move(E(tp));
}


/*
 * Function name: query_subdesc_id
 * Description:   gives name of subdesc
 * Returns:       string
 */
nomask string
query_subdesc_id()
{
    return "_dropping_yokel_";
}

/*
 * Function name: query_level
 * Description:   gives curse level
 * Returns:       int
 */
nomask int
query_level()
{
    return LEVEL;
}
     
/*
 * Function name: query_desc
 * Description:   gives curse description
 * Returns:       string
 */
nomask string
query_desc()
{
    return "let you drop things out of your inventory";
}

/*
 * Function name: query_interval
 * Description:   gives call_out interval
 * Returns:       int
 */
nomask int
query_interval()
{
    return INTERVAL;
}
