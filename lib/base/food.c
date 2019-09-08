inherit "heap";

#include <analyse.h>
#include <event.h>
#include <language.h>
#include <macros.h>
#include <properties.h>
#include <skills.h>

nomask void set_amount(int a);

private int     amount, rrmod;
private string  poison;
private mapping status;     /* remember analyse for all players */

/*
 * Function name: create_food
 * Description:   std constructor
 */
static void
create_food()
{
}

static nomask void
create_heap()
{
    set_heap_size(1);
    create_food();
    status = ([ ]);
}

int
eat_cmd()
{
    int    e;
    object tp;

    if (amount == -1)
	return 0;
    tp = this_player();
    e = tp->add_food(amount * query_split_size());
    if (e < 0)
    {
	tp->catch_tell("You cannot eat more.\n");
	return -1;
    }
    if (!e)
	e = 1;
    else
    {
	e /= amount;
	if (!e)
	    e = 1;
    }
    add_msg_size(e);
    tp->catch_msg(({ "You eat ", QNAME(this_object()), ".\n" }), 0, 0);
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " eats ", QNAME(this_object()),
			".\n" }), MSG_SEE, tp);
    set_heap_size(query_heap_size() - e);
    if (poison)
	poison->poison_player(tp, rrmod);
    return 1;
}

/*
 * Function name: set_amount
 * Description:   the 'feeding', nourishing value of this food
 * Arguments:     a - the amount
 */
nomask void
set_amount(int a)
{
    amount = a;
}

/*
 * Function name: query_amount
 * Description:   gives the feeding value, see set_amount
 * Returns:       integer
 */
nomask int
query_amount()
{
    return amount;
}

/*
 * Function name: set_poison
 * Description:   attach a poison effect to this food item
 * Arguments:     file - the poison file (see /lib/poison/)
 *                mod - resistance roll modifier
 *                       (where a value < 0 is harder to resist)
 */
nomask void
set_poison(string file, int mod)
{
    poison = file;
    rrmod = mod;
    if (poison == 0)
	add_prop(IS_POISON, 0);
    else
	add_prop(IS_POISON, 1);
}

/*
 * Function name: query_poison
 * Description:   give the poison attached to this food object, if any
 * Returns:       string, the poison file
 */
nomask string
query_poison()
{
    return poison;
}

int
analyse_cmd(object tp)
{
    if (!tp)
	return -1;
    if (status[tp] == ITEM_TRIED)
    {
	tp->catch_tell("You already tried to analyse this kind.\n");
	return -1;
    }
    if (status[tp] == ITEM_ANALYSED ||
	tp->skill_check(SK_POISON_LORE, 0, 0) > 0)
    {
	status[tp] = ITEM_ANALYSED;
	heap_search_equal(E(this_object()));
	if (!poison)
	    return 0; /* nothing to analyse */
	tp->catch_tell("You find out: It's poisoned! There is\n" +
		       poison->query_desc() + ".\n");
	return 1;
    }
    status[tp] = ITEM_TRIED;
    tp->catch_tell("You fail to analyse the " + short() + ".\n");
    return -1;
}

string
query_heap_id()
{
    if (status[this_player()] == ITEM_ANALYSED)
	return "_a_" + ::query_heap_id();
    return ::query_heap_id();
}

nomask void
set_status(mapping st)
{
    status = st;
}

nomask mapping
query_status()
{
    return status;
}

nomask void
set_analysed(object tp, int state)
{
    if (!tp) return;
    status[tp] = state;
}

nomask int
query_analysed(object tp)
{
    return status[tp];
}
