inherit "release";

#include <event.h>
#include <living.h>
#include <macros.h>
#include <properties.h>

/*
 * Function name: pull_cmd
 * Description:   pull this object (or something of this object)
 * Arguments:     what - if something at this object should be pulled
 *                       'what' indicates this item
 *                tp - the pulling object
 * Returns:       1 if success, -1 else
 */
int
pull_cmd(string what, object tp)
{
    int    step, is_liv, *co, *co_tp;
    object to;
    
    if (!tp || what) /* no standard behaviour for this action */
	return -1;
    to = this_object();
    if (!(is_liv = living(to)) && to->query_prop(OBJ_NO_GET))
    {
	release(to, tp);
	return -1;
    }
    
    if (living(tp))
    {
	if (is_liv)
	{
	    if (to->query_stat_bonus(STAT_ST) + roll_dice(-1) >
		tp->query_stat_bonus(STAT_ST) + roll_dice(-1))
		return -1;
	}
	else
	{
	    if (to->query_weight() >
		tp->query_weight() + tp->query_stat_bonus(STAT_ST) * 1000)
		return -1;
	}
    }
    if (!(co_tp = tp->query_coordinates()))
	return -1;
    co = to->query_coordinates();
    step = tp->query_step();
    if (ABS(co[0]-co_tp[0]) < step)
	co[0] = co_tp[0];
    else
    {
	if (co[0] > co_tp[0])
	    co[0] -= step;
	else
	    co[0] += step;
    }
    if (ABS(co[1]-co_tp[1]) < step)
	co[1] = co_tp[1];
    else
    {
	if (co[1] > co_tp[1])
	    co[1] -= step;
	else
	    co[1] += step;
    }
    if (ABS(co[2]-co_tp[2]) < step)
	co[2] = co_tp[2];
    else
    {
	if (co[2] > co_tp[2])
	    co[2] -= step;
	else
	    co[2] += step;
    }
    tp->echo(({ "You successfully pulled ", QTNAME(to), ".\n"}), 0, 0);
    if (is_liv)
	to->catch_msg(({ QCTNAME(tp), " succeeded to pull you.\n" }), 0, 0);
    tell_objects(LISTEN(E(tp)) - ({ to, tp }),
		 ({ QCTNAME(tp), " successfully pulls ", QNAME(to),
			".\n" }), MSG_SEE, tp);
    return 1;
}
