#define FOODHEAP 1
#if FOODHEAP
inherit "/base/heap";
#else
inherit "/base/object";
#endif

#include <event.h>
#include <language.h>
#include <macros.h>
#include <slots.h>

nomask void set_amount(int a);
nomask void set_rot(int rot, int decomp);

/* variables for food */
private int amount;
private string waste_product;

/* variables for rot */
private int time_create;
private int time_rot;
private int time_decompose;

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
#if FOODHEAP
    set_heap_size(1);
#endif
    waste_product = "";
    time_create = time();
    create_food();
}

/*
 * Function name: create_waste
 * Description:   creates waste products from food
 * Arguments:     tp - this player
 */
private void
create_waste(object tp)
{
    object ob;
    int       *hands, *co;

    if (catch(ob = clone_object(waste_product)))
    {
        tp->catch_tell("Failed.\n");
        return;
    }
    if (!sizeof(hands = tp->query_slot_type(SLT_HAND)) ||
        tp->add_tool(ob, hands[0]))
    {
     /* move into room if cloned object is living, has no hands or
        object cannot be moved to 'cloner' */
        if (co = tp->query_coordinates()[..])
        {
           /* move to the ground */
           co[2] = E(tp)->query_coordinates()[2];
           ob->set_coordinates(co);
        }
        ob->move(E(tp));
    }
}

int
eat_cmd()
{
    int    e;
    object tp;

    if (amount == -1)
    return 0;
    tp = this_player();
#if FOODHEAP
    e = tp->add_food(amount * query_split_size());
#else
    e = tp->add_food(amount);
#endif
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
    
#if FOODHEAP
    add_msg_size(e);
#endif
    tp->catch_msg(({ "You eat ", QNAME(this_object()), ".\n" }), 0, 0);
    tell_objects(LISTEN(E(tp)) - ({ tp }),
         ({ QCTNAME(tp), " eats ", QNAME(this_object()),
            ".\n" }), MSG_SEE, tp);
#if FOODHEAP
    set_heap_size(query_heap_size() - e);
#endif
    if (waste_product != "")
    create_waste(tp);
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
 * Function name: set_waste_product
 * Description:   the item produced when the food is eaten
 * Arguments:     str - the waste product
 */
nomask void
set_waste_product(string str)
{
    waste_product = str;
}

/*
 * Function name: set_rot
 * Description:   the amount of time it takes for this food to rot
 * Arguments:     rot - the amount of time until it the food rots
 *                decomp - the amount of time from when the food rots
 *                         until it decomposes entirely.
 */
nomask void
set_rot(int rot, int decomp) 
{
    time_rot = time_create + rot;
    time_decompose = time_rot + decomp;
}

static nomask void
check_rot()
{
}

static nomask string
rot_status()
{
    int progress;
    int p1,p2;
    string str;
    p1 = (time_rot - time_create);
    p2 = (time_rot - time() + 1);
    progress = p1/p2;
    if (progress < 2)
        return sprintf("%d\n%d\n%d\n",time_create,time_rot,time())
	+ sprintf("%d/%d=%d ",p1,p2,progress) + " It seems quite fresh.\n";
    else if (progress < 4)
        return "It is no longer fresh.\n";
    else 
        return "It looks like it might go bad soon.\n";
}


nomask void
do_rot()
{
}

nomask void
do_decompose()
{
}

string
long()
{
    check_rot();
    return ::long() + rot_status();
}

/*
short()
{
    check_rot();
    ::short();
}
*/
